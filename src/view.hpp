#include <iostream>

class View
{
private:
    std::string endpoint;
    std::string filepath;

public:
    View();
    View(std::string raw_response);

    ~View();
};