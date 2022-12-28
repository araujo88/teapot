#ifndef VIEW_H_
#define VIEW_H_

#include "response.hpp"

class View
{
private:
    typedef enum _type
    {
        json,
        html,
        file
    } type;

public:
    View();
    void render(type view_type, Response response);
    ~View();
};

#endif