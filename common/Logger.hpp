// Logger.hpp
#pragma once

#include <iostream>
#include <mutex>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <cstring>

namespace Logger
{
    /// Returns just the filename portion of a full path.
    inline const char *getFilename(const char *path)
    {
        const char *slash = std::strrchr(path, '/');
        const char *back = std::strrchr(path, '\\');
        const char *ptr = (slash > back ? slash : back);
        return ptr ? ptr + 1 : path;
    }

    enum class Level
    {
        Error,
        Info,
        Debug
    };

    // Convert enum ? text
    inline const char *levelToString(Level lvl)
    {
        switch (lvl)
        {
        case Level::Error:
            return "ERROR";
        case Level::Info:
            return "INFO";
        case Level::Debug:
            return "DEBUG";
        }
        return "UNKNOWN";
    }

    class LoggerImpl
    {
    public:
        static LoggerImpl &instance()
        {
            static LoggerImpl s_instance;
            return s_instance;
        }

        // core log function: formats the user msg and prints with func name
        void log(Level level,
                 const char *func,
                 const char *fmt,
                 ...)
        {
            std::lock_guard<std::mutex> lock(m_mutex);

            // format user message
            char message_buf[1024];
            va_list args;
            va_start(args, fmt);
            std::vsnprintf(message_buf, sizeof(message_buf), fmt, args);
            va_end(args);

            // emit: [LEVEL] func(): message
            std::cout
                << "[" << levelToString(level) << "] "
                << func << "() "
                << message_buf
                << "\n";
        }

    private:
        LoggerImpl() = default;
        ~LoggerImpl() = default;
        LoggerImpl(const LoggerImpl &) = delete;
        LoggerImpl &operator=(const LoggerImpl &) = delete;

        std::mutex m_mutex;
    };

} // namespace Logger

// pick the best function?signature macro
#if defined(__GNUC__) || defined(__clang__)
#define LOGGER_FUNC __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define LOGGER_FUNC __FUNCSIG__
#else
#define LOGGER_FUNC __func__
#endif

/// Logs an ERROR to stderr with filename:line context.
#define LOG_ERROR(fmt, ...)                                                   \
    do                                                                        \
    {                                                                         \
        std::fprintf(stderr,                                                  \
                     "[ERROR] %s:%d: " fmt "\n",                              \
                     Logger::getFilename(__FILE__), __LINE__, ##__VA_ARGS__); \
    } while (false)

/// Logs an INFO to stdout with filename:line context.
#define LOG_INFO(fmt, ...)                                                    \
    do                                                                        \
    {                                                                         \
        std::fprintf(stdout,                                                  \
                     "[INFO] %s:%d: " fmt "\n",                               \
                     Logger::getFilename(__FILE__), __LINE__, ##__VA_ARGS__); \
    } while (false)

/// Logs a DEBUG to stdout with filename:line context.
#define LOG_DEBUG(fmt, ...)                                                   \
    do                                                                        \
    {                                                                         \
        std::fprintf(stdout,                                                  \
                     "[DEBUG] %s:%d: " fmt "\n",                              \
                     Logger::getFilename(__FILE__), __LINE__, ##__VA_ARGS__); \
    } while (false)