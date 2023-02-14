#include "../include/middleware.hpp"

using namespace tpt;

Middleware::Middleware() {}

Middleware::~Middleware() {}

void Middleware::requestHandler(Request *request){};

void Middleware::responseHandler(Response *response){};