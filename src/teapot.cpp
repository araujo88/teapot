#include "../include/teapot.hpp"

std::unordered_map<std::string, std::string> Teapot::parseFormData(const std::string &data) // application/x-www-form-urlencoded
{
    std::unordered_map<std::string, std::string> result;
    size_t pos = 0;
    while (pos < data.length())
    {
        size_t key_end = data.find('=', pos);
        if (key_end == std::string::npos)
        {
            break;
        }
        std::string key = data.substr(pos, key_end - pos);
        pos = key_end + 1;
        size_t value_end = data.find('&', pos);
        std::string value = value_end == std::string::npos ? data.substr(pos) : data.substr(pos, value_end - pos);
        result[key] = value;
        pos = value_end == std::string::npos ? data.length() : value_end + 1;
    }
    return result;
}

Request Teapot::parseRequest(int *client_socket)
{
    char buffer[BUFFER_SIZE];
    std::string raw_request;

    checkReceive(client_socket, buffer);

    raw_request = std::string(buffer);

    return Request(raw_request);
}

void Teapot::requestHandler(int *client_socket)
{
    Request request = parseRequest(client_socket);
    std::string raw_response;
    std::string body;
    std::string content_type;
    unsigned int status_code;

    std::cout << "[" + request.getDate() + "]" + " " + request.getMethod() + " " + request.getUri() + " HTTP/1.1 ";

    if (request.getMethod() == "POST" || request.getMethod() == "PUT")
    {
        std::cout << "Content-type: " + request.getHeader("Content-Type") << std::endl;
        this->sanitizer_middleware.requestHandler(&request);
        std::cout << request.getBody() << std::endl;
    }

    if (request.getMethod() == "GET")
    {
        try
        {
            body = Utils::readFileToBuffer(this->static_files_dir + request.getUri());
            if (request.getUri() == "/")
            {
                body = Utils::readFileToBuffer(this->static_files_dir + "/index.html");
                content_type = "text/html";
            }
            else if (request.getUri().substr(request.getUri().length() - 3) == "css")
            {
                content_type = "text/css";
            }
            else if (request.getUri().substr(request.getUri().length() - 3) == "ico")
            {
                content_type = "image/x-icon";
            }
            else if (request.getUri().substr(request.getUri().length() - 3) == "gif")
            {
                content_type = "image/gif";
            }
            else if (request.getUri().substr(request.getUri().length() - 3) == "jpg")
            {
                content_type = "image/jpeg";
            }
            else if (request.getUri().substr(request.getUri().length() - 3) == "jpeg")
            {
                content_type = "image/jpeg";
            }
            else if (request.getUri().substr(request.getUri().length() - 3) == "png")
            {
                content_type = "image/png";
            }
            else
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
            if (request.getUri() == url)
            {
                body = Utils::readFileToBuffer(this->static_files_dir + file_path);
                status_code = 200;
                content_type = "text/html";
            }
        }
        for (auto const &[url, json] : this->json_responses)
        {
            if (request.getUri() == url)
            {
                body = json;
                status_code = 200;
                content_type = "application/json";
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

    this->cors_middleware.responseHandler(&response);
    this->security_middleware.responseHandler(&response);

    raw_response = response.getRawResponse();

    send(*client_socket, raw_response.c_str(), raw_response.length(), 0);

    close(*client_socket);
    delete client_socket;
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
}

Teapot::Teapot(unsigned int port)
{
    this->ip_address = "127.0.0.1";
    this->port = port;
    this->max_connections = 10;
    this->logging_type = DEFAULT;
    this->static_files_dir = "static";
}

Teapot::Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type, std::string static_files_dir)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;
    this->logging_type = logging_type;
    this->static_files_dir = static_files_dir;
}

void Teapot::runServer()
{

    std::cout << "Creating socket ..." << std::endl;
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);

    checkSocket();

    std::cout << "Socket created!" << std::endl;

    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    this->server_address.sin_addr.s_addr = inet_addr(this->ip_address.c_str());

    std::cout << "Binding socket ..." << std::endl;

    checkBind();

    std::cout << "Binding done!" << std::endl;
    std::cout << "Waiting for incoming requests..." << std::endl;

    while (true)
    {
        checkListen();

        int *client_socket = new int;
        struct sockaddr_in *client_address = NULL;

        checkAccept(client_socket, (struct sockaddr *)client_address);

        std::thread th(&Teapot::requestHandler, this, client_socket);
        th.join();
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

void Teapot::checkSocket()
{
    if (this->server_socket < 0)
    {
        perror("Socket failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Teapot::checkBind()
{
    if ((bind(this->server_socket, (struct sockaddr *)&this->server_address, sizeof(this->server_address))) < 0)
    {
        perror("Bind failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Teapot::checkListen()
{
    if ((listen(this->server_socket, this->max_connections)) < 0)
    {
        perror("Listen failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Teapot::checkAccept(int *client_socket, struct sockaddr *client_address)
{
    if ((*client_socket = accept(this->server_socket, (struct sockaddr *)client_address, (socklen_t *)sizeof(client_address))) < 0)
    {
        perror("Accept failed");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

void Teapot::checkReceive(int *client_socket, char buffer[BUFFER_SIZE])
{
    if ((recv(*client_socket, (void *)buffer, BUFFER_SIZE, 0)) < 0)
    {
        perror("Receive error");
        std::cout << "Error code: " + errno << std::endl;
        exit(1);
    }
}

Teapot::~Teapot()
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