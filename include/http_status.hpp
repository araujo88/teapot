#ifndef HTTP_STATUS_H_
#define HTTP_STATUS_H_

#include <iostream>
#include <utility>
#include <vector>
#include <iterator>

namespace tpt
{
    class HttpStatus
    {
    public:
        static std::string description(int status_code);
    };
}

#endif