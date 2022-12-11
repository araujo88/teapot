#include "../include/request.hpp"

Request::Request(std::string raw)
{
    this->raw = raw;
    this->date = Utils::date();

    std::istringstream iss(raw);
    std::string s;
    for (int i = 0; getline(iss, s, ' '); i++)
    {
        if (i == 0)
            this->method = s;
        if (i == 1)
        {
            this->url = s;
            break;
        }
    }
}

std::string Request::getMethod()
{
    return this->method;
}

std::string Request::getUrl()
{
    return this->url;
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