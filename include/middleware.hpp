#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include "request.hpp"
#include "response.hpp"

namespace tpt
{
    class IMiddleware
    {
    public:
        virtual void requestHandler(Request *request) = 0;
        virtual void responseHandler(Response *response) = 0;
    };
}

#endif