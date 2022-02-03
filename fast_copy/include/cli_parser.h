#pragma once

#include "configuration_manager.h"
#include "logger.h"
#include "publisher.h"
#include "events.h"
#include "types.h"

#include <vector>
#include <string>
#include <map>
#include <memory>
#include <algorithm>
#include <optional>
#include <unordered_map>

namespace FastCopy
{
    class CommandLineParser : public Publisher<ParserEvents>
    {
    public:
        CommandLineParser(int argc, char **argv);
        void Parse() const;

        CommandLineParser(const CommandLineParser &) = delete;
        CommandLineParser &operator=(const CommandLineParser &) = delete;
        CommandLineParser(CommandLineParser &&) = delete;
        CommandLineParser &operator=(CommandLineParser &&) = delete;
        ~CommandLineParser() = default;

    private:
        std::optional<param_t> OptionExists(const std::pair<param_t, param_t> &) const;
        bool HasOptionHelp() const;
        void HasOptionConfig() const;
        void HasOptionMode() const;
        void HasOptionThreads() const;
        void HasOptionMask() const;
        void HasOptionAddExt() const;
        void HasOptionLogLevel() const;
        void HasOptionPeriodicity() const;
        void HasOptionPath(const param_t &, const PathType) const;
        void HasOptionQueue() const;

    private:
        std::vector<param_t> tokens_;
        const std::unordered_map<param_t, param_t> parameters_{
            {"--config", "-c"},
            {"--source", "-s"},
            {"--destination", "-d"},
            {"--mode", "-m"},
            {"--threads", "-t"},
            {"--mask", "-n"},
            {"--addext", "-a"},
            {"--loglevel", "-l"},
            {"--periodicity", "-p"},
            {"--queue", "-q"}
        };
    };
} // !FastCopy