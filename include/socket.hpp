#ifndef SOCKET_H_
#define SOCKET_H_

#include "address.hpp"

namespace tpt
{
    class ISocket
    {
    public:
        virtual void bindSocket() = 0;
        virtual void listenToConnections() = 0;
        // virtual void acceptConnection(std::unique_ptr<int> &client_socket, std::unique_ptr<IAddress> &client_address) = 0;
        virtual void receive(int client_socket, char *buffer, unsigned int buffer_size) = 0;
        virtual void closeSocket() = 0;
    };
}

#endif