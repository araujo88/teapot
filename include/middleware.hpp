#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include "request.hpp"
#include "response.hpp"

class Middleware
{
    Middleware(Request request, Response response);
    ~Middleware();
};

#endif