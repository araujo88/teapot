#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include "request.hpp"
#include "response.hpp"

class Middleware
{
public:
    Middleware(){};
    void requestHandler(Request *request);
    void responseHandler(Response *response);
    ~Middleware(){};
};

#endif