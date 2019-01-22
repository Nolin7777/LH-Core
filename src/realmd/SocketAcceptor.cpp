#include "SocketAcceptor.hpp"

#include "AuthSocket.h"
#include <list>
#include <map>
#include <string>

SocketAcceptor::SocketAcceptor(uint32 connectThrottle, uint32 failedLoginThrottle)
    : ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>(), _connectThrottlePeriod(connectThrottle),
    _failedLoginThrottlePeriod(failedLoginThrottle)
{
    _connections = std::list<AuthSocket*>();
    _throttle = std::map<std::string, time_t>();
}

int SocketAcceptor::accept_svc_handler(AuthSocket *handler)
{
    int res = 0;


    if ((res = ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>::accept_svc_handler(handler)) != -1)
    {
        _connections.push_back(handler);
        handler->SetAcceptor(this);
    }

    return res;
}

int SocketAcceptor::make_svc_handler(AuthSocket *&handlerPointer)
{
    int res = 0;

    if ((res = ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>::make_svc_handler(handlerPointer)) != -1)
    {
        
    }

    return res;
}

int SocketAcceptor::activate_svc_handler(AuthSocket* handler)
{
    int res = 0;
    if ((res = ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>::activate_svc_handler(handler)) != -1)
    {
        auto ip = handler->get_remote_address();
        if (ThrottleConnectionFrom(ip, _connectThrottlePeriod))
        {
            BASIC_LOG("Connection from ip '%s' has been throttled", ip.c_str());
            handler->close(NORMAL_CLOSE_OPERATION);
            res = -1;
        }
    }

    return res;
}

bool SocketAcceptor::ThrottleConnectionFrom(const std::string& ip, uint32 duration)
{
    time_t& existing = _throttle[ip];

    time_t now = time(nullptr);
    bool throttled = false;
    if (!!existing)
    {
        if (now < existing)
            throttled = true;
    }

    // Update the throttle period for any failed connections
    // while throttled
    existing = now + duration;

    return throttled;
}

void SocketAcceptor::UpdateThrottlePeriods()
{
    // Remove any throttles that are past the expiration such that we are
    // not consuming excessive amounts of memory.
    time_t now = time(nullptr);
    for (auto iter = _throttle.begin(); iter != _throttle.end();)
    {
        if (now > iter->second)
        {
            DEBUG_LOG("Removing throttle for ip '%s'", iter->first.c_str());
            iter = _throttle.erase(iter);
        }
        else
            ++iter;
    }
}

void SocketAcceptor::SetFailedLoginThrottle(const std::string& ip)
{
    BASIC_LOG("Setting failed login throttle for IP '%s'", ip.c_str());
    ThrottleConnectionFrom(ip, _failedLoginThrottlePeriod);
}