#pragma once

#include <settings_parser.h>

#include <unordered_map>

namespace Utils
{
    using param_t = std::string;

    class JsonParser : public SettingsParser
    {
    public:
        JsonParser(const fs::path &pathToFile);

        void Parse() override;

        auto GetSections() const { return sections; }

    private:
        void RemoveQuotes(param_t &parameter) const;

    private:
        std::unordered_map<std::string, std::string> sections;
        fs::path m_pathToFile;
    };
} //! Utils