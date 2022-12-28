#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "view.hpp"
#include "model.hpp"
#include "request.hpp"
#include "response.hpp"

class Controller
{
private:
    Model model;
    View view;

public:
    Controller(Model model, View view);
    Response handler(Request request);
    ~Controller();
};

#endif
