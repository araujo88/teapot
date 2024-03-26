#include "../include/teapot.hpp"
#include "../include/unix_socket.hpp"

using namespace tpt;

volatile std::sig_atomic_t signal_received = 0;

#ifdef __linux__
extern "C" void signalHandler(int signal)
{
    signal_received = signal;
}
#endif

#ifdef _WIN32
BOOL WINAPI consoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT)
    {
        signal_received = 1;
        return TRUE;
    }
    return FALSE;
}
#endif

std::optional<Request> Teapot::parseRequest(int client_socket)
{
    char buffer[BUFFER_SIZE] = {0};
    ssize_t total_bytes_received = 0;
    ssize_t bytes_received;
    bool request_end_found = false;

    // Use a loop to handle partial reads
    while ((bytes_received = this->socket.receiveData(client_socket, buffer + total_bytes_received, BUFFER_SIZE - total_bytes_received - 1)) > 0)
    {
        total_bytes_received += bytes_received;

        // Check if the end of the request (\r\n\r\n) has been reached
        if (std::string_view(buffer, total_bytes_received).find("\r\n\r\n") != std::string_view::npos)
        {
            request_end_found = true;
            break;
        }

        // Ensure buffer isn't overflowed; leave room for null terminator
        if (total_bytes_received >= BUFFER_SIZE - 1)
            break;
    }

    // If no data received or request end not found, return nullopt
    if (total_bytes_received <= 0 || !request_end_found)
    {
        return std::nullopt;
    }

    // Null-terminate the received data
    buffer[total_bytes_received] = '\0';

    // Now, buffer contains the raw request as a C-string. Use std::string_view for further processing to avoid copies
    std::string_view request_view(buffer, total_bytes_received);
    // Example: Extract the request line or headers using std::string_view operations...

    return Request(std::string(request_view)); // Convert to std::string only when necessary, for example, to construct a Request object
}

std::string Teapot::determineContentType(const std::string &uri)
{
    // This function returns the content type based on the file extension
    // It's a simplified version; you might want to refine this based on your requirements
    if (uri.ends_with(".css"))
        return "text/css";
    if (uri.ends_with(".ico"))
        return "image/x-icon";
    if (uri.ends_with(".gif"))
        return "image/gif";
    if (uri.ends_with(".jpg") || uri.ends_with(".jpeg"))
        return "image/jpeg";
    if (uri.ends_with(".png"))
        return "image/png";
    if (uri.ends_with(".js"))
        return "text/javascript";
    if (uri.ends_with(".html"))
        return "text/html";
    // Default content type for unknown types
    return "text/plain";
}

void Teapot::requestHandler(SOCKET client_socket)
{
    auto context = std::make_unique<Context>(nullptr, nullptr);
    auto request = parseRequest(client_socket);
    std::string body;
    std::string content_type;
    std::string method;
    std::string uri;
    unsigned int status_code = 500; // Default to internal server error in case of early failure

    try
    {
        if (request)
        {
            context->request = &(*request);
            this->sanitizer_middleware.handle(context.get());
            uri = request->getUri();
            if (uri == "/")
                uri = "/index.html"; // Normalize root access to a specific file, e.g., index.html
            method = request->getMethod();
            content_type = determineContentType(uri); // Determine content type early based on URI

            if (method == "GET")
            {
                // Check for predefined routes or responses before attempting to read a file
                auto routeIt = this->routes.find(uri);
                auto jsonIt = this->json_responses.find(uri);
                auto htmlIt = this->html_responses.find(uri);

                if (routeIt != this->routes.end())
                {
                    body = Utils::readFileToBuffer(this->static_files_dir + routeIt->second);
                    status_code = 200;
                }
                else if (jsonIt != this->json_responses.end())
                {
                    body = jsonIt->second;
                    status_code = 200;
                    content_type = "application/json";
                }
                else if (htmlIt != this->html_responses.end())
                {
                    body = htmlIt->second;
                    status_code = 200;
                    content_type = "text/html";
                }
                else
                {
                    try
                    {
                        body = Utils::readFileToBuffer(this->static_files_dir + uri);
                        status_code = 200;
                    }
                    catch (FileNotFoundException &)
                    {
                        body = Utils::readFileToBuffer(this->static_files_dir + "/404.html");
                        content_type = "text/html";
                        status_code = 404;
                    }
                }
            }
            else
            {
                // If not GET, assume method not supported
                body = Utils::readFileToBuffer(this->static_files_dir + "/405.html");
                content_type = "text/html";
                status_code = 405;
            }
        }
        else
        {
            // Handle parsing failure by responding with 500 Internal Server Error
            body = Utils::readFileToBuffer(this->static_files_dir + "/500.html");
            content_type = "text/html";
        }
    }
    catch (std::exception &e)
    {
        body = Utils::readFileToBuffer(this->static_files_dir + "/500.html");
        content_type = "text/html";
    }
    Response response(body, content_type, status_code);
    context->response = &response;
    this->cors_middleware.handle(context.get());
    this->security_middleware.handle(context.get());

    LOG_INFO(logger, this->socket.getClientIp() + " " + method + " " + uri + " HTTP/1.1 " + response.getStatusCode() + " " + response.getStatusCodeDescription());

    std::string raw_response = response.getRawResponse();
    this->socket.sendData(client_socket, raw_response.c_str(), raw_response.length(), 0);

    this->socket.closeSocket(client_socket);
}

Teapot::Teapot() : Teapot("127.0.0.1", 8000, 10, DEFAULT, "static") {}

Teapot::Teapot(unsigned int port) : Teapot("127.0.0.1", port, 10, DEFAULT, "static") {}

Teapot::Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type, std::string static_files_dir)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;
    this->logging_type = logging_type;
    this->static_files_dir = static_files_dir;
    this->sanitizer_middleware = SanitizerMiddleware();
    this->cors_middleware = CORSMiddleware();
    this->security_middleware = SecurityMiddleware();
    this->logger = ConsoleLogger();
    // Conditional compilation based on the operating system
#ifdef __linux__
    this->socket = tpt::UnixSocket(this->logger, this->ip_address, this->port, this->max_connections);
#endif
#ifdef _WIN32
    this->socket = tpt::WinSocket(this->ip_address, this->port, this->max_connections);
#endif
}

void Teapot::run()
{
#ifdef __linux__
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
#endif
#ifdef _WIN32
    if (!SetConsoleCtrlHandler(consoleHandler, TRUE))
    {
        std::cerr << "ERROR: Could not set control handler" << std::endl;
        return;
    }
#endif

    socket.bindSocket();

    while (!signal_received)
    {
        socket.listenToConnections();

        SOCKET client_socket;
        void *client_addr = nullptr;

        try
        {
            socket.acceptConnection(client_socket, client_addr);
            auto res = std::async(std::launch::async, &Teapot::requestHandler, this, client_socket);
            // std::jthread th(&Teapot::requestHandler, this, client_socket);
        }
        catch (IPBlackListedException &e)
        {
            std::cout << e.what();
            this->socket.closeSocket(client_socket);
        }
        catch (SocketAcceptException &)
        {
            continue;
        }
        catch (SocketCloseException &)
        {
            exit(EXIT_FAILURE);
        }
    }

    LOG_INFO(logger, "Shutting down...");
}

void Teapot::serveFile(std::string url, std::string file_path)
{
    this->routes.insert(std::pair<std::string, std::string>(url, file_path));
}

void Teapot::returnJSON(std::string url, std::string json)
{
    if (!Utils::isValidJSON(json))
        throw InvalidJSONException();
    json = Utils::formatJSON(json);
    this->json_responses.insert(std::pair<std::string, std::string>(url, json));
}

void Teapot::returnHTML(std::string url, std::string html)
{
    this->html_responses.insert(std::pair<std::string, std::string>(url, html));
}

void Teapot::addMiddleware(CORSMiddleware cors_middleware)
{
    this->cors_middleware = cors_middleware;
}

void Teapot::addMiddleware(SanitizerMiddleware sanitizer_middleware)
{
    this->sanitizer_middleware = sanitizer_middleware;
}

void Teapot::addMiddleware(SecurityMiddleware security_middleware)
{
    this->security_middleware = security_middleware;
}

Teapot::~Teapot()
{
    this->socket.closeSocket();
}
