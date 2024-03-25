#include "../include/unix_socket.hpp"

using namespace tpt;

UnixSocket::UnixSocket()
{
    this->ip_address = "127.0.0.1";
    this->port = 8000;
    this->max_connections = 10;

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket < 0)
    {
        perror("Socket failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(EXIT_FAILURE);
    }

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());
}

UnixSocket::UnixSocket(unsigned int port)
{
    this->ip_address = "127.0.0.1";
    this->port = port;
    this->max_connections = 10;

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket < 0)
    {
        perror("Socket failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(EXIT_FAILURE);
    }

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());
}

UnixSocket::UnixSocket(std::string ip_address, unsigned int port)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = 10;

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket < 0)
    {
        perror("Socket failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(EXIT_FAILURE);
    }

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());
}

UnixSocket::UnixSocket(std::string ip_address, unsigned int port, unsigned int max_connections)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->server_socket < 0)
    {
        perror("Socket failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Socket created!" << std::endl;

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());
}

void UnixSocket::bindSocket()
{
    std::cout << "Binding socket ..." << std::endl;
    if ((bind(this->server_socket, (struct sockaddr *)&this->server_address, sizeof(this->server_address))) < 0)
    {
        perror("Bind failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Binding done!" << std::endl;
    std::cout << "Listening to connections ..." << std::endl;
}

void UnixSocket::listenToConnections()
{
    if ((listen(this->server_socket, this->max_connections)) < 0)
    {
        perror("Listen failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void UnixSocket::acceptConnection(int &client_socket, void *client_address)
{
    socklen_t client_addr_size = sizeof(client_address);
    client_socket = accept(this->server_socket, (struct sockaddr *)&client_address, &client_addr_size);
    if (client_socket < 0)
    {
        perror("Accept failed");
        std::cout << "Error code: " + std::to_string(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

ssize_t UnixSocket::receiveData(int client_socket, char *buffer, unsigned int buffer_size)
{
    ssize_t data = recv(client_socket, (void *)buffer, buffer_size, 0);
    if (data < 0)
    {
        perror("Receive error");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
    return data;
}

void UnixSocket::sendData(int client_socket, const void *buffer, unsigned int buffer_size, int flags)
{
    send(client_socket, buffer, buffer_size, flags);
}

void UnixSocket::closeSocket()
{
    std::cout << "Closing socket ..." << std::endl;
    if (shutdown(this->server_socket, SHUT_RDWR) == -1)
    {
        perror("An error occurred while shutting down the socket: ");
        std::cout << "Error code: " + errno << std::endl;
        exit(EXIT_FAILURE);
    }
    if (close(this->server_socket) == 0)
    {
        std::cout << "Socket closed!" << std::endl;
        exit(EXIT_SUCCESS);
    }
    else
    {
        perror("An error occurred while closing the socket: ");
        std::cout << "Error code: " + errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

void UnixSocket::closeSocket(int client_socket)
{
    close(client_socket);
}

UnixSocket::~UnixSocket() {}
