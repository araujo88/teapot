#include "webserver.hpp"
#include "utils.hpp"

void *send_HTML(void *client_socket);

Webserver::Webserver()
{
    this->ip_address = "127.0.0.1";
    this->port = 8000;
    this->max_connections = 1;

    createServer();
}

Webserver::Webserver(std::string ip_address, unsigned int port, unsigned int max_connections)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;

    createServer();
}

void Webserver::createServer()
{
    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    checkSocket(server_socket);

    std::cout << "Socket created!" << std::endl;

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    std::cout << "Binding socket ..." << std::endl;

    checkBind(this->server_socket, &this->server_address);

    std::cout << "Binding done!" << std::endl;

    std::cout << "Waiting for incoming requests... (press Ctrl+C to quit)" << std::endl;
    while (true)
    {
        checkListen(this->server_socket, this->max_connections);

        this->client_socket = (int *)malloc(sizeof(int));
        this->client_address = NULL;

        checkAccept(this->server_socket, this->client_socket, (struct sockaddr *)this->client_address);

        pthread_t t;
        pthread_create(&t, NULL, send_HTML, (void *)this->client_socket);
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
    char server_message[BUFFER_SIZE];
    char *current_date;
    std::string content;
    time_t t;
    time(&t);

    current_date = ctime(&t);
    current_date[strcspn(current_date, "\n")] = 0;

    content = Utils::readFileToBuffer("static/index.html");
    sprintf(server_message, "HTTP/1.0 200 OK\nDate: %s\nContent-Type: text/html\nContent-Length: %ld\n\n%s", current_date, strlen(content.c_str()), content.c_str());
    send(*(int *)client_socket, &server_message, sizeof(server_message), 0);
    memset(server_message, 0, sizeof(server_message));
    close(*(int *)client_socket);
    free(client_socket);
    return NULL;
}