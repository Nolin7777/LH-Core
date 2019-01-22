#ifndef _SOCKET_ACCEPTOR_INCLUDED
#define _SOCKET_ACCEPTOR_INCLUDED

#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <list>

#include "AuthSocket.h"

//class AuthSocket;

class SocketAcceptor : public ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>
{
public:
    SocketAcceptor();
    ~SocketAcceptor() {}

    std::list<AuthSocket*> const& GetConnections() { return _connections; }

    void RemoveConnectionHandler(AuthSocket* connection) { _connections.remove(connection); }
protected:
    int accept_svc_handler(AuthSocket *handler) override;
    int make_svc_handler(AuthSocket *&handlerPointer) override;

private:
    std::list<AuthSocket*> _connections;
};
#endif