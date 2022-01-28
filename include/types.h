#pragma once
#include <string>

namespace FastCopy
{
    using param_t = std::string;

    enum class OperatingMode
    {
        UNDEFINED,
        REGULAR,
        BACKUP
    };

    enum class LogLevel
    {
        UNDEFINED,
        INFO,
        WARN,
        ERR
    };

    enum class PathType
    {
        Source,
        Destination
    };
} // !FastCopy