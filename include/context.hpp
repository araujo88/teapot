#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "request.hpp"
#include "response.hpp"

namespace tpt
{

    // Context class encapsulates the request and response,
    // along with any additional data that might be relevant
    // for the middleware and request handling lifecycle.
    class Context
    {
    public:
        Request *request;
        Response *response;
        // Add additional fields or methods here that can be used to
        // store and access data relevant to the request-processing lifecycle.

        Context(Request *req, Response *res) : request(req), response(res) {}

        // Additional helper methods can be added here to simplify
        // working with the request and response or to add custom logic.
    };

} // namespace tpt

#endif // CONTEXT_H_
