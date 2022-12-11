#ifndef REQUEST_H_
#define REQUEST_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include "utils.hpp"

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
    std::list<std::string> headers;
    std::string url;
    std::string http_version;
    std::string date;
    std::string body;

public:
    Request(std::string raw);
    std::string getMethod();
    std::string getUrl();
    std::string getDate();
    std::string getBody();
    void setBody(std::string body);
    ~Request();
};

#endif