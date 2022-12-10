#ifndef HTTP_STATUS_H_
#define HTTP_STATUS_H_

#include <iostream>
#include <utility>
#include <vector>
#include <iterator>

class HttpStatus
{
public:
    static std::string description(int status_code);
};

#endif