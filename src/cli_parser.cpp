#include "cli_parser.h"
#include "settings_parser.h"
#include "ini_parser.h"
#include "json_parser.h"
#include "logger.h"

#include <exception>
#include <ctime>
#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <iostream>

namespace fs = std::filesystem;

namespace FastCopy
{
    CommandLineParser::CommandLineParser(int argc, char **argv)
    {
        for (int i = 1; i < argc; ++i)
            tokens_.emplace_back(argv[i]);
    }

    void CommandLineParser::Parse() const
    {
        if (HasOptionHelp())
            return;
        HasOptionConfig();
        HasOptionPath("--source", PathType::Source);
        HasOptionPath("--destination", PathType::Destination);
        HasOptionMode();
        HasOptionThreads();
        HasOptionMask();
        HasOptionAddExt();
        HasOptionLogLevel();
        HasOptionPeriodicity();
        HasOptionQueue();
    }

    bool CommandLineParser::HasOptionHelp() const
    {
        auto itOption = std::find(tokens_.cbegin(), tokens_.cend(), "--help");
        if (itOption != tokens_.cend())
        {
            std::cerr << "Usage: cp [OPTION]... --source SOURCE --destination DEST\n"
                      << "Copy SOURCE to DEST.\n\n"

                      << "Mandatory options:\n"
                      << "-s, --source /path/to/source/directory\n"
                      << "-d, --destination /path/to/source/directory\n\n"

                      << "Nonmandatory options:\n"
                      << std::left << std::setw(50) << "-c, --config /path/to/configuration/file.json "
                      << "the utility can parse .json and .ini.\n"

                      << std::left << std::setw(50) << "--mode [MODE] "
                      << "supports two mode: regular and backup. Default is regular.\n"

                      << std::left << std::setw(50) << "-t, --threads [THREADS NUMBER] "
                      << "the default value is the number of concurrent threads supported by the implementation.\n"

                      << std::left << std::setw(50) << "-n, --mask [FILENAME MASK] "
                      << "filter files with mask using regex. The default value is \".\"\n"

                      << std::left << std::setw(50) << "-a, --addext [ADDITIONAL EXTENSION] "
                      << "adding extension to copied files. The default value is \"\"\n"

                      << std::left << std::setw(50) << "-l, --loglevel [LOG LEVEL] "
                      << "supports three log levels of console: info, warning and error. The default log level of file is info. The default log level of console is error.\n"

                      << std::left << std::setw(50) << "-p, --periodicity [PERIODICITY TIME] "
                      << "takes parameter in seconds. The default value is 60.\n"

                      << std::left << std::setw(50) << "-q, --queue [QUEUE SIZE] "
                      << "queue size. The default value is 100.\n";

            std::ranges::for_each(m_Handlers, [](auto &spHandler)
                                  { spHandler->OnHelp(); });
            return true;
        }
        return false;
    }

    void CommandLineParser::HasOptionConfig() const
    {
        auto config = OptionExists(*parameters_.find("--config"));
        if (config.has_value())
        {
            if (!fs::is_regular_file(config.value()))
            {
                const std::string msg = "The path to configuration file is invalid.";
                LOG_E(msg);
                throw std::runtime_error(msg);
            }
            else
            {
                fs::path extension = fs::path(config.value()).extension();

                std::shared_ptr<Utils::SettingsParser> settingsParser;

                if (extension == ".ini" || extension == ".config")
                    settingsParser = std::make_shared<Utils::IniParser>(config.value());
                else if (extension == ".json")
                    settingsParser = std::make_shared<Utils::JsonParser>(config.value());

                for (auto &spHandler : m_Handlers)
                    settingsParser->Subscribe(spHandler);

                settingsParser->Parse();
            }
        }
    }

    void CommandLineParser::HasOptionPath(const param_t &token, const PathType type) const
    {
        auto path = OptionExists(*parameters_.find(token));
        if (path.has_value())
            std::ranges::for_each(m_Handlers, [&path, &type](auto &spHandler)
                                  { spHandler->OnPath(type, path.value()); });
    }

    void CommandLineParser::HasOptionMask() const
    {
        auto mask = OptionExists(*parameters_.find("--mask"));
        if (mask.has_value())
            std::ranges::for_each(m_Handlers, [&mask](auto &spHandler)
                                  { spHandler->OnFileNameMask(mask.value()); });
    }

    void CommandLineParser::HasOptionAddExt() const
    {
        auto addExt = OptionExists(*parameters_.find("--addext"));
        if (addExt.has_value())
            std::ranges::for_each(m_Handlers, [&addExt](auto &spHandler)
                                  { spHandler->OnAdditionalFileExtension(addExt.value()); });
    }

    void CommandLineParser::HasOptionLogLevel() const
    {
        auto logLevel = OptionExists(*parameters_.find("--loglevel"));
        if (logLevel.has_value())
            std::ranges::for_each(m_Handlers, [&logLevel, this](auto &spHandler)
                                  { spHandler->OnLogLevel(logLevel.value()); });
    }

    void CommandLineParser::HasOptionPeriodicity() const
    {
        auto periodicity = OptionExists(*parameters_.find("--periodicity"));
        if (periodicity.has_value())
            std::ranges::for_each(m_Handlers, [&periodicity](auto &spHandler)
                                  { spHandler->OnTimeBackup(periodicity.value()); });
    }

    void CommandLineParser::HasOptionThreads() const
    {
        auto threads = OptionExists(*parameters_.find("--threads"));
        if (threads.has_value())
            std::ranges::for_each(m_Handlers, [&threads](auto &spHandler)
                                  { spHandler->OnThreadsNumber(threads.value()); });
    }

    void CommandLineParser::HasOptionMode() const
    {
        auto mode = OptionExists(*parameters_.find("--mode"));
        if (mode.has_value())
            std::ranges::for_each(m_Handlers, [&mode, this](auto &spHandler)
                                  { spHandler->OnOperationMode(mode.value()); });
    }

    void CommandLineParser::HasOptionQueue() const
    {
        auto queue = OptionExists(*parameters_.find("--queue"));
        if (queue.has_value())
            std::ranges::for_each(m_Handlers, [&queue, this](auto &spHandler)
                                  { spHandler->OnQueueSize(queue.value()); });
    }

    std::optional<param_t> CommandLineParser::OptionExists(const std::pair<param_t, param_t> &option) const
    {
        auto itOptionLong = std::find(tokens_.cbegin(), tokens_.cend(), option.first);
        auto itOptionShort = std::find(tokens_.cbegin(), tokens_.cend(), option.second);

        if (itOptionLong == tokens_.cend() && itOptionShort == tokens_.cend())
            return {};

        if (itOptionLong != tokens_.cend() && ++itOptionLong != tokens_.cend())
            return *itOptionLong;

        if (itOptionShort != tokens_.cend() && ++itOptionShort != tokens_.cend())
            return *itOptionShort;

        const std::string msg = "The option parameter is missing.";
        LOG_E(msg);
        throw std::runtime_error(msg);
    }
} // !FastCopy