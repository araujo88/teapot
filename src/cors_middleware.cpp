#include "../include/cors_middleware.hpp"

using namespace tpt;

CORSMiddleware::CORSMiddleware()
{
    this->allow_origins = "127.0.0.1";
    this->allow_methods = "GET";
    this->allow_headers = "X-Requested-With";
    this->max_age = 86400;
    this->allow_credentials = true;
}

CORSMiddleware::CORSMiddleware(std::string allow_origins,
                               std::string allow_methods,
                               std::string allow_headers,
                               unsigned int max_age,
                               bool allow_credentials)
{
    this->allow_origins = allow_origins;
    this->allow_methods = allow_methods;
    this->allow_headers = allow_headers;
    this->max_age = max_age;
    this->allow_credentials = allow_credentials;
}

void CORSMiddleware::handle(Context *context)
{
    context->response->addHeader("Access-Control-Allow-Origin: " + this->allow_origins);
    context->response->addHeader("Access-Control-Allow-Methods: " + this->allow_methods);
    context->response->addHeader("Access-Control-Allow-Headers: " + this->allow_headers);
    context->response->addHeader("Access-Control-Max-Age: " + std::to_string(this->max_age));
    context->response->addHeader("Access-Control-Allow-Credentials: " + Utils::btos(this->allow_credentials));
}

CORSMiddleware::~CORSMiddleware()
{
}
