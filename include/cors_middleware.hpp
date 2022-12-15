#ifndef CORS_MIDDLEWARE_H_
#define CORS_MIDDLEWARE_H_

#include "middleware.hpp"
#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"

class CORSMiddleware : public Middleware
{
private:
    std::string allow_origins;
    std::string allow_methods;
    std::string allow_headers;
    unsigned int max_age;
    bool allow_credentials;

public:
    CORSMiddleware();
    CORSMiddleware(std::string allow_origins,
                   std::string allow_methods,
                   std::string allow_headers,
                   unsigned int max_age,
                   bool allow_credentials);
    ~CORSMiddleware();
    void responseHandler(Response *response);
};

#endif