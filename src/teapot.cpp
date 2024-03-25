#include "../include/teapot.hpp"
#include "../include/unix_socket.hpp"

using namespace tpt;

std::optional<Request> Teapot::parseRequest(int client_socket)
{
    char buffer[BUFFER_SIZE] = {0};
    std::string raw_request;

    ssize_t bytes_received = this->socket.receiveData(client_socket, buffer, BUFFER_SIZE);
    if (bytes_received > 0)
    {
        raw_request = std::string(buffer);
        std::cout << raw_request << std::endl;
        return Request(raw_request);
    }
    else
    {
        return std::nullopt;
    }
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
            try
            {
                body = Utils::readFileToBuffer(this->static_files_dir + request->getUri());
                if (request->getUri() == "/")
                {
                    body = Utils::readFileToBuffer(this->static_files_dir + "/index.html");
                    content_type = "text/html";
                }
                if (request->getUri().length() >= 3 && request->getUri().substr(request->getUri().length() - 3) == "css")
                {
                    content_type = "text/css";
                }
                else if (request->getUri().length() >= 3 && request->getUri().substr(request->getUri().length() - 3) == "ico")
                {
                    content_type = "image/x-icon";
                }
                else if (request->getUri().length() >= 3 && request->getUri().substr(request->getUri().length() - 3) == "gif")
                {
                    content_type = "image/gif";
                }
                else if (request->getUri().length() >= 3 && request->getUri().substr(request->getUri().length() - 3) == "jpg")
                {
                    content_type = "image/jpeg";
                }
                else if (request->getUri().length() >= 3 && request->getUri().substr(request->getUri().length() - 3) == "jpeg")
                {
                    content_type = "image/jpeg";
                }
                else if (request->getUri().length() >= 3 && request->getUri().substr(request->getUri().length() - 3) == "png")
                {
                    content_type = "image/png";
                }
                else if (request->getUri().length() >= 2 && request->getUri().substr(request->getUri().length() - 2) == "js")
                {
                    content_type = "text/javascript";
                }
                else if (request->getUri().length() >= 4 && request->getUri().substr(request->getUri().length() - 4) == "html")
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
                if (request->getUri() == url)
                {
                    body = Utils::readFileToBuffer(this->static_files_dir + file_path);
                    status_code = 200;
                    content_type = "text/html";
                }
            }
            for (auto const &[url, json] : this->json_responses)
            {
                if (request->getUri() == url)
                {
                    body = json;
                    status_code = 200;
                    content_type = "application/json";
                }
            }
            for (auto const &[url, html] : this->html_responses)
            {
                if (request->getUri() == url)
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
