#include "RealmEventAnnouncements.h"
#include "Policies/SingletonImp.h"
#include "Player.h"
#include "World.h"
#include "Language.h"
#include "Util.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "SharedDefines.h"
#include "../shared/json.hpp"
#include <curl/curl.h>
#include <unordered_map>

struct BossDetails {
    std::uint32_t event_id;
    std::uint32_t music_id;
    std::uint32_t sound_id;
    std::string message;
};

const std::unordered_map<std::uint32_t, const BossDetails> boss_events {
    { 10184, { REALM_FIRST_ONYXIA_CLEAR,   5035, 3523, "Onyxia's influence over Stormwind's nobility is no more, thanks to the brave adventurers from %guild. A promising beginning but greater challenges lie in wait..." } },
    { 11502, { REALM_FIRST_MC_CLEAR,       6077, 7555, "You have defeated me too soon, %guild! Congratulations on the realm first Ragnaros kill." }},
    { 11583, { REALM_FIRST_BWL_CLEAR,      6076, 8469, "Congratulations to %guild on their realm first Nefarian kill. Let the games begin!" }},
    { 15339, { REALM_FIRST_AQ20_CLEAR,     5194, 8656, "With the purging of The Ruins of Ahn'Qiraj, the sandstorms tinting the sky orange began to subside. Congratulations %guild, for realm first Ossirian the Unscarred!" }},
    { 15727, { REALM_FIRST_AQ40_CLEAR,     8577, 8700, "With their hearts still in one piece, for the most part, %guild emerged triumphant over C'Thun." }},
    { 14834, { REALM_FIRST_ZG_CLEAR,       8452, 8468, "Zul'Gurub has fallen! Hakkar the Soulflayer is no more. Congratulations, %guild." }},
    { 15990, { REALM_FIRST_NAXX_CLEAR,     8885, 8691, "With Kel'thuzad's defeat, the chilled veil hanging over Azeroth begins to lift. Against all odds, %guild have emerged from Naxxramas, victorious." }},
    { 14887, { REALM_FIRST_YSONDRE,        4434, 3523, "Ysondre has returned to the Emerald Nightmare..." }},
    { 14890, { REALM_FIRST_TAERAR,         3900, 3523, "Taerar has returned to the Emerald Nightmare..." }},
    { 14889, { REALM_FIRST_EMERISS,        6669, 3523, "Emeriss has returned to the Emerald Nightmare..." }},
    { 14888, { REALM_FIRST_LETHON,         4517, 3523, "Lethon has returned to the Emerald Nightmare..." }},
    { 448,   { REALM_FIRST_HOGGER,         5036,  308, "More bones to gnaw on! Hogger has been defeated!" }},
    { 448,   { REALM_FIRST_SKERAM,         8493, 8616, "%guild shows promise in The Temple of Ahn'Qiraj with their realm first Prophet Skeram kill!" }},
    { 6109,  { REALM_FIRST_AZUREGOS,       4517, 3523, "Azuregos has returned to his spirit form..." }},
    { 12397, { REALM_FIRST_KAZZAK,         5354, 8095, "Kazzak stands watch over the blackended crater no more." }},
    { 12435, { REALM_FIRST_RAZORGORE,      6077, 3523, "Blackwing Lair has been breached. %guild have defeated Razorgore." }},
    { 16998, { REALM_FIRST_BIGGLESWORTH,   4041, 8879, "No! A curse upon you, interlopers! The armies of the Lich King will hunt you down. You will not escape your fate! %guild have defeated Mr Bigglesworth." }},
    { 6499,  { REALM_FIRST_DEVILSAUR_KILL, 0000, 6159, "With their skins no longer having any economic value, the devilsaurs looked forward to roaming free during the final days. Alas, they did not anticipate %player's arrival. Realm first devilsaur kill." }},
    { 6500,  { REALM_FIRST_DEVILSAUR_KILL, 0000, 6159, "With their skins no longer having any economic value, the devilsaurs looked forward to roaming free during the final days. Alas, they did not anticipate %player's arrival. Realm first devilsaur kill." }},
    { 6584,  { REALM_FIRST_DEVILSAUR_KILL, 0000, 6159, "With their skins no longer having any economic value, the devilsaurs looked forward to roaming free during the final days. Alas, they did not anticipate %player's arrival. Realm first devilsaur kill." }},
    { 6498,  { REALM_FIRST_DEVILSAUR_KILL, 0000, 6159, "With their skins no longer having any economic value, the devilsaurs looked forward to roaming free during the final days. Alas, they did not anticipate %player's arrival. Realm first devilsaur kill." }},
    { 15931, { REALM_FIRST_GROBBULUS,      8677, 8711, "%guild have received their slime innoculations. Realm first Grobbulus." }},
    { 15932, { REALM_FIRST_GLUTH,          8677, 8724, "Thanks to the veterinarians of %guild, Gluth, the foul plague-dog of Naxxramas, is no more." }},
    { 15928, { REALM_FIRST_THADDIUS,       8883, 8870, "Thank... you... %guild. Realm first Thaddius." }},
    { 15954, { REALM_FIRST_NOTH,           8677, 8848, "I will serve the master... in death. Noth the Plaguebringer has been defeated by %guild." }},
    { 15936, { REALM_FIRST_HEIGAN,         8886, 1332, "It's just a jump to the left, and then a step to the right. With your hands on your hips, you bring your knees in tight. But it's the screaming that really drives you insane. Let's do the Heigan dance again. Realm first Heigan the Unclean for %guild." }},
    { 16011, { REALM_FIRST_LOATHEB,        8677, 8828, "%guild has slain Loatheb." }},
    { 15956, { REALM_FIRST_ANUBREKHAN,     8677, 8789, "Shhh, %guild... it will all be over soon. Realm first Anub'Rekhan." }},
    { 15953, { REALM_FIRST_FAERLINA,       8677, 8798, "The master will avenge her! %guild has slain Grand Widow Faerlina." }},
    { 15952, { REALM_FIRST_MAEXXNA,        8887, 0000, "Maexxna — born deep within the mountains of Northrend many ages ago, she was captured within the dread citadel Naxxramas, where she gives birth to her brood, feeding them the corpses of all who dare venture too deeply into Naxxramas. %guild has slain Maexxna." }},
    { 16061, { REALM_FIRST_RAZUVIOUS,      8687, 8860, "%guild, my finest students! An honorable... death. Realm first Razuvious." }},
    { 16060, { REALM_FIRST_GOTHIK,         8677, 8805, "He is... undone. %guild have escaped with their souls and slain Gothik." }},
    { 181366,{ REALM_FIRST_FOUR_HM,        8677, 6159, "Their bodies battered and bruised, %guild pressed on. The voices of their warriors were hoarse, yet they still commanded the attention of their foes. The Four Horsemen have been defeated." }},
    { 16028, { REALM_FIRST_PATCHWERK,      8888, 8911, "No more play? %guild have defeated Patchwerk." }},
    { 15989, { REALM_FIRST_SAPPHIRON,      8890, 8729, "As Sapphiron's skeletal remains clattered against the glistening floor, so did the swords and shields of %guild's warriors. Their bones ached and their hands were numb. The frostbite was taking its toll and these heroes knew they would not be able to withstand the chill for much longer. One final push..." }},
};

const std::unordered_map<std::uint8_t, std::pair<uint32_t, std::string>> class_events {
    { CLASS_WARRIOR,  {REALM_FIRST_60_WARRIOR, "warrior" } },
    { CLASS_PALADIN,  {REALM_FIRST_60_PALADIN, "paladin" } },
    { CLASS_HUNTER,   {REALM_FIRST_60_HUNTER,  "hunter"  } },
    { CLASS_ROGUE,    {REALM_FIRST_60_ROGUE,   "rogue"   } },
    { CLASS_PRIEST,   {REALM_FIRST_60_PRIEST,  "priest"  } },
    { CLASS_SHAMAN,   {REALM_FIRST_60_SHAMAN,  "shaman"  } },
    { CLASS_MAGE,     {REALM_FIRST_60_MAGE,    "mage"    } },
    { CLASS_WARLOCK,  {REALM_FIRST_60_WARLOCK, "warlock" } },
    { CLASS_DRUID,    {REALM_FIRST_60_DRUID,   "druid"   } }
};

INSTANTIATE_SINGLETON_1(RealmEventAnnounce);

void discord_post(const std::string& message, std::uint32_t id, 
                  const std::string post = "http://netslum.cc:8080/post/progress",
                  std::uint32_t entity_id = 0, const std::string name = "") {
    CURL* curl = curl_easy_init();

    if (!curl)
    {
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, post.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); // :O
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); // :O!
    curl_slist* list = nullptr;
    list = curl_slist_append(list, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    nlohmann::json data
    {
        { "event_id", id },
        { "description", message },
        { "id", entity_id },
        { "name", name },
    };

    const auto str = data.dump();
    const auto cstr = str.c_str();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, cstr);
    curl_easy_perform(curl);
    curl_slist_free_all(list);
    curl_easy_cleanup(curl);
}

// look ma, free function because I don't have time for recompiling
std::pair<std::uint32_t, std::string> determine_guild(const Map::PlayerList& players) {
    std::unordered_map<uint32_t, unsigned int> counts;

    // writing a piece of code so ghastly is the dev equivalent of
    // kicking a puppy or stepping on a kitten
    for(const auto& player_ref : players) {
        const auto player = player_ref.getSource();
        ++counts[player->GetGuildId()];
    }

    unsigned int max_count = 0;
    std::uint32_t guild_id = 0;
    bool tied = false;
    std::uint32_t tied_guild_id = 0;

    for(auto itr : counts) {
        if(itr.second > max_count) {
            max_count = itr.second;
            guild_id = itr.first;
        }
    }

    for(auto itr : counts) {
        if(itr.second == max_count && itr.first != guild_id) {
            tied = true;
            tied_guild_id = itr.first;
        }
    }

    if(tied && !guild_id) {
        guild_id ^= tied;
        tied ^= guild_id;
        guild_id ^= tied;
    }

    std::string guild_name = guild_id? sGuildMgr.GetGuildNameById(guild_id) : "a rag-tag pickup group";

    if(tied) {
        guild_name += " & " + (tied_guild_id? sGuildMgr.GetGuildNameById(tied_guild_id) : "a rag-tag pickup group");
    }
    
    return { guild_id, guild_name };
}

// super basic (read: crap), only replaces the first instance of %guild
std::string format_message(std::string message, std::string name) {
    const auto pos = message.find("%guild");

    if(pos != std::string::npos) {
        message.replace(pos, 6, name);
    }

    return message;
}

// super basic (read: crap), only replaces the first instance of %player
std::string format_pmessage(std::string message, std::string name) {
    const auto pos = message.find("%player");

    if(pos != std::string::npos) {
        message.replace(pos, 7, name);
    }

    return message;
}

void RealmEventAnnounce::boss_kill(std::uint32_t boss_id, const Map::PlayerList& players) {
    const auto it = boss_events.find(boss_id);

    if(it == boss_events.end()) {   
        return;
    }

    bool ignore = false;
    const auto& guild_details = determine_guild(players);

    // Any Naxx boss hack
    if((boss_id == 15954 || boss_id == 16061 || boss_id == 16028 || boss_id == 15956)
        && !sObjectMgr.GetSavedVariable(REALM_FIRST_ANY_NAXX_BOSS, 0, &ignore)) {
        const auto& message = format_message("Naxxramas has been breached by %guild!", guild_details.second);
        sObjectMgr.SetSavedVariable(REALM_FIRST_ANY_NAXX_BOSS, 1, true);
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, message.c_str());
        discord_post(message, REALM_FIRST_ANY_NAXX_BOSS);
    }

    if(!sObjectMgr.GetSavedVariable(it->second.event_id, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(it->second.event_id, 1, true);
    
        const auto& message = format_message(it->second.message, guild_details.second);
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, message.c_str());
        discord_post(message, it->second.event_id);

        if(it->second.sound_id) {
            sWorld.SendWorldSound(it->second.sound_id);
        }

        if(it->second.music_id) {
            sWorld.SendWorldMusic(it->second.music_id);
        }
    }

    // screw it, send it again
    discord_post("", it->second.event_id, "https://lightshope.org/api/progress",
        guild_details.first, guild_details.second);
}

void RealmEventAnnounce::boss_kill(std::uint32_t boss_id, const Player& player) {
    const auto it = boss_events.find(boss_id);

    if(it == boss_events.end()) {
        return;
    }

    bool ignore = false;

    if(!sObjectMgr.GetSavedVariable(it->second.event_id, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(it->second.event_id, 1, true);

        const auto& message = format_pmessage(it->second.message, player.GetName());
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, message.c_str());
        discord_post(message, it->second.event_id);
        discord_post("", it->second.event_id, "https://lightshope.org/api/progress/",
        player.GetEntry(), player.GetName());

        if(it->second.sound_id) {
            sWorld.SendWorldSound(it->second.sound_id);
        }

        if(it->second.music_id) {
            sWorld.SendWorldMusic(it->second.music_id);
        }
    }    
}

void RealmEventAnnounce::boss_kill(std::uint32_t boss_id) {
    const auto it = boss_events.find(boss_id);

    if(it == boss_events.end()) {
        return;
    }

    bool ignore = false;

    if(!sObjectMgr.GetSavedVariable(it->second.event_id, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(it->second.event_id, 1, true);
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, it->second.message.c_str());
        discord_post(it->second.message, it->second.event_id);
        discord_post("", it->second.event_id, "https://lightshope.org/api/progress/");

        if(it->second.sound_id) {
            sWorld.SendWorldSound(it->second.sound_id);
        }

        if(it->second.music_id) {
            sWorld.SendWorldMusic(it->second.music_id);
        }
    }    
}

void RealmEventAnnounce::raid_clear(MapID id, const Map::PlayerList& players) {
    sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, "Raid clear.");
}

void RealmEventAnnounce::generic_level_up(std::uint32_t level, const Player& player) {
    bool ignore = false;
    
    if(level >= 10 && !sObjectMgr.GetSavedVariable(REALM_FIRST_LEVEL_N + level, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_LEVEL_N + level, 1, true);
        std::stringstream ss;
        const std::string played = secsToTimeString(player.GetTotalPlayedTime(), false, false);

        ss << player.GetName() << " has hit level " << level << " in " << played << "!";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_LEVEL_N + level);
        discord_post("", REALM_FIRST_LEVEL_N + level, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }
}

void RealmEventAnnounce::announce_level_up(std::uint32_t level, const Player& player) {
    bool ignore = false;

    if(level == 20 && !sObjectMgr.GetSavedVariable(REALM_FIRST_20, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_20, 1, true);
        std::stringstream ss;
        const std::string played = secsToTimeString(player.GetTotalPlayedTime(), false, false);

        ss << player.GetName() << " has hit level 20 in " << played << "! The race to 60 is underway...";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_20);
        discord_post("", REALM_FIRST_20, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if(level == 30 && !sObjectMgr.GetSavedVariable(REALM_FIRST_30, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_30, 1, true);
        std::stringstream ss;
        const std::string played = secsToTimeString(player.GetTotalPlayedTime(), false, false);

        ss << "Ahead of the pack, " << player.GetName() << " has hit level 30 in " << played << "!";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_30);
        discord_post("", REALM_FIRST_30, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if(level == 40 && !sObjectMgr.GetSavedVariable(REALM_FIRST_40, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_40, 1, true);
        std::stringstream ss;
        const std::string played = secsToTimeString(player.GetTotalPlayedTime(), false, false);

        ss << "Saddle up, " << player.GetName() << "! " << player.GetName() << " has hit level 40 in " << played << "! The pace begins to quicken...";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_40);
        discord_post("", REALM_FIRST_40, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if(level == 50 && !sObjectMgr.GetSavedVariable(REALM_FIRST_50, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_50, 1, true);
        std::stringstream ss;
        const std::string played = secsToTimeString(player.GetTotalPlayedTime(), false, false);

        ss << "The goal is in sight, as " << player.GetName() << " reaches level 50 in " << played << ". There's still plenty of time for an upset, however...";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_50);
        discord_post("", REALM_FIRST_50, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if(level == 60) {
        const auto it = class_events.find(player.getClass());

        if(!sObjectMgr.GetSavedVariable(REALM_FIRST_60, 0, &ignore)) {
            sObjectMgr.SetSavedVariable(REALM_FIRST_60, 1, true);
            std::stringstream ss, ss2;
            const std::string played = secsToTimeString(player.GetTotalPlayedTime(), false, false);

            
            const std::string& class_name = it != class_events.end()? it->second.second : "<missing class ID, what the f~, the server's going to crash>";
            ss << "Congratulations to " << player.GetName() << " on reaching realm first level 60 in " << played << "! An entire world beckons, but will this nascent " << class_name << " be brave enough to face the rest of this final journey?";
            sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
            ss2 << "It's a dangerous business, " << player.GetName() << ", going out your door. You step onto the road, and if you don't keep your feet, there's no knowing where you might be swept off to.";
            sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss2.str().c_str());
            discord_post(ss.str(), REALM_FIRST_60);
            discord_post("", REALM_FIRST_60, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
            sObjectMgr.SetSavedVariable(it->second.first, 1, true);
            return;
        }

        if(!sObjectMgr.GetSavedVariable(it->second.first, 0, &ignore)) {
            sObjectMgr.SetSavedVariable(it->second.first, 1, true);
            const std::string played = secsToTimeString(player.GetTotalPlayedTime(), false, false);
            std::stringstream ss;
            ss << "Congratulations to " << player.GetName() << " on realm first level 60 " << it->second.second << " in " << played << "! Best of luck with the journey ahead...";
            sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
            discord_post(ss.str(), it->second.first);
            discord_post("", it->second.first, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
        }
    }
}

void RealmEventAnnounce::level_up(std::uint32_t level, const Player& player) {
    switch(level) {
        case 20:
        case 30:
        case 40:
        case 50:
        case 60:
            announce_level_up(level, player);
            break;
        default:
            generic_level_up(level, player);
    }
}

void RealmEventAnnounce::quest_complete(std::uint32_t quest, const Player& player) {
    bool ignore = false;

    if (quest == 7631 && !sObjectMgr.GetSavedVariable(REALM_FIRST_DREADSTEED, 0, &ignore)) {
        std::stringstream ss;
        ss << player.GetName() << " conjures forth a dark steed, borne of fire, ash and brimstone. Fear he who rides the Dreadsteed.";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        sObjectMgr.SetSavedVariable(REALM_FIRST_DREADSTEED, 1, true);
        discord_post(ss.str(), REALM_FIRST_DREADSTEED);
        discord_post("", REALM_FIRST_DREADSTEED, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if (quest == 7647 && !sObjectMgr.GetSavedVariable(REALM_FIRST_CHARGER, 0, &ignore)) {
        std::stringstream ss;
        ss << "Having placed the scryer upon the bone-littered floor, " << player.GetName() << " drew the soul of a once noble"
        << " charger from the remains of Darkweaver. Arise, your service is needed once more.";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        sObjectMgr.SetSavedVariable(REALM_FIRST_CHARGER, 1, true);
        discord_post(ss.str(), REALM_FIRST_CHARGER);
        discord_post("", REALM_FIRST_CHARGER, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if (quest == 8743) {
        // bang a gong, Discord only, server already has an announcement
        std::stringstream ss;
        ss << player.GetName() << ", Champion of the Broze Dragonflight, has rung the Scarab Gong. The ancient gates of Ahn'Qiraj open, revealing the horrors of a forgotten war...";
        discord_post(ss.str(), REALM_FIRST_BANG_A_GONG);
        discord_post("", REALM_FIRST_BANG_A_GONG, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }
}

void RealmEventAnnounce::skill_acquired(std::uint32_t skill, const Player& player) {
    bool ignore = false;

    if(skill == 33391 && !sObjectMgr.GetSavedVariable(REALM_FIRST_150_RIDING, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_150_RIDING, 1, true);

        std::stringstream ss;
        ss << "Hey, big spender! " << player.GetName() << " has purchased the realm's first epic mount skill!";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_150_RIDING);
        discord_post("", REALM_FIRST_150_RIDING, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }
}

void RealmEventAnnounce::item_acquired(std::uint32_t item, const Player& player) {
    bool ignore = false;

    if (item == 19019 && !sObjectMgr.GetSavedVariable(REALM_FIRST_THUNDERFURY, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_THUNDERFURY, 1, true);

        std::stringstream ss;
        ss << player.GetName() << " has banished the elementals and reunited the bindings. Behold, Thunderfury, Blessed Blade of the Windseeker!";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_THUNDERFURY);
        discord_post("", REALM_FIRST_THUNDERFURY, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if (item == 13086) {
        std::stringstream ss;
        ss << player.GetName() << ", having earned the trust of Rivern Frostwind after performing a set of rather arduous and repetitive tasks, %player was bestowed with their very own Winterspring Frostsaber... for a modest price, of course. ";
        
        if(!sObjectMgr.GetSavedVariable(REALM_FIRST_THUNDERFURY, 0, &ignore)) {
            sObjectMgr.SetSavedVariable(REALM_FIRST_THUNDERFURY, 1, true);
            ss << "Realm first Winterspring Frostsaber!";
        }

        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_WS_FROSTSABER);
        discord_post("", REALM_FIRST_WS_FROSTSABER, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if (item == 13335) { // so rare that we'll announce them all (probably zero)
        std::stringstream ss;
        ss << "Extreme fortune has smiled upon " << player.GetName() << "...";
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_BARON_MOUNT);
        discord_post("", REALM_FIRST_BARON_MOUNT, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if (item == 12344 && !sObjectMgr.GetSavedVariable(REALM_FIRST_SEAL_OF_ASCENSION, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_SEAL_OF_ASCENSION, 1, true);

        std::stringstream ss;
        ss << player.GetName() << " has completed the Seal of Ascension. Upper Blackrock Spire awaits..."; 
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_SEAL_OF_ASCENSION);
        discord_post("", REALM_FIRST_SEAL_OF_ASCENSION, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }

    if (item == 18404 && !sObjectMgr.GetSavedVariable(REALM_FIRST_ONYXIA_ATTUNE, 0, &ignore)) {
        sObjectMgr.SetSavedVariable(REALM_FIRST_ONYXIA_ATTUNE, 1, true);

        std::stringstream ss;
        ss << player.GetName() << " has attuned to Onyxia's Lair. Lady Prestor awaits..."; 
        sWorld.SendWorldText(LANG_REALM_ACHIEVEMENT, ss.str().c_str());
        discord_post(ss.str(), REALM_FIRST_ONYXIA_ATTUNE);
        discord_post("", REALM_FIRST_ONYXIA_ATTUNE, "https://lightshope.org/api/progress/", player.GetEntry(), player.GetName());
    }
}