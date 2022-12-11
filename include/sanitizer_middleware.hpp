#ifndef SANITIZER_MIDDLEWARE_H_
#define SANITIZER_MIDDLEWARE_H_

#include "middleware.hpp"
#include "request.hpp"
#include "response.hpp"

class SanitizerMiddleware : public Middleware
{
public:
    SanitizerMiddleware();
    ~SanitizerMiddleware();
    void requestHandler(Request *request);
    void responseHandler(Response *response);
};

#endif