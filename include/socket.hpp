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
        virtual void acceptConnection(int &client_socket, void *client_address) = 0;
        virtual void receiveData(int client_socket, char *buffer, unsigned int buffer_size) = 0;
        virtual void sendData(int client_socket, const void *buffer, unsigned int buffer_size, int flags) = 0;
        virtual void closeSocket() = 0;
        virtual void closeSocket(int client_socket) = 0;
    };
}

#endif