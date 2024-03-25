#ifndef SANITIZER_MIDDLEWARE_H_
#define SANITIZER_MIDDLEWARE_H_

#include "middleware.hpp"
#include "request.hpp"
#include "response.hpp"

namespace tpt
{
    class SanitizerMiddleware : public IMiddleware
    {
    public:
        SanitizerMiddleware();
        ~SanitizerMiddleware();
        virtual void handle(Context *context) override;
    };
}

#endif
