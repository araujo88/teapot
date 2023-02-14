#ifndef BASE_EXCEPTIONS_H_
#define BASE_EXCEPTIONS_H_
#include <exception>

namespace tpt
{
    class FileNotFoundException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error - file not found";
        }
    };

    class InvalidJSONException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error - JSON parsing error";
        }
    };
}

#endif