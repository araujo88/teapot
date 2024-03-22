#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <stack>
#include <unordered_map>
#include <vector>
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
        static std::string formatFormDataToJson(std::unordered_map<std::string, std::string> formData);
        static std::vector<std::unordered_map<std::string, std::string>> parseJsonArray(const std::string &json);
    };
}

#endif
