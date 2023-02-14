#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <stack>
#include "base_exceptions.hpp"

namespace tpt
{
    class Utils
    {
    public:
        static std::string readFileToBuffer(std::string filename);
        static std::string date();
        static std::string btos(bool x);
        static std::string &replaceString(std::string &s, const std::string &from, const std::string &to);
        static std::string &formatJSON(std::string &json);
        static bool isValidJSON(const std::string &str);
    };
}

#endif