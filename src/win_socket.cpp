#ifdef _WIN32
#include "../include/win_socket.hpp"

using namespace tpt;

WinSocket::WinSocket() : WinSocket(ConsoleLogger()) {}

WinSocket::WinSocket(ConsoleLogger logger) : WinSocket(logger, "127.0.0.1", 8000, 10) {}

WinSocket::WinSocket(ConsoleLogger logger, unsigned int port) : WinSocket(logger, "127.0.0.1", port, 10) {}

WinSocket::WinSocket(ConsoleLogger logger, std::string ip_address, unsigned int port) : WinSocket(logger, ip_address, port, 10) {}

WinSocket::WinSocket(ConsoleLogger logger, std::string ip_address, unsigned int port, unsigned int max_connections)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;
    this->logger = logger;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        throw SocketCreationException("WSAStartup error", WSAGetLastError());
    }

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket == INVALID_SOCKET)
    {
        throw SocketCreationException("could not create socket", WSAGetLastError());
    }

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    Utils::fillIPBlacklist(this->ip_blacklist);
}

void WinSocket::bindSocket()
{
    std::cout << "Binding socket ..." << std::endl;
    if ((bind(this->server_socket, (struct sockaddr *)&this->server_address, sizeof(server_address))) == SOCKET_ERROR)
    {
        throw SocketBindingException("binding failed", WSAGetLastError());
    }
    std::cout << "Binding done!" << std::endl;
    std::cout << "Listening to connections ..." << std::endl;
}

void WinSocket::listenToConnections()
{
    if ((listen(this->server_socket, this->max_connections)) == SOCKET_ERROR)
    {
        throw SocketListenException("listening failed", WSAGetLastError());
    }
}

void WinSocket::acceptConnection(SOCKET &client_socket, void *client_address)
{
    struct sockaddr_storage client_addr_storage;
    int client_addr_size = sizeof(client_addr_storage);

    //this->setSocketTimeout(this->server_socket, 5);
    client_socket = accept(this->server_socket, (sockaddr *)&client_addr_storage, &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        throw SocketAcceptException("error accepting connections", WSAGetLastError());
    }

    // Assuming client_address is meant to store the result
    if (client_address != nullptr)
    {
        std::memcpy(client_address, &client_addr_storage, client_addr_size);
    }

    char ip_str[INET6_ADDRSTRLEN] = {0}; // Large enough for both IPv4 and IPv6
    if (client_addr_storage.ss_family == AF_INET)
    {
        // IPv4
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&client_addr_storage;
        inet_ntop(AF_INET, &addr_in->sin_addr, ip_str, INET_ADDRSTRLEN);
    }
    else if (client_addr_storage.ss_family == AF_INET6)
    {
        // IPv6
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&client_addr_storage;
        inet_ntop(AF_INET6, &addr_in6->sin6_addr, ip_str, INET6_ADDRSTRLEN);
    }

    this->client_ip = std::string(ip_str);

    for (auto it : this->ip_blacklist)
    {
        if (this->client_ip == it)
        {
            throw IPBlackListedException();
        }
    }
}

ssize_t WinSocket::receiveData(SOCKET client_socket, char *buffer, unsigned int buffer_size)
{
    ssize_t data = recv(client_socket, buffer, buffer_size, 0);
    if (data < 0)
    {
        throw SocketReceiveException("recv error", WSAGetLastError());
    }
    return data;
}

void WinSocket::sendData(SOCKET client_socket, const void *buffer, unsigned int buffer_size, int flags)
{
    if (send(client_socket, (char *)buffer, buffer_size, flags) == -1)
    {
        throw SocketSendException();
    }
}

void WinSocket::closeSocket()
{
    std::cout << "Closing socket ..." << std::endl;
    closesocket(this->server_socket);
    WSACleanup();
    std::cout << "Socket closed!" << std::endl;
}

void WinSocket::closeSocket(SOCKET client_socket)
{
    closesocket(client_socket);
}

void WinSocket::setSocketTimeout(SOCKET sock, int timeoutSec)
{
    // Initialize the timeout value
    DWORD timeout = timeoutSec * 1000; // Convert to milliseconds, as required by Windows

    // Set the receive timeout option
    int result = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
    if (result == SOCKET_ERROR)
    {
        std::printf("Error setting timeout: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

WinSocket::~WinSocket() {}

std::string WinSocket::getClientIp()
{
    return this->client_ip;
}

#endif
