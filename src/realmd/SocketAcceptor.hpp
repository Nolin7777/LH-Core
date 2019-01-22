#ifndef _SOCKET_ACCEPTOR_INCLUDED
#define _SOCKET_ACCEPTOR_INCLUDED

#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <list>
#include <map>

#include "AuthSocket.h"

class SocketAcceptor : public ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>
{
public:
    SocketAcceptor(uint32 connectThrottle, uint32 failedLoginThrottle);
    ~SocketAcceptor() {}

    std::list<AuthSocket*> const& GetConnections() { return _connections; }

    void RemoveConnectionHandler(AuthSocket* connection) { _connections.remove(connection); }
    void SetFailedLoginThrottle(const std::string& ip);
    void UpdateThrottlePeriods();
protected:
    int accept_svc_handler(AuthSocket *handler) override;
    int make_svc_handler(AuthSocket *&handlerPointer) override;
    int activate_svc_handler(AuthSocket *handler) override;

    bool ThrottleConnectionFrom(const std::string& ip, uint32 duration);
private:
    std::list<AuthSocket*> _connections;
    std::map<std::string, time_t> _throttle;
    uint32 _connectThrottlePeriod;
    uint32 _failedLoginThrottlePeriod;
};
#endif