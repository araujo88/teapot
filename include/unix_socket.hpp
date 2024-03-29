#ifdef __linux__
#ifndef UNIX_SOCKET_H_
#define UNIX_SOCKET_H_

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include "socket.hpp"
#include "utils.hpp"
#include "base_exceptions.hpp"
#include "console_logger.hpp"

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
        std::string client_ip;
        std::vector<std::string> ip_blacklist;
        std::vector<SOCKET> client_sockets;
        ConsoleLogger logger;

    public:
        UnixSocket();
        UnixSocket(ConsoleLogger logger);
        UnixSocket(ConsoleLogger logger, unsigned int port);
        UnixSocket(ConsoleLogger logger, std::string ip_address, unsigned int port);
        UnixSocket(ConsoleLogger logger, std::string ip_address, unsigned int port, unsigned int max_connections);
        ~UnixSocket();
        std::string getClientIp();
        virtual void bindSocket() override;
        virtual void listenToConnections() override;
        virtual bool acceptConnection(SOCKET &client_socket, void *client_address) override;
        virtual ssize_t receiveData(SOCKET client_socket, char *buffer, unsigned int buffer_size) override;
        virtual void sendData(SOCKET client_socket, const void *buffer, unsigned int buffer_size, int flags) override;
        virtual void closeSocket() override;
        virtual void closeSocket(SOCKET client_socket) override;
        virtual void setSocketTimeout(SOCKET sock, int timeoutSec) override;
    };
}

#endif
#endif
