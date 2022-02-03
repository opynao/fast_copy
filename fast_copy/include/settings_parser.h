#pragma once

#include "publisher.h"

#include <string>
#include <filesystem>
#include <memory>

namespace fs = std::filesystem;
namespace Utils
{
    class SettingsParser : public Publisher<FastCopy::ParserEvents>
    {
    public:
        virtual void Parse() = 0;
    };
} // !Utils