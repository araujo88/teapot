#ifndef TEAPOT_H_
#define TEAPOT_H_

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
#include "middleware.hpp"
#include "utils.hpp"
#include "request.hpp"
#include "response.hpp"
#include "base_exceptions.hpp"

#define BUFFER_SIZE 2048

class Teapot
{
private:
    typedef enum _logging
    {
        DISABLED,
        NORMAL,
        VERBOSE
    } logging;

    logging logging_type;
    std::string ip_address;
    unsigned int port;
    unsigned int max_connections;
    int server_socket;
    struct sockaddr_in server_address;
    std::map<std::string, std::string> routes;
    std::string static_files_dir;

    void checkSocket();
    void checkBind();
    void checkListen();
    void checkAccept(int *client_socket, struct sockaddr *client_address);
    void checkReceive(int *client_socket, char buffer[BUFFER_SIZE]);
    Request parseRequest(int *client_socket);
    void requestHandler(int *client_socket);

public:
    Teapot();
    Teapot(unsigned int port);
    Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type, std::string static_files_dir);
    void runServer();
    void addRoute(std::string url, std::string file_path);
    void addMiddleware(Middleware middleware);
    ~Teapot();
};

#endif