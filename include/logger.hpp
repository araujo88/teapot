#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>

namespace tpt
{

    // Define the LogLevel enum to specify the severity of the log messages.
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    // ILogger interface with virtual methods for logging messages at different severity levels.
    class ILogger
    {
    public:
        virtual ~ILogger() = default; // Virtual destructor for proper cleanup of derived classes

        // Pure virtual functions for logging messages at different severity levels
        virtual void log(const std::string &message, LogLevel level, const char *file = nullptr, int line = 0) = 0;
        virtual void debug(const std::string &message) = 0;
        virtual void info(const std::string &message) = 0;
        virtual void warning(const std::string &message) = 0;
        virtual void error(const std::string &message) = 0;
        virtual void critical(const std::string &message) = 0;

        // Additional virtual methods can be added here as needed
    };

} // namespace tpt

#endif // LOGGER_H_
