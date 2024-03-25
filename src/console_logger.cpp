#include "../include/console_logger.hpp"

using namespace tpt;

void ConsoleLogger::log(const std::string &message, LogLevel level, const char *file = nullptr, int line = 0)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // You might want to adjust the time format according to your needs
    std::tm now_tm = *std::localtime(&now_time);

    std::cout << "[" << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "] ";
    std::cout << "[" << static_cast<int>(level) << "] ";

    if (file)
    {
        std::cout << "[" << file << ":" << line << "] ";
    }

    std::cout << message << std::endl;
}
void ConsoleLogger::debug(const std::string &message)
{
    log(message, LogLevel::DEBUG);
}

void ConsoleLogger::info(const std::string &message)
{
    log(message, LogLevel::INFO);
}

void ConsoleLogger::warning(const std::string &message)
{
    log(message, LogLevel::WARNING);
}

void ConsoleLogger::error(const std::string &message)
{
    log(message, LogLevel::ERROR);
}

void ConsoleLogger::critical(const std::string &message)
{
    log(message, LogLevel::CRITICAL);
}

// Helper Macros
#define LOG(logger, level, message) (logger).log(message, level, __FILE__, __LINE__)
#define LOG_DEBUG(logger, message) LOG(logger, tpt::LogLevel::DEBUG, message)
#define LOG_INFO(logger, message) LOG(logger, tpt::LogLevel::INFO, message)
#define LOG_WARNING(logger, message) LOG(logger, tpt::LogLevel::WARNING, message)
#define LOG_ERROR(logger, message) LOG(logger, tpt::LogLevel::ERROR, message)
#define LOG_CRITICAL(logger, message) LOG(logger, tpt::LogLevel::CRITICAL, message)
