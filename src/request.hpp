#include <iostream>

class Request
{
private:
    std::string raw;
    std::string headers;
    std::string body;
    std::string date;
    std::string method;

public:
    Request();
    ~Request();
};