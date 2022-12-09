#include <iostream>
#include "http_status.hpp"

class Request
{
private:
    typedef enum _type
    {
        GET,
        HEAD,
        POST,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH
    } type;
    std::string raw;
    std::string headers;
    std::string body;
    std::string date;
    std::string method;

public:
    Request();
    ~Request();
};