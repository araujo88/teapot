#include "../include/sanitizer_middleware.hpp"

using namespace tpt;

SanitizerMiddleware::SanitizerMiddleware()
{
}

void SanitizerMiddleware::handle(Context *context)
{
    std::string body = context->request->getBody();
    body = Utils::replaceString(body, "<", "&lt;");
    body = Utils::replaceString(body, ">", "&gt;");
    context->request->setBody(body);
}

SanitizerMiddleware::~SanitizerMiddleware()
{
}
