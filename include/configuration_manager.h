#pragma once
#include "types.h"
#include "logger.h"
#include "events.h"

#include <ctime>
#include <unordered_map>
#include <optional>
#include <thread>
#include <type_traits>

namespace FastCopy
{
    class ConfigurationManager : public ParserEvents
    {
    public:
        ConfigurationManager();
        virtual void OnHelp() override;
        virtual void OnPath(const PathType &type, const param_t &path) override;
        virtual void OnOperationMode(const param_t &mode) override;
        virtual void OnAdditionalFileExtension(const param_t &extension) override;
        virtual void OnThreadsNumber(const param_t &threads) override;
        virtual void OnTimeBackup(const param_t &backupTime) override;
        virtual void OnFileNameMask(const param_t &mask) override;
        virtual void OnLogLevel(const param_t &level) override;
        virtual void OnQueueSize(const param_t &queueSize) override;

        fs::path GetPath(const PathType &type) const;
        OperatingMode GetOperationMode() const;
        size_t GetThreadsCount() const;
        std::time_t GetTimeBackup() const;
        std::string GetFileNameMask() const;
        std::string GetAdditionalFileExtension() const;
        LogLevel GetLogLevel() const;
        bool GetHelp() const;
        size_t GetQueueSize() const;

        bool CheckConfiguration();

    private:
        void CheckThreadsNumber();

        template <typename Map>
        typename Map::mapped_type ConvertType(const Map &map, const typename Map::key_type &key)
        {
            auto it = map.find(key);
            if (it != map.cend())
                return it->second;
            return Map::mapped_type::UNDEFINED;
        }

        template <typename T>
        void ConvertType(const param_t &str, T &t)
        {
            std::stringstream iss(str);
            iss >> t;
        }

    private:
        bool m_help;
        OperatingMode m_mode;
        std::time_t m_backupTime;
        size_t m_threads;
        std::string m_additionalExtension;
        param_t m_fileNameMask;
        LogLevel m_logLevel;
        size_t m_queueSize;
        std::unordered_map<PathType, fs::path> m_paths;

    private:
        const std::unordered_map<param_t, OperatingMode> m_opMode{
            {"regular", OperatingMode::REGULAR},
            {"backup", OperatingMode::BACKUP}};

        // all msg in file
        // you can choose what you want in console: warnings (+errors), only errors or all
        const std::unordered_map<param_t, LogLevel> m_lgLev{
            {"error", LogLevel::ERR},
            {"warn", LogLevel::WARN},
            {"info", LogLevel::INFO}};
    };
} // !FastCopy