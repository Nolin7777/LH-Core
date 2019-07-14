/*
 * Copyright (C) 2019 Light's Hope <https://lightshope.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "VPNLookup.h"
#include "json.hpp"
#include "Log.h"
#include "Database/DatabaseEnv.h"
#include <chrono>
#include <sstream>

using json = nlohmann::json;

VPNLookup* VPNLookup::_global_service;

std::string ip_to_str(std::uint32_t ip) {
    std::stringstream ss;
    ss << (ip >> 24) << "." << ((ip & ~0xFF00FFFF) >> 16) << "."
       << ((ip & ~0xFFFF00FF) >> 8) << "." << ((ip & ~0xFFFFFF00));
    return ss.str();
}

VPNLookup::VPNLookup() : _stop(false) {
    load_cache();
    _worker = std::thread(&VPNLookup::process, this);
}

void VPNLookup::process() {
    while(!_stop) {
        _semaphore.acquire();

        std::unique_lock<std::mutex> guard(_queue_lock);

        if(_queue.empty()) {
            continue;
        }

        auto local_queue = std::move(_queue);
        guard.unlock();

        while(!local_queue.empty()) {
            auto& lookup_request = local_queue.front();
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // crap rate limiting
            const auto res = blocking_lookup(lookup_request.ip);
            lookup_request.callback(res);
            local_queue.pop();
        }
    }
}

VPNLookup::Result VPNLookup::do_lookup(const std::uint32_t ip) {
    const auto ip_str = ip_to_str(ip);

    bool error = false;
    VPNLookup::Result res { VPNLookup::Result::INTERNAL_ERROR };
    auto it = _params.find(ConfigParam::GII_CONTACT);

    // decent service apart from being so heavily rate-limited as to make
    // it entirely useless for real-world applications. Why's this here?
    if(it != _params.end()) {
        res = lookup_ipintel(ip_str, it->second);

        if(res == Result::VPN) {
            return Result::VPN;
        } else if(res == Result::INTERNAL_ERROR) {
            error = true;
        }
    }
    

    it = _params.find(ConfigParam::VPNBLOCK_KEY);

    // low cost service but it's not that good (false negatives)
    if(it != _params.end()) {
        res = lookup_vpnblocker(ip_str, it->second);

        if(res == Result::VPN) {
            return Result::VPN;
        } else if(res == Result::INTERNAL_ERROR) {
            error = true;
        }
    }
    
    it = _params.find(ConfigParam::IPQS_KEY);

    // more expensive, so use it if the cheaper services don't flag
    // any positives
    if(it != _params.end()) {
        res = lookup_ipqs(ip_str, it->second);

        if(res == Result::VPN) {
            return Result::VPN;
        } else if(res == Result::INTERNAL_ERROR) {
            error = true;
        }
    }

    return !error? Result::NOT_VPN : Result::INTERNAL_ERROR;
}

void VPNLookup::load_cache() {
    auto res = std::unique_ptr<QueryResult>(LoginDatabase.PQuery("SELECT `ip`, `is_vpn` FROM `vpn_cache`"));

    if(!res) {
        sLog.outError("Unable to load VPN cache.");
        return;
    }

    do {
        auto fields = res->Fetch();
        _cache[fields[0].GetUInt32()] = fields[1].GetBool();
    } while(res->NextRow());
}

void VPNLookup::reload_cache()
{
    std::lock_guard<std::mutex> lock(_cache_lock);
    load_cache();
}

VPNLookup::CacheResult VPNLookup::lookup_cache(const std::uint32_t ip) {
    // save on lookups by assuming the entire class C (/24)
    // block will return the same result
    std::lock_guard<std::mutex> guard(_cache_lock);
    const auto masked_ip = ip & ~0xFF;
    const auto it = _cache.find(masked_ip);

    if(it != _cache.end()) {
        return it->second? CacheResult::VPN : CacheResult::NOT_VPN;
    }

    return CacheResult::CACHE_MISS;
}

VPNLookup::Result VPNLookup::blocking_lookup(const std::uint32_t ip) {
    const auto cache_res = lookup_cache(ip);

    if(cache_res != CacheResult::CACHE_MISS) {
        return cache_res == CacheResult::VPN? Result::VPN : Result::NOT_VPN;
    }

    const auto res = do_lookup(ip);

    if(res != Result::INTERNAL_ERROR) {
        const auto masked_ip = ip & ~0xFF;
    
        {   
            std::lock_guard<std::mutex> guard(_cache_lock);
            _cache[masked_ip] = (res == Result::VPN);
        }

        const auto res_val = (res == Result::VPN);
        LoginDatabase.PExecute("INSERT INTO vpn_cache (`ip`, `is_vpn`) VALUES (%u, %u)", masked_ip, res_val);
    }

    return res;
}

void VPNLookup::lookup(const std::uint32_t ip, LookupCallback callback) {
    const auto cache_res = lookup_cache(ip);

    if(cache_res != CacheResult::CACHE_MISS) {
        callback(cache_res == CacheResult::VPN? Result::VPN : Result::NOT_VPN);
        return;
    }

    std::lock_guard<std::mutex> guard(_queue_lock);
    _queue.push({ip, std::move(callback)});
    _semaphore.release();
}

void VPNLookup::shutdown() {
    if(_worker.joinable()) {
        _stop = true;
        _semaphore.release();
        _worker.join();
    }
}

VPNLookup::~VPNLookup() {
    shutdown();
}

void VPNLookup::set_param(ConfigParam param, std::string value) {
    if(value.empty()) {
        return;
    }

    _params[param] = std::move(value);
}

size_t curl_write_cb(void *ptr, std::size_t size, std::size_t nmemb, std::string* data) {
    data->append(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

CURLcode VPNLookup::curl_perform(const char* uri, std::string& response, long& resp_code) {
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, uri);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    auto res = curl_easy_perform(curl);

    if(res == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp_code);
    }

    curl_easy_cleanup(curl);
    return res;
}

// https://www.ipqualityscore.com/api/json/ip/YOUR_API_KEY_HERE/USER_IP_HERE
VPNLookup::Result VPNLookup::lookup_ipqs(const std::string& ip, const std::string& key) try {
    std::stringstream uri;
    uri << "https://www.ipqualityscore.com/api/json/ip/"
        << key << "/" << ip << "?strictness=1&allow_public_access_points=true";

    const std::string full_uri { uri.str() };
    std::string response;
    long resp_code = 0;
    auto curl_res = curl_perform(full_uri.c_str(), response, resp_code);

    if(curl_res != CURLE_OK) {
        sLog.outError("CURL failed to perform query, with code %u", curl_res);
        return Result::INTERNAL_ERROR;
    }

    if (resp_code != 200) {
        sLog.outError("Error performing query with IPQS. Response code: %ul", resp_code);
        return Result::INTERNAL_ERROR;
    }

    auto json = json::parse(response);
    auto success = json.at("success"); // why doesn't this work with <bool>?
    
    if(!success) {
        sLog.outError("Error performing query with IPQS. Response: %s", response.c_str());
        return Result::INTERNAL_ERROR;
    }

    return json.at("proxy")? Result::VPN : Result::NOT_VPN;
} catch (const std::exception& e) {
    sLog.outError("Error performing query with IPQS. Exception: %s, ", e.what());
    return Result::INTERNAL_ERROR;
}

// http://check.getipintel.net/check.php?ip=IPHere&contact=YourEmailAddressHere
VPNLookup::Result VPNLookup::lookup_ipintel(const std::string& ip, const std::string& key) try {
    std::stringstream uri;
    uri << "http://check.getipintel.net/check.php?ip=" << ip << "&contact=" << key
        << "&format=json";

    const std::string full_uri { uri.str() };
    std::string response;
    long resp_code = 0;

    auto curl_res = curl_perform(full_uri.c_str(), response, resp_code);
    
    if(curl_res != CURLE_OK) {
        sLog.outError("CURL failed to perform query, with code %u", curl_res);
        return Result::INTERNAL_ERROR;
    }

    if (resp_code != 200) {
        sLog.outError("Error performing query with GetIPIntel. Response code: %ul", resp_code);
        return Result::INTERNAL_ERROR;
    }

    auto json = json::parse(response);
    auto status = json.at("status").get<std::string>();
    
    if(status != "success") {
        sLog.outError("Error performing query with GetIPIntel. Response: %s", response.c_str());
        return Result::INTERNAL_ERROR;
    }

    // returned value is between 0 and 1 but whatever
    return json.at("result").get<std::string>() == "1"? Result::VPN : Result::NOT_VPN;
} catch (const std::exception& e) {
    sLog.outError("Error performing query with GetIPIntel. Exception: %s", e.what());
    return Result::INTERNAL_ERROR;
}

// http://api.vpnblocker.net/v2/json/IPHere/APIKeyHere
VPNLookup::Result VPNLookup::lookup_vpnblocker(const std::string& ip, const std::string& key) try {
    std::stringstream uri;
    uri << "http://api.vpnblocker.net/v2/json/" << ip << "/" << key;

    std::string response;
    long resp_code = 0;

    auto curl_res = curl_perform(uri.str().c_str(), response, resp_code);
    
    if(curl_res != CURLE_OK) {
        sLog.outError("CURL failed to perform query, with code %u", curl_res);
        return Result::INTERNAL_ERROR;
    }

    if (resp_code != 200) {
        sLog.outError("Error performing query with VPNBlocker. Response code: %ul", resp_code);
        return Result::INTERNAL_ERROR;
    }

    auto json = json::parse(response);
    auto status = json.at("status").get<std::string>();

    if(status != "success") {
        sLog.outError("Error performing query with VPNBlocker. Response: %s", response.c_str());
        return Result::INTERNAL_ERROR;
    }

    return json.at("host-ip")? Result::VPN : Result::NOT_VPN;
} catch (const std::exception& e) {
    sLog.outError("Error performing query with VPNBlocker. Exception: %s", e.what());
    return Result::INTERNAL_ERROR;
}

void VPNLookup::set_global_vpnlookup(VPNLookup* service) {
    _global_service = service;
}

VPNLookup* VPNLookup::get_global_vpnlookup() {
    return _global_service;
}