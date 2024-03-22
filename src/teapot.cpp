#include "../include/teapot.hpp"
#include "../include/unix_socket.hpp"

using namespace tpt;

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

Model Teapot::extractAndStore(const std::string &requestBody)
{
    auto formData = this->parseFormData(requestBody);
    return Model{formData["name"], formData["surname"]};
}

Request Teapot::parseRequest(int client_socket)
{
    char buffer[BUFFER_SIZE];
    std::string raw_request;

    this->socket.receiveData(client_socket, buffer, BUFFER_SIZE);

    raw_request = std::string(buffer);

    return Request(raw_request);
}

void Teapot::mainEventLoop(int client_socket)
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
            if (request.getUri() == "/all")
            {
                body = Utils::readFileToBuffer(this->static_files_dir + "/db.json");
                content_type = "application/json";
            }
            if (request.getUri().length() >= 3 && request.getUri().substr(request.getUri().length() - 3) == "css")
            {
                content_type = "text/css";
            }
            else if (request.getUri().length() >= 3 && request.getUri().substr(request.getUri().length() - 3) == "ico")
            {
                content_type = "image/x-icon";
            }
            else if (request.getUri().length() >= 3 && request.getUri().substr(request.getUri().length() - 3) == "gif")
            {
                content_type = "image/gif";
            }
            else if (request.getUri().length() >= 3 && request.getUri().substr(request.getUri().length() - 3) == "jpg")
            {
                content_type = "image/jpeg";
            }
            else if (request.getUri().length() >= 3 && request.getUri().substr(request.getUri().length() - 3) == "jpeg")
            {
                content_type = "image/jpeg";
            }
            else if (request.getUri().length() >= 3 && request.getUri().substr(request.getUri().length() - 3) == "png")
            {
                content_type = "image/png";
            }
            else if (request.getUri().length() >= 2 && request.getUri().substr(request.getUri().length() - 2) == "js")
            {
                content_type = "text/javascript";
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
        for (auto const &[url, html] : this->html_responses)
        {
            if (request.getUri() == url)
            {
                body = html;
                status_code = 200;
                content_type = "text/html";
            }
        }
    }
    else if (request.getMethod() == "POST")
    {
        db::JsonDatabase database = db::JsonDatabase(this->static_files_dir + "/db.json");
        auto data = parseFormData(request.getBody());
        Model model = extractAndStore(request.getBody());

        std::unordered_map<std::string, std::string> newEntry = {
            {"id", std::to_string(model.getId())},
            {"name", model.getName()},
            {"surname", model.getSurname()}};

        database.write(Utils::formatFormDataToJson(newEntry));
        status_code = 201;
        body = Utils::readFileToBuffer(this->static_files_dir + "/201.html");
        content_type = "text/html";
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

    this->socket.sendData(client_socket, raw_response.c_str(), raw_response.length(), 0);
    this->socket.closeSocket(client_socket);
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
    this->socket = tpt::UnixSocket(port);
}

Teapot::Teapot(std::string ip_address, unsigned int port, unsigned int max_connections, logging logging_type, std::string static_files_dir)
{
    this->ip_address = ip_address;
    this->port = port;
    this->max_connections = max_connections;
    this->logging_type = logging_type;
    this->static_files_dir = static_files_dir;
    this->socket = tpt::UnixSocket(ip_address, port, max_connections);
}

void Teapot::runServer()
{
    socket.bindSocket();

    while (true)
    {
        socket.listenToConnections();

        int client_socket_fd;
        void *client_addr = nullptr;

        socket.acceptConnection(client_socket_fd, client_addr);

        std::jthread th(&Teapot::mainEventLoop, this, client_socket_fd);
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
