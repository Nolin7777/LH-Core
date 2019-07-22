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

#include "WhisperTargetLimits.h"
#include <algorithm>
#include <memory>
#include "Database/DatabaseEnv.h"

WhisperTargetLimits::WhisperTargetLimits(std::uint32_t account_id, std::uint32_t max_targets,
                                         std::uint32_t bypass_level, std::uint32_t decay)
    : account_id_(account_id), max_targets_(max_targets), bypass_level_(bypass_level), decay_(decay) {
    load_targets();
}

WhisperTargetLimits::~WhisperTargetLimits() {
    save_targets();
}

void WhisperTargetLimits::save_targets() {
    CharacterDatabase.PExecute("DELETE FROM `whisper_targets` WHERE `account` = %u", account_id_);

    for(const auto& target : targets_) {
        CharacterDatabase.PExecute(
            "INSERT INTO `whisper_targets` (`account`, `target_guid`, `time`) VALUES (%u, %u, %u)",
            account_id_, target.first, target.second
        );
    }
}

void WhisperTargetLimits::load_targets() {
    auto res = std::unique_ptr<QueryResult>(CharacterDatabase.PQuery("SELECT `target_guid`, `time` FROM `whisper_targets` WHERE `account` = %u", account_id_));

    if(!res) {
        sLog.outError("Unable to load whisper targets for account ID %u.", account_id_);
        return;
    }

    do {
        auto fields = res->Fetch();
        targets_[fields[0].GetUInt32()] = fields[1].GetUInt64();
    } while(res->NextRow());
}

bool WhisperTargetLimits::can_whisper(const std::uint32_t target_guid, const std::uint32_t level) {
    if(level >= bypass_level_ || !max_targets_) {
        return true;
    }

    const auto it = targets_.find(target_guid);

    if(it != targets_.end()) {
        return true;
    }

    const auto time = std::time(nullptr);
    
    if(targets_.size() >= max_targets_) {
        target_decay(time);
    }

    if(targets_.size() >= max_targets_) {
        return false;
    }

    targets_[target_guid] = time;
    return true;
}

void WhisperTargetLimits::target_decay(const std::time_t& time) {
    for(auto it = std::begin(targets_); it != end(targets_);) {
        if ((time - it->second) >= decay_) {
            it = targets_.erase(it);
        } else {
            ++it;
        }
    }
}