
#ifndef CONSOLE_LOGGER_H_
#define CONSOLE_LOGGER_H_

#include "logger.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>

namespace tpt
{
    class ConsoleLogger : public ILogger
    {
    public:
        virtual void log(const std::string &message, LogLevel level, const char *file = nullptr, int line = 0) override;
        virtual void debug(const std::string &message) override;
        virtual void info(const std::string &message) override;
        virtual void warning(const std::string &message) override;
        virtual void error(const std::string &message) override;
        virtual void critical(const std::string &message) override;
    };
}

#endif