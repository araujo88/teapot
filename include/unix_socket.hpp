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
        void acceptConnection(int &client_socket, struct sockaddr_in &client_address);
        virtual void receive(int client_socket, char *buffer, unsigned int buffer_size) override;
        virtual void closeSocket() override;
    };
}

#endif