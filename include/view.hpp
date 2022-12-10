#ifndef VIEW_H_
#define VIEW_H_

#include <iostream>

class View
{
private:
    std::string url;
    std::string filepath;

public:
    View();
    View(std::string raw_response);

    ~View();
};

#endif