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

#pragma once

#include <atomic>
#include <ace/Semaphore.h>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <curl/curl.h>

class VPNLookup {
public:
    enum class Result
    {
        VPN, NOT_VPN, INTERNAL_ERROR
    };

    typedef std::function<void(Result)> LookupCallback;

    enum class ConfigParam {
        IPQS_KEY, GII_CONTACT, VPNBLOCK_KEY
    };

private:
    std::unordered_map<std::uint32_t, bool> _cache;
    std::unordered_map<ConfigParam, std::string> _params;

    enum class CacheResult {
        VPN, NOT_VPN, CACHE_MISS
    };

    struct LookupRequest {
        std::uint32_t ip;
        LookupCallback callback;
    };

    static VPNLookup* _global_service;

    void load_cache();
    void process();
    CacheResult lookup_cache(std::uint32_t ip);
    Result do_lookup(std::uint32_t ip);
    Result lookup_vpnblocker(const std::string& ip, const std::string& key);
    Result lookup_ipintel(const std::string& ip, const std::string& key);
    Result lookup_ipqs(const std::string& ip, const std::string& key);
    CURLcode curl_perform(const char* uri, std::string& response, long& resp_code);
    
    std::queue<LookupRequest> _queue;
    std::mutex _queue_lock;
    std::mutex _cache_lock;
    std::thread _worker;
    ACE_Semaphore _semaphore;
    std::atomic_bool _stop;

public:
    VPNLookup();
    ~VPNLookup();

    void shutdown();
    void reload_cache();
    void lookup(std::uint32_t ip, LookupCallback callback);
    void set_param(ConfigParam param, std::string value);
    Result blocking_lookup(std::uint32_t ip);

    static void set_global_vpnlookup(VPNLookup* service);
    static VPNLookup* get_global_vpnlookup();
};