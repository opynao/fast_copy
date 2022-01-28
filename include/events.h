#pragma once

#include "types.h"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace FastCopy
{
    class ParserEvents
    {
    public:
        using param_t = std::string;
        virtual ~ParserEvents() = default;
        virtual void OnHelp() = 0;
        virtual void OnPath(const PathType &type, const param_t &path) = 0;
        virtual void OnOperationMode(const param_t &mode) = 0;
        virtual void OnAdditionalFileExtension(const param_t &extension) = 0;
        virtual void OnThreadsNumber(const param_t &threads) = 0;
        virtual void OnTimeBackup(const param_t &backupTime) = 0;
        virtual void OnFileNameMask(const param_t &mask) = 0;
        virtual void OnLogLevel(const param_t &level) = 0;
        virtual void OnQueueSize(const param_t &level) = 0;
    };
} // !FastCopy
