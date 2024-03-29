#ifndef BASE_EXCEPTIONS_H_
#define BASE_EXCEPTIONS_H_

#include <stdexcept>
#include <string>
#include <sstream>

namespace tpt
{
    class BaseException : public std::runtime_error
    {
    public:
        BaseException(const std::string &message = "An error occurred", int errorCode = -1)
            : std::runtime_error(message), m_errorCode(errorCode)
        {
            // Prepare the full message including the error code
            std::ostringstream ss;
            ss << message << " Error code: " << errorCode << ".";
            m_fullMessage = ss.str();
        }

        int getErrorCode() const noexcept
        {
            return m_errorCode;
        }

        virtual const char *what() const noexcept override
        {
            return m_fullMessage.c_str();
        }

    protected:
        int m_errorCode;

    private:
        std::string m_fullMessage;
    };

    class FileNotFoundException : public BaseException
    {
    public:
        FileNotFoundException(
            const std::string &message = "File not found",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };

    class InvalidJSONException : public BaseException
    {
    public:
        InvalidJSONException(
            const std::string &message = "JSON parsing error",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };

    class SocketCreationException : public BaseException
    {
    public:
        SocketCreationException(
            const std::string &message = "Error when creating socket",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };

    class SocketCloseException : public BaseException
    {
    public:
        SocketCloseException(
            const std::string &message = "Error closing socket",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };

    class SocketBindingException : public BaseException
    {
    public:
        SocketBindingException(
            const std::string &message = "Error binding socket",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };

    class SocketListenException : public BaseException
    {
    public:
        SocketListenException(
            const std::string &message = "Error listening to connections",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };

    class SocketAcceptException : public BaseException
    {
    public:
        SocketAcceptException(
            const std::string &message = "Error accepting connections",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };

    class SocketReceiveException : public BaseException
    {
    public:
        SocketReceiveException(
            const std::string &message = "Error receiving data",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };

    class SocketSendException : public BaseException
    {
    public:
        SocketSendException(
            const std::string &message = "Error sending data",
            int errorCode = -1)
            : BaseException(message, errorCode) {}
    };
}

#endif
