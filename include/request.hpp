#include <iostream>

class Request
{
private:
    typedef enum _method
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
    } method;
    std::string raw;
    std::string headers;
    std::string body;
    std::string date;
    std::string url;

public:
    Request(std::string raw);
    ~Request();
};