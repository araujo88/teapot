#ifndef REQUEST_H_
#define REQUEST_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include "utils.hpp"

namespace tpt
{
    static const std::vector<std::string> http_methods = {
        "GET",
        "HEAD",
        "POST",
        "PUT",
        "DELETE",
        "CONNECT",
        "OPTIONS",
        "TRACE",
        "PATCH"};

    class Request
    {
    private:
        std::string method;
        std::string raw;
        std::map<std::string, std::string> headers;
        std::string uri;
        std::string date;
        std::string body;

    public:
        Request(const std::string &raw);
        std::string getMethod();
        std::string getUri();
        std::string getDate();
        std::string getBody();
        std::string getHeader(std::string key);
        void setBody(std::string body);
        void printHeaders();
        ~Request();
    };
}

#endif
