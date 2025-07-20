#pragma once

#include <cstdarg>
#include <cstring>

namespace Logger
{
    /// Get just the filename
    inline const char *getFilename(const char *path)
    {
        const char *slash = std::strrchr(path, '/');
        const char *back = std::strrchr(path, '\\');
        const char *ptr = (slash > back ? slash : back);
        return ptr ? ptr + 1 : path;
    }
} // namespace Logger

#if defined(__GNUC__) || defined(__clang__)
#define LOGGER_FUNC __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define LOGGER_FUNC __FUNCSIG__
#else
#define LOGGER_FUNC __func__
#endif

#define LOG_ERROR(fmt, ...)                                                   \
    do                                                                        \
    {                                                                         \
        std::fprintf(stderr,                                                  \
                     "[ERROR] %s:%d: " fmt "\n",                              \
                     Logger::getFilename(__FILE__), __LINE__, ##__VA_ARGS__); \
    } while (false)

#define LOG_INFO(fmt, ...)                                                    \
    do                                                                        \
    {                                                                         \
        std::fprintf(stdout,                                                  \
                     "[INFO] %s:%d: " fmt "\n",                               \
                     Logger::getFilename(__FILE__), __LINE__, ##__VA_ARGS__); \
    } while (false)

#define LOG_DEBUG(fmt, ...)                                                   \
    do                                                                        \
    {                                                                         \
        std::fprintf(stdout,                                                  \
                     "[DEBUG] %s:%d: " fmt "\n",                              \
                     Logger::getFilename(__FILE__), __LINE__, ##__VA_ARGS__); \
    } while (false)