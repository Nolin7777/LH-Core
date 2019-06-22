#pragma once

#include "Policies/Singleton.h"
#include "Map.h"

class Player;

// horrible design but who cares? I have <24h to make this work.
class RealmEventAnnounce {
    void announce_level_up(std::uint32_t level, const Player& player);

public:
    enum class MapID { // not actual IDs... obviously
        MC, ONYXIA, BWL, ZG, AQ20, AQ40, NAXX
    };

    void boss_kill(std::uint32_t boss_id);
    void boss_kill(std::uint32_t boss_id, const Player& player);
    void boss_kill(std::uint32_t boss_id, const Map::PlayerList& players);
    void raid_clear(MapID id, const Map::PlayerList& players);
    void level_up(std::uint32_t level, const Player& player);
    void quest_complete(std::uint32_t quest, const Player& player);
    void skill_acquired(std::uint32_t skill, const Player& player);
    void item_acquired(std::uint32_t item, const Player& player);
};

#define sRealmEventAnnounce MaNGOS::Singleton<RealmEventAnnounce>::Instance()