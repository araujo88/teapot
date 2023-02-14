#ifndef SECURITY_MIDDLEWARE_H_
#define SECURITY_MIDDLEWARE_H_

#include "middleware.hpp"

namespace tpt
{
    class SecurityMiddleware : public Middleware
    {
    private:
        std::string cross_origin_opener_policy;
        std::string referrer_policy;
        std::string strict_transport_security;
        std::string x_content_type_options;
        std::string x_frame_options;
        std::string x_xss_protection;

    public:
        SecurityMiddleware();
        SecurityMiddleware(std::string cross_origin_opener_policy,
                           std::string referrer_policy,
                           std::string strict_transport_security,
                           std::string x_content_type_options,
                           std::string x_frame_options,
                           std::string x_xss_protection);
        virtual void requestHandler(Request *request) override;
        virtual void responseHandler(Response *response) override;
        ~SecurityMiddleware();
    };
}

#endif