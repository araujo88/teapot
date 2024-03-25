#ifdef _WIN32
#include "../include/win_socket.hpp"

using namespace tpt;

WinSocket::WinSocket()
{
    this->ip_address = "127.0.0.1";
    this->port = 8000;
    this->max_connections = 10;

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
}

WinSocket::WinSocket(unsigned int port)
{
    this->ip_address = "127.0.0.1";
    this->port = port;
    this->max_connections = 10;

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
}

WinSocket::WinSocket(std::string ip_address, unsigned int port)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = 10;

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
}

WinSocket::WinSocket(std::string ip_address, unsigned int port, unsigned int max_connections)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = 10;

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
    client_socket = accept(this->server_socket, static_cast<sockaddr*>(client_address), &client_addr_size);
    if (client_socket == INVALID_SOCKET) {
        std::printf("Error accepting connections: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

ssize_t WinSocket::receiveData(SOCKET client_socket, char *buffer, unsigned int buffer_size)
{
    ssize_t data = recv(client_socket, buffer, buffer_size, 0);
    if (data < 0)
    {
        perror("Receive error");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
    return data;
}

void WinSocket::sendData(SOCKET client_socket, const void *buffer, unsigned int buffer_size, int flags)
{
    send(client_socket, (char*)buffer, buffer_size, flags);
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

WinSocket::~WinSocket() {}

#endif
