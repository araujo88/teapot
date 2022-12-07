#include <iostream>

class Response
{
private:
    std::string raw;
    std::string headers;
    std::string body;
    std::string date;
    std::string status_code;

public:
    Response();
    ~Response();
};