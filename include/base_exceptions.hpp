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
            return "File not found";
        }
    };

    class InvalidJSONException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "JSON parsing error";
        }
    };

    class IPBlackListedException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "IP is blacklisted";
        }
    };

    class SocketCreationException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error when creating socket";
        }
    };

    class SocketCloseException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error closing socket";
        }
    };

    class SocketBindingException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error binding socket";
        }
    };

    class SocketListenException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error listening to connections";
        }
    };

    class SocketAcceptException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error accepting connections";
        }
    };

    class SocketReceiveException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error receiving data";
        }
    };

    class SocketSendException : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "Error sending data";
        }
    };
}

#endif
