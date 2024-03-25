#include "../include/request.hpp"

using namespace tpt;

Request::Request(const std::string& raw) {
    this->raw = raw;
    this->date = Utils::date();

    size_t pos = 0;

    // Parse request line
    size_t line_end = raw.find("\r\n");
    if (line_end == std::string::npos) {
        // Handle error or set defaults
        return;
    }
    std::string request_line = raw.substr(pos, line_end);
    pos = line_end + 2;
    size_t method_end = request_line.find(' ');
    if (method_end != std::string::npos) {
        this->method = request_line.substr(0, method_end);
    }
    size_t uri_end = request_line.find(' ', method_end + 1);
    if (uri_end != std::string::npos) {
        this->uri = request_line.substr(method_end + 1, uri_end - method_end - 1);
    }
    this->uri = Utils::sanitizePath(this->uri);

    // Parse headers
    size_t headers_end = raw.find("\r\n\r\n", pos);
    if (headers_end != std::string::npos) {
        std::string headers_str = raw.substr(pos, headers_end - pos);
        pos = 0;
        while (pos < headers_str.length()) {
            size_t header_line_end = headers_str.find("\r\n", pos);
            if (header_line_end == std::string::npos) {
                break; // Exit the loop if no more headers are found.
            }
            std::string header_line = headers_str.substr(pos, header_line_end - pos);
            pos = header_line_end + 2;
            size_t colon_pos = header_line.find(':');
            if (colon_pos == std::string::npos) {
                continue; // Skip invalid header line.
            }
            std::string key = header_line.substr(0, colon_pos);
            std::string value = header_line.substr(colon_pos + 1).erase(0, header_line[colon_pos + 1] == ' ' ? 1 : 0); // Adjust for potential space.
            this->headers[key] = value;
        }
    }

    // Extract body
    if (headers_end != std::string::npos && headers_end + 4 < raw.length()) {
        this->body = raw.substr(headers_end + 4);
    }
    else {
        this->body = ""; // Ensure body is empty if headers_end is npos.
    }
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
