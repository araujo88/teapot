#include "../include/sanitizer_middleware.hpp"

SanitizerMiddleware::SanitizerMiddleware()
{
}

void SanitizerMiddleware::requestHandler(Request *request)
{
    std::string body = request->getBody();
    body = Utils::replaceString(body, "<", "&lt;");
    body = Utils::replaceString(body, ">", "&gt;");
    request->setBody(body);
}

void SanitizerMiddleware::responseHandler(Response *response)
{
}

SanitizerMiddleware::~SanitizerMiddleware()
{
}