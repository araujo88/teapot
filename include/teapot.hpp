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
#include <list>
#include <optional>
#include <unordered_map>

#include "middleware.hpp"
#include "cors_middleware.hpp"
#include "sanitizer_middleware.hpp"
#include "security_middleware.hpp"
#include "utils.hpp"
#include "request.hpp"
#include "response.hpp"
#include "base_exceptions.hpp"
#include "controller.hpp"
#include "view.hpp"

#define BUFFER_SIZE 2048

class Teapot
{
private:
    typedef enum _logging
    {
        DEFAULT,
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
    std::map<std::string, std::string> html_responses;
    std::map<std::string, std::string> text_responses;
    std::map<std::string, std::string> json_responses;
    std::string static_files_dir;
    CORSMiddleware cors_middleware;
    SanitizerMiddleware sanitizer_middleware;
    SecurityMiddleware security_middleware;

    void checkSocket();
    void checkBind();
    void checkListen();
    void checkAccept(int *client_socket, struct sockaddr *client_address);
    void checkReceive(int *client_socket, char buffer[BUFFER_SIZE]);
    Request parseRequest(int *client_socket);
    std::unordered_map<std::string, std::string> parseFormData(const std::string &data);
    void requestHandler(int *client_socket);

public:
    Teapot();
    Teapot(unsigned int port);
    Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type, std::string static_files_dir);
    void runServer();
    void serveFile(std::string url, std::string file_path);
    void returnPlainText(std::string url, std::string text);
    void returnPlainText(std::string url, std::string text, unsigned int status_code);
    void returnHTML(std::string url, std::string html);
    void returnHTML(std::string url, std::string html, unsigned int status_code);
    void returnJSON(std::string url, std::string json);
    void returnJSON(std::string url, std::string json, unsigned int status_code);
    void addView(std::string url, Controller controller);
    void addMiddleware(CORSMiddleware cors_middleware);
    void addMiddleware(SanitizerMiddleware sanitizer_middleware);
    void addMiddleware(SecurityMiddleware security_middleware);

    ~Teapot();
};

#endif