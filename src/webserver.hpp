#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <list>

#define BUFFER_SIZE 1024

class Webserver
{
private:
    std::string ip_address;
    unsigned int port;
    unsigned int max_connections;
    int server_socket;
    int *client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in *client_address;
    std::list<std::string> routes;

    void createServer();
    void checkSocket(int server_socket);
    void checkBind(int server_socket, struct sockaddr_in *server_address);
    void checkListen(int server_socket, int num_connections);
    void checkAccept(int server_socket, int *client_socket, struct sockaddr *client_address);
    void signalHandler(int sig);

public:
    Webserver();
    Webserver(std::string ip_address, unsigned int port, unsigned int max_connections);
    ~Webserver();
};