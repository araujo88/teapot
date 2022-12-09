#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <arpa/inet.h>
#include <fstream>
#include <iterator>
#include <map>
#include <sstream>
#include <thread>
#include <algorithm>

class Teapot
{
private:
    std::string ip_address;
    unsigned int port;
    unsigned int max_connections;
    int server_socket;
    struct sockaddr_in server_address;
    std::map<std::string, std::string> routes;

    void checkSocket();
    void checkBind();
    void checkListen();
    void checkAccept(int *client_socket, struct sockaddr *client_address);

public:
    Teapot();
    Teapot(std::string ip_address, unsigned int port, unsigned int max_connections);
    void runServer();
    void addRoute(std::string url, std::string file_path);
    ~Teapot();
};