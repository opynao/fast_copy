#include "config_initialize.h"

namespace FastCopy
{
    std::shared_ptr<ConfigurationManager> InitializeConfig(int argc, char *argv[])
    {
        auto config = std::make_shared<ConfigurationManager>();
        CommandLineParser parser(argc, argv);
        parser.Subscribe(config);
        parser.Parse();
        return config;
    }
} //! FastCopy
