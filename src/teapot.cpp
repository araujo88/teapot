#include "../include/teapot.hpp"
#include "../include/utils.hpp"
#include "../include/request.hpp"
#include "../include/response.hpp"

void parseRequest(int *client_socket)
{
    char buffer[2048];
    std::string raw_request;

    if ((recv(*client_socket, (void *)&buffer, sizeof(buffer), 0)) < 0)
    {
        perror("Receive error:");
        printf("Error code: %d\n", errno);
        exit(1);
    }

    raw_request = buffer;

    std::cout << raw_request << std::endl;

    Request request(raw_request);
}

void serveFile(int *client_socket)
{
    parseRequest(client_socket);

    std::string url;
    std::ostringstream response;
    std::string date;
    std::string content;
    time_t t;
    time(&t);

    url = "/";

    date = ctime(&t);
    date.erase(remove(date.begin(), date.end(), '\n'), date.end());

    content = Utils::readFileToBuffer("static/index.html");

    std::cout << "[" + date + "]" + " GET " + url + " HTTP/1.1 200 OK" << std::endl;

    response << "HTTP/1.1 200 OK\nDate: " << date << "\nContent-Type: text/html\nContent-Length: " << content.length() << "\n\n"
             << content;
    send(*client_socket, response.str().c_str(), response.str().length(), 0);
    close(*client_socket);
    delete client_socket;
}

Teapot::Teapot()
{
    this->ip_address = "127.0.0.1";
    this->port = 8000;
    this->max_connections = 1;
    this->logging_type = NORMAL;
}

Teapot::Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;
    this->logging_type = logging_type;
}

void Teapot::runServer()
{

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    checkSocket();

    std::cout << "Socket created!" << std::endl;

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    std::cout << "Binding socket ..." << std::endl;

    checkBind();

    std::cout << "Binding done!" << std::endl;

    std::cout << "Waiting for incoming requests..." << std::endl;

    while (true)
    {
        checkListen();

        int *client_socket = new int;
        struct sockaddr_in *client_address = NULL;

        checkAccept(client_socket, (struct sockaddr *)client_address);

        std::thread th(serveFile, client_socket);
        th.join();
    }
}

void Teapot::checkSocket()
{
    if (this->server_socket < 0)
    {
        perror("Socket failed: ");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Teapot::checkBind()
{
    if ((bind(this->server_socket, (struct sockaddr *)&this->server_address, sizeof(this->server_address))) < 0)
    {
        perror("Bind failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Teapot::checkListen()
{
    if ((listen(this->server_socket, this->max_connections)) < 0)
    {
        perror("Listen failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Teapot::checkAccept(int *client_socket, struct sockaddr *client_address)
{
    if ((*client_socket = accept(this->server_socket, (struct sockaddr *)client_address, (socklen_t *)sizeof(client_address))) < 0)
    {
        perror("Accept failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

Teapot::~Teapot()
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

void Teapot::addRoute(std::string url, std::string file_path)
{
    this->routes.insert(std::pair<std::string, std::string>(url, file_path));
}