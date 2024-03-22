#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <iostream>
#include <list>
#include <sstream>
#include "http_status.hpp"
#include "utils.hpp"

namespace tpt
{
    class Response
    {
    private:
        std::string raw;
        std::string body;
        std::string date;
        unsigned int status_code;
        std::string status_code_description;
        std::string content_type;
        unsigned int content_length;
        std::list<std::string> headers;

        void parseDate();

    public:
        Response(std::string body, std::string content_type, unsigned int status_code);
        std::string getRawResponse();
        std::string getStatusCode();
        std::string getStatusCodeDescription();
        void addHeader(std::string header);
        ~Response();
    };
}

#endif
