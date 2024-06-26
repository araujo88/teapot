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
#include <filesystem>
#include "base_exceptions.hpp"

namespace tpt
{
    class Utils
    {
    private:
        Utils() {}
        Utils(const Utils &) = delete;
        Utils &operator=(const Utils &) = delete;

    public:
        static Utils &getInstance()
        {
            static Utils instance;
            return instance;
        }

        static std::string readFileToBuffer(const std::string &filename);
        static std::string date();
        static std::string btos(bool x);
        static std::string &replaceString(std::string &s, const std::string &from, const std::string &to);
        static std::string &formatJSON(std::string &json);
        static bool isValidJSON(const std::string &str);
        static std::string sanitizePath(const std::string &path);
        static void fillIPBlacklist(std::vector<std::string> &ip_blacklist);
    };
}

#endif
