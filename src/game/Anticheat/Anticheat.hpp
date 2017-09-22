/**
* Contains interface and calls that have to be implemented by the anticheat lib.
*/

#ifndef __ANTICHEAT_HPP_
#define __ANTICHEAT_HPP_

#include "Auth/BigNumber.h"

#include <memory>
#include <string>

enum CheatAction
{
    CHEAT_ACTION_NONE           = 0x00,
    CHEAT_ACTION_LOG            = 0x01,
    CHEAT_ACTION_REPORT_GMS     = 0x02,
    CHEAT_ACTION_KICK           = 0x04,
    CHEAT_ACTION_BAN_ACCOUNT    = 0x08,
    CHEAT_ACTION_BAN_IP         = 0x10,
    CHEAT_ACTION_TELEPORT_BACK  = 0x20,
    CHEAT_ACTION_MUTE_PUB_CHANS = 0x40, // Mutes the account from public channels
    CHEAT_MAX_ACTIONS,
};

enum NormalizeFlags
{
    NF_CUT_COLOR                = 0x001,
    NF_REPLACE_WORDS            = 0x002,
    NF_CUT_SPACE                = 0x004,
    NF_CUT_CTRL                 = 0x008,
    NF_CUT_PUNCT                = 0x010,
    NF_CUT_NUMBERS              = 0x020,
    NF_REPLACE_UNICODE          = 0x040,
    NF_REMOVE_REPEATS           = 0x080,
    NF_REMOVE_NON_LATIN         = 0x100
};

class WorldSession;
class Player;
struct AreaEntry;

// interface to anticheat session (one for each world session)
class SessionAnticheatInterface
{
    public:
        virtual void Update(uint32 diff) = 0;

        virtual void CompleteCinematic() = 0;

        virtual bool IsMuted(bool checkChatType, uint32 chatType) const = 0;

        virtual void NewPlayer() = 0;
        virtual void LeaveWorld() = 0;

        virtual void SendPlayerInfo(WorldSession *) = 0;

        // miscellaneous action
        // TODO: Let anticheat decide on action based on its configuration
        virtual void MiscAction(const char *detector, const char *comment, uint32 action) = 0;

        // movement cheats
        virtual bool Movement(MovementInfo &mi, const WorldPacket &packet) = 0;
        virtual bool InterpolateMovement(MovementInfo const& mi, uint32 diffMs, float &x, float &y, float &z, float &o) = 0;
        virtual bool SpeedChangeAck(MovementInfo &mi, const WorldPacket &packet, float newSpeed) = 0;
        virtual void Unreachable(Unit *attacker) = 0;
        virtual bool IsInKnockBack() const = 0;
        virtual void KnockBack(float speedxy, float speedz, float cos, float sin) = 0;
        virtual void OnExplore(const AreaEntry *p) = 0;
        virtual void OrderSent(const WorldPacket &packet) = 0;

        // warden
        virtual void WardenPacket(WorldPacket &packet) = 0;

        // recording and acting on results
        virtual void Action(uint8 type, const char *format, ...) = 0;
};

// interface to anticheat system
class AnticheatLibInterface
{
    public:
        // this function needs to support executing at any time
        virtual void Initialize() = 0;

        // create anticheat session for a new world session
        virtual std::unique_ptr<SessionAnticheatInterface> NewSession(WorldSession *session, const BigNumber &K) = 0;

        // anti spam
        virtual bool ValidateGuildName(const std::string &name) = 0;
        virtual std::string NormalizeMessage(const std::string &message, uint32 mask) = 0;
        virtual void AddMessage(const std::string &msg, uint32 type, PlayerPointer from, PlayerPointer to) = 0;
        virtual void MuteAccount(uint32 id) = 0;
        virtual void UnmuteAccount(uint32 id) = 0;

        // GM .anticheat command handler
        virtual bool ChatCommand(Player *target, const std::string &args) = 0;
};

AnticheatLibInterface* GetAnticheatLib();
#define sAnticheatLib (GetAnticheatLib())

class NullSessionAnticheat : public SessionAnticheatInterface
{
    private:
        bool m_inKnockBack;

    public:
        NullSessionAnticheat() : m_inKnockBack(false) {}

        virtual void Update(uint32) {}

        virtual void CompleteCinematic() {}

        virtual bool IsMuted(bool, uint32) const { return false; }

        virtual void NewPlayer() {} 
        virtual void LeaveWorld() {};

        virtual void SendPlayerInfo(WorldSession *) {}

        // miscellaneous action
        virtual void MiscAction(const char *detector, const char *comment, uint32 action) {};

        // movement cheats
        virtual bool Movement(MovementInfo &, const WorldPacket &packet)
        {
            if (packet.GetOpcode() == MSG_MOVE_FALL_LAND)
                m_inKnockBack = false;

            return true;
        }
        virtual bool InterpolateMovement(MovementInfo const& , uint32, float &, float &, float &, float &) { return false; }
        virtual bool SpeedChangeAck(MovementInfo &, const WorldPacket &, float) { return true; }
        virtual void Unreachable(Unit *) {}
        virtual bool IsInKnockBack() const { return m_inKnockBack; }
        virtual void KnockBack(float speedxy, float speedz, float cos, float sin) { m_inKnockBack = true; }
        virtual void OnExplore(const AreaEntry *) {}
        virtual void OrderSent(const WorldPacket &) {}

        // warden
        virtual void WardenPacket(WorldPacket &) {}

        // recording and acting on results
        virtual void Action(uint8 type, const char *format, ...) {}
};

#ifdef USE_ANTICHEAT
#include "module/libanticheat.hpp"
#else
class NullAnticheatLib sealed : public AnticheatLibInterface
{
    public:
        virtual void Initialize() {}

        virtual std::unique_ptr<SessionAnticheatInterface> NewSession(WorldSession *, const BigNumber &)
        {
            return std::make_unique<NullSessionAnticheat>();
        }

        // anti spam
        virtual bool ValidateGuildName(const std::string &) { return false; }
        virtual std::string NormalizeMessage(const std::string &message, uint32) { return message; }
        virtual void AddMessage(const std::string &, uint32, PlayerPointer, PlayerPointer) {}
        virtual void MuteAccount(uint32) {}
        virtual void UnmuteAccount(uint32) {}

        virtual bool ChatCommand(Player *, const std::string &) { return false; }
};
#endif

#endif /*!__ANTICHEAT_HPP_*/