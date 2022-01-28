#include "json_parser.h"
#include "logger.h"

#include <fstream>
#include <algorithm>

namespace Utils
{
    JsonParser::JsonParser(const fs::path &pathToFile)
        : m_pathToFile{pathToFile}
    {
    }

    void JsonParser::Parse()
    {
        std::ifstream configFile(m_pathToFile);

        for (std::string line; std::getline(configFile, line);)
        {
            if (line.empty() || line == "{" || line == "}")
                continue;

            size_t delimiter = line.find(':');

            std::string parameter = line.substr(0, delimiter);
            std::string value = line.substr(delimiter + 1, std::string::npos);

            RemoveQuotes(parameter);
            RemoveQuotes(value);
            
            sections[parameter] = value;

            if (parameter == "source")
            {
                std::ranges::for_each(m_Handlers, [&value](auto &spHandler)
                                      { spHandler->OnPath(FastCopy::PathType::Source, value); });
            }
            else if (parameter == "destination")
            {
                std::ranges::for_each(m_Handlers, [&value](auto &spHandler)
                                      { spHandler->OnPath(FastCopy::PathType::Destination, value); });
            }
            else if (parameter == "threads")
            {
                std::ranges::for_each(m_Handlers, [&value](auto &spHandler)
                                      { spHandler->OnThreadsNumber(value); });
            }
            else if (parameter == "mode")
            {
                std::ranges::for_each(m_Handlers, [&value](auto &spHandler)
                                      { spHandler->OnOperationMode(value); });
            }
            else if (parameter == "periodicity")
            {
                std::ranges::for_each(m_Handlers, [&value, this](auto &spHandler)
                                      { spHandler->OnTimeBackup(value); });
            }
            else if (parameter == "mask")
            {
                std::ranges::for_each(m_Handlers, [&value](auto &spHandler)
                                      { spHandler->OnFileNameMask(value); });
            }
            else if (parameter == "loglevel")
            {
                std::ranges::for_each(m_Handlers, [&value](auto &spHandler)
                                      { spHandler->OnLogLevel(value); });
            }
            else if (parameter == "addext")
            {
                std::ranges::for_each(m_Handlers, [&value](auto &spHandler)
                                      { spHandler->OnAdditionalFileExtension(value); });
            }
        }
    }

    void JsonParser::RemoveQuotes(param_t &str) const
    {
        str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
        str.erase(std::remove(str.begin(), str.end(), ','), str.end());
        str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    }
} //! Utils