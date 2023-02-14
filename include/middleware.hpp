#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include "request.hpp"
#include "response.hpp"

namespace tpt
{
    class Middleware
    {
    public:
        Middleware();
        virtual void requestHandler(Request *request);
        virtual void responseHandler(Response *response);
        ~Middleware();
    };
}

#endif