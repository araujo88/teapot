#include "../include/teapot.hpp"
#include "../include/unix_socket.hpp"

using namespace tpt;

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

void Teapot::mainEventLoop(int client_socket)
{
    Context *context = new Context(nullptr, nullptr);
    std::optional<Request> request = parseRequest(client_socket);

    if (request)
    {
        std::string raw_response;
        std::string body;
        std::string content_type;
        unsigned int status_code;

        context->request = &request.value();

        std::cout << "[" + request->getDate() + "]" + " " + request->getMethod() + " " + request->getUri() + " HTTP/1.1 ";

        if (request->getMethod() == "POST" || request->getMethod() == "PUT")
        {
            std::cout << "Content-type: " + request->getHeader("Content-Type") << std::endl;
            this->sanitizer_middleware.handle(context);
            std::cout << request->getBody() << std::endl;
        }

        if (request->getMethod() == "GET")
        {
            std::string uri = request->getUri();
            if (uri == "/")
            {
                uri = "/index.html";
            }
            try
            {
                body = Utils::readFileToBuffer(this->static_files_dir + uri);
                if (uri == "/")
                {
                    body = Utils::readFileToBuffer(this->static_files_dir + "/index.html");
                    content_type = "text/html";
                }
                if (uri.length() >= 3 && uri.substr(uri.length() - 3) == "css")
                {
                    content_type = "text/css";
                }
                else if (uri.length() >= 3 && uri.substr(uri.length() - 3) == "ico")
                {
                    content_type = "image/x-icon";
                }
                else if (uri.length() >= 3 && uri.substr(uri.length() - 3) == "gif")
                {
                    content_type = "image/gif";
                }
                else if (uri.length() >= 3 && uri.substr(uri.length() - 3) == "jpg")
                {
                    content_type = "image/jpeg";
                }
                else if (uri.length() >= 3 && uri.substr(uri.length() - 3) == "jpeg")
                {
                    content_type = "image/jpeg";
                }
                else if (uri.length() >= 3 && uri.substr(uri.length() - 3) == "png")
                {
                    content_type = "image/png";
                }
                else if (uri.length() >= 2 && uri.substr(uri.length() - 2) == "js")
                {
                    content_type = "text/javascript";
                }
                else if (uri.length() >= 4 && uri.substr(uri.length() - 4) == "html")
                {
                    content_type = "text/html";
                }
                status_code = 200;
            }
            catch (FileNotFoundException &e)
            {
                body = Utils::readFileToBuffer(this->static_files_dir + "/404.html");
                content_type = "text/html";
                status_code = 404;
            }
            for (auto const &[url, file_path] : this->routes)
            {
                if (uri == url)
                {
                    body = Utils::readFileToBuffer(this->static_files_dir + file_path);
                    status_code = 200;
                    content_type = "text/html";
                }
            }
            for (auto const &[url, json] : this->json_responses)
            {
                if (uri == url)
                {
                    body = json;
                    status_code = 200;
                    content_type = "application/json";
                }
            }
            for (auto const &[url, html] : this->html_responses)
            {
                if (uri == url)
                {
                    body = html;
                    status_code = 200;
                    content_type = "text/html";
                }
            }
        }
        else
        {
            status_code = 500;
            body = Utils::readFileToBuffer(this->static_files_dir + "/500.html");
            content_type = "text/html";
        }
        Response response = Response(body, content_type, status_code);
        std::cout << response.getStatusCode() + " " + response.getStatusCodeDescription() << std::endl;

        context->response = &response;

        this->cors_middleware.handle(context);
        this->security_middleware.handle(context);

        raw_response = response.getRawResponse();

        this->socket.sendData(client_socket, raw_response.c_str(), raw_response.length(), 0);
    }
    this->socket.closeSocket(client_socket);
    delete context;
}

Teapot::Teapot()
{
    this->ip_address = "127.0.0.1";
    this->port = 8000;
    this->max_connections = 10;
    this->logging_type = DEFAULT;
    this->static_files_dir = "static";
    this->sanitizer_middleware = SanitizerMiddleware();
    this->cors_middleware = CORSMiddleware();
    this->security_middleware = SecurityMiddleware();
#ifdef __linux__
    this->socket = tpt::UnixSocket(this->port);
#endif
#ifdef _WIN32
    this->socket = tpt::WinSocket(this->port);
#endif
}

Teapot::Teapot(unsigned int port)
{
    this->ip_address = "127.0.0.1";
    this->port = port;
    this->max_connections = 10;
    this->logging_type = DEFAULT;
    this->static_files_dir = "static";
#ifdef __linux__
    this->socket = tpt::UnixSocket(this->port);
#endif
#ifdef _WIN32
    this->socket = tpt::WinSocket(this->port);
#endif
}

Teapot::Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type, std::string static_files_dir)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;
    this->logging_type = logging_type;
    this->static_files_dir = static_files_dir;
#ifdef __linux__
    this->socket = tpt::UnixSocket(ip_address, port, max_connections);
#endif
#ifdef _WIN32
    this->socket = tpt::WinSocket(ip_address, port, max_connections);
#endif
}

void Teapot::runServer()
{
    socket.bindSocket();

    while (true)
    {
        socket.listenToConnections();

        SOCKET client_socket;
        void *client_addr = nullptr;

        socket.acceptConnection(client_socket, client_addr);

        std::jthread th(&Teapot::mainEventLoop, this, client_socket);
    }
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
