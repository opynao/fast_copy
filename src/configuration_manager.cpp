#include "configuration_manager.h"
#include "logger.h"

#include <thread>

namespace FastCopy
{
    ConfigurationManager::ConfigurationManager()
        : m_help{false},
          m_mode{OperatingMode::REGULAR},
          m_backupTime{60},
          m_threads{std::jthread::hardware_concurrency()},
          m_additionalExtension{""},
          m_fileNameMask{"."},
          m_logLevel{LogLevel::ERR},
          m_queueSize{100}
    {
    }

    bool ConfigurationManager::CheckConfiguration()
    {
        // Working directories exist
        if (m_paths.size() < 2)
        {
            const std::string msg = "The source and destination directories are necessary options "
                                    "and must be presented in the command line arguments or in the configuration file. "
                                    "Try 'fast_copy --help' for more information.";
            LOG_E(msg);
            throw std::runtime_error(msg);
        }
        // Paths to working directories are correct
        if (!fs::exists(m_paths.at(PathType::Source)) || !fs::exists(m_paths.at(PathType::Destination)))
        {
            const std::string msg = "The source or destination directory is invalid. "
                                    "Please, check the path. ";
            LOG_E(msg);
            throw std::runtime_error(msg);
        }
        // Operating mode and log level have correct options
        if (m_mode == OperatingMode::UNDEFINED || m_logLevel == LogLevel::UNDEFINED)
        {
            const std::string msg = "One of the option parameter is invalid. Please, check. "
                                    "Try 'fast_copy --help' for more information.";
            LOG_E(msg);
            throw std::runtime_error(msg);
        }
        // Threads number isn't more than supported by implementation
        if (m_threads > std::jthread::hardware_concurrency())
        {
            m_threads = std::jthread::hardware_concurrency();
            LOG_W("The threads number is too large. "
                  "The default value will be the the number"
                  " of concurrent threads supported by the implementation: {}",
                  std::to_string(m_threads));
        }
        // Threads number isn't null
        if (m_threads == 0)
        {
            m_threads = std::jthread::hardware_concurrency();
            LOG_W("The threads number is too large. "
                  "The default value will be the the number"
                  " of concurrent threads supported by the implementation: {}",
                  std::to_string(m_threads));
        }

        return true;
    }

    //////////////////////////// ON EVENT ////////////////////////////////
    void ConfigurationManager::OnHelp()
    {
        m_help = true;
    }
    void ConfigurationManager::OnPath(const PathType &type, const param_t &path)
    {
        m_paths[type] = fs::path(path);
    }
    void ConfigurationManager::OnOperationMode(const param_t &mode)
    {
        m_mode = ConvertType(m_opMode, mode);
    }
    void ConfigurationManager::OnThreadsNumber(const param_t &threads)
    {
        ConvertType(threads, m_threads);
    }
    void ConfigurationManager::OnTimeBackup(const param_t &backupTime)
    {
        ConvertType(backupTime, m_backupTime);
    }
    void ConfigurationManager::OnFileNameMask(const param_t &mask)
    {
        m_fileNameMask = mask;
    }
    void ConfigurationManager::OnLogLevel(const param_t &level)
    {
        m_logLevel = ConvertType(m_lgLev, level);
    }
    void ConfigurationManager::OnAdditionalFileExtension(const param_t &extension)
    {
        m_additionalExtension = extension;
    }
    void ConfigurationManager::OnQueueSize(const param_t &queueSize)
    {
        ConvertType(queueSize, m_queueSize);
    }

    //////////////////////////// ON_EVENT /////////////////////

    //////////////////////////// GET ////////////////////////////////
    bool ConfigurationManager::GetHelp() const
    {
        return m_help;
    }
    fs::path ConfigurationManager::GetPath(const PathType &type) const
    {
        auto it = m_paths.find(type);
        if (it != m_paths.cend())
            return it->second;
        return {};
    }
    OperatingMode ConfigurationManager::GetOperationMode() const
    {
        return m_mode;
    }
    size_t ConfigurationManager::GetThreadsCount() const
    {
        return m_threads;
    }
    std::time_t ConfigurationManager::GetTimeBackup() const
    {
        return m_backupTime;
    }
    std::string ConfigurationManager::GetFileNameMask() const
    {
        return m_fileNameMask;
    }
    std::string ConfigurationManager::GetAdditionalFileExtension() const
    {
        return m_additionalExtension;
    }
    LogLevel ConfigurationManager::GetLogLevel() const
    {
        return m_logLevel;
    }
    size_t ConfigurationManager::GetQueueSize() const
    {
        return m_queueSize;
    }

    //////////////////////////// GET /////////////////////
} // !FastCopy