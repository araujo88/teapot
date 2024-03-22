#include "../include/controller.hpp"

using namespace tpt;

Controller::Controller(Model model, View view)
{
    this->model = model;
    this->view = view;
}

Response Controller::handler(Request request)
{
    std::string body = "test";
    std::string content_type = "text/html";
    unsigned int status_code = 200;

    if (request.getMethod() == "GET" && request.getUri() == "/users")
    {
        return Response(body, content_type, status_code);
    }
    else
    {
        return Response("Error 404 - not found", "text/html", 404);
    }
}

Controller::~Controller()
{
}
