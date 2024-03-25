#include "../include/security_middleware.hpp"

using namespace tpt;

SecurityMiddleware::SecurityMiddleware()
{
    this->cross_origin_opener_policy = "same-origin";
    this->referrer_policy = "strict-origin-when-cross-origin";
    this->strict_transport_security = "max-age=31556926; includeSubDomains";
    this->x_content_type_options = "nosniff";
    this->x_frame_options = "DENY";
    this->x_xss_protection = "1; mode=block";
}

SecurityMiddleware::SecurityMiddleware(std::string cross_origin_opener_policy,
                                       std::string referrer_policy,
                                       std::string strict_transport_security,
                                       std::string x_content_type_options,
                                       std::string x_frame_options,
                                       std::string x_xss_protection)
{
    this->cross_origin_opener_policy = cross_origin_opener_policy;
    this->referrer_policy = referrer_policy;
    this->strict_transport_security = strict_transport_security;
    this->x_content_type_options = x_content_type_options;
    this->x_frame_options = x_frame_options;
    this->x_xss_protection = x_xss_protection;
}

void SecurityMiddleware::handle(Context *context)
{
    context->response->addHeader("Cross-Origin-Opener-Policy: " + this->cross_origin_opener_policy);
    context->response->addHeader("Referrer-Policy: " + this->referrer_policy);
    context->response->addHeader("Strict-Transport-Security: " + this->strict_transport_security);
    context->response->addHeader("X-Content-Type-Options: " + this->x_content_type_options);
    context->response->addHeader("X-Frame-Options: " + this->x_frame_options);
    context->response->addHeader("X-XSS-Protection: " + this->x_xss_protection);
}

SecurityMiddleware::~SecurityMiddleware()
{
}
