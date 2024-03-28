#include "../include/response.hpp"

using namespace tpt;

Response::Response(std::string body, std::string content_type, unsigned int status_code)
{
    HttpStatus http_status;
    std::ostringstream response_stream;

    this->content_type = content_type;
    this->status_code = status_code;
    this->body = "\n" + body;
    this->content_length = body.length();
    this->status_code_description = http_status.description(this->status_code);
    this->date = Utils::date();

    this->headers = {{"HTTP/1.1 " + std::to_string(this->status_code) + " " + this->status_code_description + "\n"},
                     {"Date: " + this->date + "\n"},
                     {"Content-Type: " + this->content_type + "\n"},
                     {"Content-Length: " + std::to_string(this->content_length) + "\n"}};

    for (auto const &i : this->headers)
    {
        response_stream << i;
    }
    response_stream << this->body;

    this->addHeader("Server: Teapot");
    this->raw = response_stream.str();
}

std::string Response::getRawResponse()
{
    return this->raw;
}

void Response::addHeader(std::string header)
{
    std::ostringstream response_stream;

    this->headers.push_back(header + "\n");

    for (auto const &i : this->headers)
    {
        response_stream << i;
    }
    response_stream << this->body;
    this->raw = response_stream.str();
}

std::string Response::getStatusCode()
{
    return std::to_string(this->status_code);
}

std::string Response::getStatusCodeDescription()
{
    return this->status_code_description;
}

Response::~Response()
{
}
