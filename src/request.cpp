#include "../include/request.hpp"

Request::Request(std::string raw)
{
    this->raw = raw;

    std::istringstream iss(raw);
    std::string s;
    for (int i = 0; getline(iss, s, ' '); i++)
    {
        if (i == 0)
            this->method = s;
        if (i == 1)
            this->url = s;
        if (i == 2)
        {
            this->http_version = s;
            break;
        }
    }
    std::cout << "Method " << this->method << std::endl;
    std::cout << "URL " << this->url << std::endl;
    std::cout << "Version " << this->http_version << std::endl;
}

Request::~Request()
{
}