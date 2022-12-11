#include "../include/sanitizer_middleware.hpp"

SanitizerMiddleware::SanitizerMiddleware()
{
}

void SanitizerMiddleware::requestHandler(Request *request)
{
    std::string body = request->getBody();
    body.replace(body.find("<"), std::string("<").size(), "&lt;");
    body.replace(body.find(">"), std::string(">").size(), "&gt;");
    request->setBody(body);
}

SanitizerMiddleware::~SanitizerMiddleware()
{
}