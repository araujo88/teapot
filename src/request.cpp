#include "../include/request.hpp"

using namespace tpt;

Request::Request(const std::string &raw)
{
    this->raw = raw;
    this->date = Utils::date();

    size_t pos = 0;

    // Parse request line
    size_t line_end = raw.find("\r\n");
    std::string request_line = raw.substr(pos, line_end - pos);
    pos = line_end + 2;
    size_t method_end = request_line.find(' ');
    this->method = request_line.substr(0, method_end);
    size_t uri_end = request_line.find(' ', method_end + 1);
    this->uri = request_line.substr(method_end + 1, uri_end - method_end - 1);

    // Parse headers
    size_t headers_end = raw.find("\r\n\r\n");
    std::string headers_str = raw.substr(0, headers_end);
    pos = 0;
    while (pos < headers_end)
    {
        size_t header_line_end = headers_str.find("\r\n", pos);
        if (header_line_end == std::string::npos)
        {
            break;
        }
        std::string header_line = headers_str.substr(pos, header_line_end - pos);
        pos = header_line_end + 2;
        size_t colon_pos = header_line.find(':');
        if (colon_pos == std::string::npos)
        {
            continue;
        }
        std::string key = header_line.substr(0, colon_pos);
        std::string value = header_line.substr(colon_pos + 2);
        this->headers[key] = value;
    }

    // Extract body
    if (headers_end == std::string::npos)
    {
        this->body = "";
    }
    this->body = raw.substr(headers_end + 4);
}

std::string Request::getMethod()
{
    return this->method;
}

std::string Request::getUri()
{
    return this->uri;
}

void Request::printHeaders()
{
    for (const auto &[key, value] : this->headers)
    {
        std::cout << "  " << key << ": " << value << std::endl;
    }
}

std::string Request::getHeader(std::string key)
{
    return this->headers.find(key)->second;
}

std::string Request::getDate()
{
    return this->date;
}

void Request::setBody(std::string body)
{
    this->body = body;
}

std::string Request::getBody()
{
    return this->body;
}

Request::~Request()
{
}
