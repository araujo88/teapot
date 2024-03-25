#ifndef MIDDLEWARE_H_
#define MIDDLEWARE_H_

#include "context.hpp" // Include the Context header

namespace tpt
{

    class IMiddleware
    {
    public:
        virtual void handle(Context *context) = 0;
    };

} // namespace tpt

#endif // MIDDLEWARE_H_
