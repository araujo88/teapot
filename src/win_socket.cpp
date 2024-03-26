#ifdef _WIN32
#include "../include/win_socket.hpp"

using namespace tpt;

WinSocket::WinSocket()
{
    this->ip_address = "127.0.0.1";
    this->port = 8000;
    this->max_connections = 10;
    this->logger = ConsoleLogger();

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::printf("Failed. Error Code : %d", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket == INVALID_SOCKET)
    {
        std::printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    Utils::fillIPBlacklist(this->ip_blacklist);
}

WinSocket::WinSocket(ConsoleLogger logger)
{
    this->ip_address = "127.0.0.1";
    this->port = 8000;
    this->max_connections = 10;
    this->logger = logger;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::printf("Failed. Error Code : %d", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket == INVALID_SOCKET)
    {
        std::printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    Utils::fillIPBlacklist(this->ip_blacklist);
}

WinSocket::WinSocket(ConsoleLogger logger, unsigned int port)
{
    this->ip_address = "127.0.0.1";
    this->port = port;
    this->max_connections = 10;
    this->logger = logger;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::printf("Failed. Error Code : %d", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket == INVALID_SOCKET)
    {
        std::printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    Utils::fillIPBlacklist(this->ip_blacklist);
}

WinSocket::WinSocket(ConsoleLogger logger, std::string ip_address, unsigned int port)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = 10;
    this->logger = logger;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::printf("Failed. Error Code : %d", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket == INVALID_SOCKET)
    {
        std::printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    Utils::fillIPBlacklist(this->ip_blacklist);
}

WinSocket::WinSocket(ConsoleLogger logger, std::string ip_address, unsigned int port, unsigned int max_connections)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = 10;
    this->logger = logger;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        std::printf("Failed. Error Code : %d", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket == INVALID_SOCKET)
    {
        std::printf("Could not create socket: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
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
        std::printf("Binding failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    this->setSocketTimeout(this->server_socket, 5);
    std::cout << "Binding done!" << std::endl;
    std::cout << "Listening to connections ..." << std::endl;
}

void WinSocket::listenToConnections()
{
    if ((listen(this->server_socket, this->max_connections)) == SOCKET_ERROR)
    {
        std::printf("Listening failed: %d\n", WSAGetLastError());
        closesocket(this->server_socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void WinSocket::acceptConnection(SOCKET &client_socket, void *client_address)
{
    int client_addr_size = sizeof(sockaddr_in);
    client_socket = accept(this->server_socket, static_cast<sockaddr *>(client_address), &client_addr_size);
    if (client_socket == INVALID_SOCKET)
    {
        std::printf("Error accepting connections: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
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
        std::printf("Receive error\n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return data;
}

void WinSocket::sendData(SOCKET client_socket, const void *buffer, unsigned int buffer_size, int flags)
{
    send(client_socket, (char *)buffer, buffer_size, flags);
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

#endif
