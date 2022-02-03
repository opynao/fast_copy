#pragma once
#include "configuration_manager.h"
#include "cli_parser.h"

#include <memory>

namespace FastCopy
{
    std::shared_ptr<ConfigurationManager> InitializeConfig(int argc, char *argv[]);
} //! FastCopy
