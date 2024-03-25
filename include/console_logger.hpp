
#ifndef CONSOLE_LOGGER_H_
#define CONSOLE_LOGGER_H_

#include "logger.hpp"

namespace tpt
{
    class ConsoleLogger : public Logger
    {
    public:
        virtual void log(const std::string &message, tpt::LogLevel level, const char *file, int line) override;
    };
}

#endif