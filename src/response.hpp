#include <iostream>
#include "http_status.hpp"

class Response
{
private:
    std::string raw;
    std::string headers;
    std::string body;
    std::string date;
    std::string status_code;
    std::string description;

public:
    Response(std::string status_code, std::string headers, std::string body);
    ~Response();
};