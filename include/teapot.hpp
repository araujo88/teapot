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
#include <memory>

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
#include "unix_socket.hpp"
#include "json_database.hpp"

#define BUFFER_SIZE 2048

namespace tpt
{
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
        std::map<std::string, std::string> json_responses;
        std::string static_files_dir;
        CORSMiddleware cors_middleware;
        SanitizerMiddleware sanitizer_middleware;
        SecurityMiddleware security_middleware;
        std::list<Controller> controllers;
        tpt::UnixSocket socket;

        Request parseRequest(int client_socket);
        std::unordered_map<std::string, std::string> parseFormData(const std::string &data);
        void mainEventLoop(int client_socket);

    public:
        Teapot();
        Teapot(unsigned int port);
        Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type, std::string static_files_dir);
        void runServer();
        void serveFile(std::string url, std::string file_path);
        void returnHTML(std::string url, std::string html);
        void returnHTML(std::string url, std::string html, unsigned int status_code);
        void returnJSON(std::string url, std::string json);
        void returnJSON(std::string url, std::string json, unsigned int status_code);
        void addMiddleware(CORSMiddleware cors_middleware);
        void addMiddleware(SanitizerMiddleware sanitizer_middleware);
        void addMiddleware(SecurityMiddleware security_middleware);
        void addController(Controller controller);
        Model extractAndStore(const std::string &requestBody);
        ~Teapot();
    };
}

#endif
