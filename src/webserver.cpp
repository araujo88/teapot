#include "webserver.hpp"
#include "utils.hpp"
#include "request.hpp"
#include "response.hpp"

void serveFile(int *client_socket)
{
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

void Webserver::checkSocket()
{
    if (this->server_socket < 0)
    {
        perror("Socket failed: ");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Webserver::checkBind()
{
    if ((bind(this->server_socket, (struct sockaddr *)&this->server_address, sizeof(this->server_address))) < 0)
    {
        perror("Bind failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Webserver::checkListen()
{
    if ((listen(this->server_socket, this->max_connections)) < 0)
    {
        perror("Listen failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Webserver::checkAccept(int *client_socket, struct sockaddr *client_address)
{
    if ((*client_socket = accept(this->server_socket, (struct sockaddr *)client_address, (socklen_t *)sizeof(client_address))) < 0)
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

void Webserver::addRoute(std::string url, std::string file_path)
{
    this->routes.insert(std::pair<std::string, std::string>(url, file_path));
}