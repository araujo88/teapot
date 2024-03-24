#ifdef __linux__
#ifndef UNIX_SOCKET_H_
#define UNIX_SOCKET_H_

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "socket.hpp"

namespace tpt
{
    class UnixSocket : public ISocket
    {
    private:
        int server_socket;
        struct sockaddr_in server_address;
        unsigned int port;
        unsigned int max_connections;
        std::string ip_address;

    public:
        UnixSocket();
        UnixSocket(unsigned int port);
        UnixSocket(std::string ip_address, unsigned int port);
        UnixSocket(std::string ip_address, unsigned int port, unsigned int max_connections);
        ~UnixSocket();
        virtual void bindSocket() override;
        virtual void listenToConnections() override;
        virtual void acceptConnection(int &client_socket, void *client_address) override;
        virtual ssize_t receiveData(int client_socket, char *buffer, unsigned int buffer_size) override;
        virtual void sendData(int client_socket, const void *buffer, unsigned int buffer_size, int flags) override;
        virtual void closeSocket() override;
        virtual void closeSocket(int client_socket) override;
    };
}

#endif
#endif
