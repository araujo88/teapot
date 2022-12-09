#include <iostream>
#include <algorithm>
#include <sstream>

class Request
{
private:
    typedef enum _http_method
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
    } http_method;
    std::string method;
    std::string raw;
    std::string headers;
    std::string body;
    std::string url;
    std::string http_version;

public:
    Request(std::string raw);
    ~Request();
};