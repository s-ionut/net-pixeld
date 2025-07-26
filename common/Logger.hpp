#pragma once

#include <cstdarg>
#include <cstring>
#include <cstdio>

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