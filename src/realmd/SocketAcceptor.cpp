#include "SocketAcceptor.hpp"

#include "AuthSocket.h"
#include <list>

SocketAcceptor::SocketAcceptor()
    : ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>()
{
    _connections = std::list<AuthSocket*>();
}

int SocketAcceptor::accept_svc_handler(AuthSocket *handler)
{
    int res = 0;
    if ((res = ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>::accept_svc_handler(handler)) != -1)
    {
        _connections.push_back(handler);
    }

    return res;
}

int SocketAcceptor::make_svc_handler(AuthSocket *&handlerPointer)
{
    int res = 0;
    if ((res = ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor>::make_svc_handler(handlerPointer)) != -1)
    {
        handlerPointer->SetAcceptor(this);
    }

    return res;
}