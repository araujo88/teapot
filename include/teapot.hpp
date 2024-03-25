#ifndef TEAPOT_H_
#define TEAPOT_H_

#include <iostream>
#include <csignal>
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
#include "context.hpp"
#include "base_exceptions.hpp"
#include "console_logger.hpp"

#ifdef __linux__
#include "unix_socket.hpp"
#endif

#ifdef _WIN32
#include "win_socket.hpp"
#endif

#define BUFFER_SIZE 8192

namespace tpt
{
    class Teapot
    {
    private:
        typedef enum
        {
            DEFAULT,
            NORMAL,
            VERBOSE
        } logging;

        logging logging_type;
        std::string ip_address;
        unsigned int port;
        unsigned int max_connections;
        std::map<std::string, std::string> routes;
        std::map<std::string, std::string> html_responses;
        std::map<std::string, std::string> json_responses;
        std::string static_files_dir;
        CORSMiddleware cors_middleware;
        SanitizerMiddleware sanitizer_middleware;
        SecurityMiddleware security_middleware;
        ConsoleLogger logger;
#ifdef __linux__
        tpt::UnixSocket socket;
#endif
#ifdef _WIN32
        tpt::WinSocket socket;
#endif

        std::optional<Request> parseRequest(int client_socket);
        std::string determineContentType(const std::string &uri);
        void mainEventLoop(SOCKET client_socket);

    public:
        Teapot();
        Teapot(unsigned int port);
        Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type, std::string static_files_dir);
        void run();
        void serveFile(std::string url, std::string file_path);
        void returnHTML(std::string url, std::string html);
        void returnHTML(std::string url, std::string html, unsigned int status_code);
        void returnJSON(std::string url, std::string json);
        void returnJSON(std::string url, std::string json, unsigned int status_code);
        void addMiddleware(CORSMiddleware cors_middleware);
        void addMiddleware(SanitizerMiddleware sanitizer_middleware);
        void addMiddleware(SecurityMiddleware security_middleware);
        ~Teapot();
    };
}

#endif
