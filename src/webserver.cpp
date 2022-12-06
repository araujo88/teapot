#include "webserver.hpp"
#include "utils.hpp"
#include <sstream>

void *send_HTML(void *client_socket);

Webserver::Webserver()
{
    this->ip_address = "127.0.0.1";
    this->port = 8000;
    this->max_connections = 1;
}

Webserver::Webserver(std::string ip_address, unsigned int port, unsigned int max_connections)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;
}

void Webserver::runServer()
{

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    checkSocket(this->server_socket);

    std::cout << "Socket created!" << std::endl;

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    std::cout << "Binding socket ..." << std::endl;

    checkBind(this->server_socket, &this->server_address);

    std::cout << "Binding done!" << std::endl;

    std::cout << "Waiting for incoming requests... (enter 'q' to quit)" << std::endl;

    while (true)
    {
        char chr;
        checkListen(this->server_socket, this->max_connections);

        this->client_socket = new int;
        this->client_address = NULL;

        checkAccept(this->server_socket, this->client_socket, (struct sockaddr *)this->client_address);

        pthread_t t;
        pthread_create(&t, NULL, send_HTML, (void *)this->client_socket);
        if (std::cin >> chr && chr == 'q')
        {
            break;
        }
    }
}

void Webserver::checkSocket(int server_socket)
{
    if (server_socket < 0)
    {
        perror("Socket failed: ");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Webserver::checkBind(int server_socket, struct sockaddr_in *server_address)
{
    if ((bind(server_socket, (struct sockaddr *)server_address, sizeof(*server_address))) < 0)
    {
        perror("Bind failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Webserver::checkListen(int server_socket, int num_connections)
{
    if ((listen(server_socket, num_connections)) < 0)
    {
        perror("Listen failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Webserver::checkAccept(int server_socket, int *client_socket, struct sockaddr *client_address)
{
    if ((*client_socket = accept(server_socket, (struct sockaddr *)client_address, (socklen_t *)sizeof(client_address))) < 0)
    {
        perror("Accept failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

Webserver::~Webserver()
{
    std::cout << "Closing socket ..." << std::endl;
    if (close(this->server_socket) == 0)
    {
        std::cout << "Socket closed!" << std::endl;
        exit(0);
    }
    else
    {
        perror("An error occurred while closing the socket: ");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void *send_HTML(void *client_socket)
{
    std::ostringstream response;
    std::string current_date;
    std::string content;
    time_t t;
    time(&t);

    current_date = ctime(&t);

    content = Utils::readFileToBuffer("static/index.html");
    response << "HTTP/1.1 200 OK\nDate: " << current_date << "Content-Type: text/html\nContent-Length: " << content.length() << "\n\n"
             << content;
    send(*(int *)client_socket, response.str().c_str(), response.str().length(), 0);
    close(*(int *)client_socket);
    delete (int *)client_socket;
    return NULL;
}