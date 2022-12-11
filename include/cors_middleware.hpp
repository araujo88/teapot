#ifndef CORS_MIDDLEWARE_H_
#define CORS_MIDDLEWARE_H_

#include "middleware.hpp"
#include "request.hpp"
#include "response.hpp"

class CORSMiddleware : public Middleware
{
private:
    std::string allow_origins;
    std::string allow_methods;
    std::string allow_headers;
    std::string allow_credentials;

public:
    CORSMiddleware();
    CORSMiddleware(std::string allow_origins,
                   std::string allow_methods,
                   std::string allow_headers,
                   std::string allow_credentials);
    ~CORSMiddleware();
    void responseHandler(Response *response);
};

#endif