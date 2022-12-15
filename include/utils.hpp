#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "base_exceptions.hpp"

class Utils
{
public:
    static std::string readFileToBuffer(std::string filename);
    static std::string date();
    static std::string btos(bool x);
};

#endif