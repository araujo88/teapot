#ifdef _WIN32
#ifndef WIN_SOCKET_H_
#define WIN_SOCKET_H_

#include <iostream>
#include <string>
#include <sys/types.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")
#include "socket.hpp"
#include "utils.hpp"
#include "base_exceptions.hpp"
#include "console_logger.hpp"

namespace tpt
{
    class WinSocket : public ISocket
    {
    private:
        WSADATA wsa;
        SOCKET server_socket;
        struct sockaddr_in server_address;
        unsigned int port;
        unsigned int max_connections;
        std::string ip_address;
        std::vector<std::string> ip_blacklist;
        ConsoleLogger logger;

    public:
        WinSocket();
        WinSocket(ConsoleLogger logger);
        WinSocket(ConsoleLogger logger, unsigned int port);
        WinSocket(ConsoleLogger logger, std::string ip_address, unsigned int port);
        WinSocket(ConsoleLogger logger, std::string ip_address, unsigned int port, unsigned int max_connections);
        ~WinSocket();
        std::string getClientIp();
        virtual void bindSocket() override;
        virtual void listenToConnections() override;
        virtual void acceptConnection(SOCKET &client_socket, void *client_address) override;
        virtual ssize_t receiveData(SOCKET client_socket, char *buffer, unsigned int buffer_size) override;
        virtual void sendData(SOCKET client_socket, const void *buffer, unsigned int buffer_size, int flags) override;
        virtual void closeSocket() override;
        virtual void closeSocket(SOCKET client_socket) override;
        virtual void setSocketTimeout(SOCKET sock, int timeoutSec) override;
    };
}

#endif
#endif
