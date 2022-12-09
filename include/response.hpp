#include <iostream>
#include "http_status.hpp"
#include "headers.hpp"

class Response
{
private:
    std::string raw;
    std::string body;
    std::string date;
    std::string status_code;
    std::string description;
    Headers headers;

public:
    Response(std::string status_code, Headers headers, std::string body);
    ~Response();
};