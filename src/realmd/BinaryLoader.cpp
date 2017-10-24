/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 * Copyright (C) 2009-2011 MaNGOSZero <https://github.com/mangos/zero>
 * Copyright (C) 2011-2016 Nostalrius <https://nostalrius.org>
 * Copyright (C) 2016-2017 Elysium Project <https://github.com/elysium-project>
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

#include "BinaryLoader.h"
#include "Log.h"
#include <fstream>
#include <sstream>

std::unique_ptr<BinaryLoader> binaryLoader;

BinaryLoader::BinaryLoader()
{
    const std::vector<std::string> winx86
    {
        "WoW.exe", "fmod.dll", "ijl15.dll",
        "dbghelp.dll", "unicows.dll"
    };

    const std::vector<std::string> macx86
    {
        "MacOS/World of Warcraft", "Info.plist",
        "Resources/Main.nib/objects.xib",
        "Resources/wow.icns", "PkgInfo"
    };

    const std::vector<std::string> macppc
    {
        "MacOS/World of Warcraft", "Info.plist",
        "Resources/Main.nib/objects.xib",
        "Resources/wow.icns", "PkgInfo"
    };

    const std::vector<uint16_t> builds
    {
        5875, 6005, 6141
    };

    for (auto& build : builds)
    {
        loadClientData(build, winx86, 'Win', 'x86');
        loadClientData(build, macppc, 'Mac', 'PPC');
        loadClientData(build, macx86, 'Mac', 'x86');
    }
}

/// todo: we C++17 yet?
const ClientData* BinaryLoader::fetchData(uint32_t os, uint32_t platform,
    uint16_t build) const
{
    auto it = std::find_if(clientData.begin(), clientData.end(), [&](auto& data)
    {
        return data.os == os && data.platform == platform && data.build == build;
    });

    return it != clientData.end() ? &(*it) : nullptr;
}

void BinaryLoader::loadClientData(uint16_t build, const std::vector<std::string>& filenames,
    uint32_t os, uint32_t platform)
{
    // temp hacks
    std::string platformStr = platform == 'x86' ? "x86" : "ppc";
    std::string osStr = os == 'Win' ? "Win" : "Mac";

    std::stringstream dirName;
    dirName << osStr << "_" << platformStr << "_" << build;

    std::vector<std::vector<uint8_t>> binaries;
    std::vector<uint8_t> buffer;

    for (auto& filename : filenames)
    {
        std::string path(dirName.str() + "/" + filename);
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            sLog.outError("Unable to load client binary (%s), skipping version", path.c_str());
            return;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        buffer.resize(static_cast<std::size_t>(size) + buffer.size());

        if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
        {
            sLog.outError("Error! Could not read client binary (%s)", path.c_str());
            exit(-1);
        }

        binaries.emplace_back(std::move(buffer));
    }

    clientData.emplace_back(ClientData{ os, platform, build, std::move(binaries) });
    sLog.outBasic("Loaded binaries for %s %s, build %u", osStr.c_str(), platformStr.c_str(), build);
}