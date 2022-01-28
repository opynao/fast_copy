#include "logger.h"
#include "string_conversion.h"

#include <map>
#include <csignal>

namespace Utils
{
    const std::map<FastCopy::OperatingMode, std::string> opMode{
        {FastCopy::OperatingMode::REGULAR, "regular"},
        {FastCopy::OperatingMode::BACKUP, "backup"}};

    const std::map<FastCopy::LogLevel, spdlog::level::level_enum> logLevel{
        {FastCopy::LogLevel::ERR, spdlog::level::level_enum::err},
        {FastCopy::LogLevel::WARN, spdlog::level::level_enum::warn},
        {FastCopy::LogLevel::INFO, spdlog::level::level_enum::info}};

    const std::map<FastCopy::LogLevel, std::string> logLevelStr{
        {FastCopy::LogLevel::ERR, "error"},
        {FastCopy::LogLevel::WARN, "warn"},
        {FastCopy::LogLevel::INFO, "info"}};

    void SignalHandler(int signal)
    {
        spdlog::shutdown();
        exit(signal);
    }

    void LogInit()
    {
        // if (fs::exists("logs/"))
        //     fs::remove_all("logs/");

        spdlog::init_thread_pool(8192, 1); // queue with 8k items and 1 backing thread.

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(spdlog::level::warn);

        const int max_size = 1024 * 1024 * 5; // 5 mb
        const int max_files = 1000;
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/log.txt", max_size, max_files, true);
        file_sink->set_level(spdlog::level::info);

        const std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
        auto logger = std::make_shared<spdlog::async_logger>("logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

#if defined LOG_ENABLED
        logger->set_level(spdlog::level::info);
#else
        logger->set_level(spdlog::level::off);
#endif
        spdlog::register_logger(logger);
        
        logger->flush_on(spdlog::level::info);
        // handler for Ctrl-C, all msgs will be than in logs
        // otherwise we get not full msgs
        std::signal(SIGINT, SignalHandler);
    }

    void SetLogLevel(const FastCopy::LogLevel &level)
    {
        spdlog::get("logger")->sinks()[0]->set_level(logLevel.at(level));
    }

    void LogConfiguration(const std::shared_ptr<::FastCopy::ConfigurationManager> &config)
    {
        LOG_I("Files are going to be copied with following settings:");
        LOG_I("The source directory is: {}", ConvertWstringToString(String(config->GetPath(FastCopy::PathType::Source))));
        LOG_I("The destination directory is: {}", ConvertWstringToString(String(config->GetPath(FastCopy::PathType::Destination))));
        LOG_I("The operating mode is: {}", opMode.at(config->GetOperationMode()));

        if (config->GetOperationMode() == FastCopy::OperatingMode::BACKUP)
            LOG_I("The periodicity of backup: {}", config->GetTimeBackup());

        LOG_I("Number of concurrent threads is: {}", config->GetThreadsCount());
        LOG_I("File additional extension is: \"{}\"", config->GetAdditionalFileExtension());
        LOG_I("Filename mask is: \"{}\"", config->GetFileNameMask());
        LOG_I("Log level is: {}", logLevelStr.at(config->GetLogLevel()));
    }

    void LogError(const std::string &msg)
    {
        spdlog::get("logger")->error(msg);
#if defined LOG_ENABLED
        spdlog::shutdown();
#endif
    }

    void FlushLog()
    {
#if defined LOG_ENABLED
        spdlog::shutdown();
#endif
    }
} // !Utils