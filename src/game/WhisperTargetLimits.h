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

#include <unordered_map>
#include <cstdint>
#include <ctime>

class WhisperTargetLimits {
    const std::uint32_t account_id_;
    const std::uint32_t max_targets_;
    const std::uint32_t bypass_level_;
    const std::uint32_t decay_;
    std::unordered_map<std::uint32_t, std::uint64_t> targets_;

    void load_targets();
    void save_targets();
    void target_decay(const std::time_t& time);

public:
    WhisperTargetLimits(std::uint32_t account_id, std::uint32_t max_targets,
                        std::uint32_t bypass_level, std::uint32_t decay);
    ~WhisperTargetLimits();

    bool can_whisper(std::uint32_t target_guid, std::uint32_t level);
};