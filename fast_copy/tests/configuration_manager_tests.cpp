#include "../include/cli_parser.h"
#include "../include/configuration_manager.h"
#include "../include/logger.h"
#include "../include/configuration_manager_mock.h"
#include "../include/helper_functions.h"

#include "gtest/gtest.h"

#include <limits>
#include <type_traits>
#include <cstring>
#include <thread>

using namespace FastCopy;
using namespace helper_functions;

// To run tests you need to turn off LOG_ENABLED define in logger.h
//////////////////EMPTY SOURCE & DEST///////////////////
TEST(ConfigurationManagerTest, SourceAndDestinationEmpty)
{
    auto f = []()
    {
        Utils::LogInit();
        auto configuration = std::make_shared<ConfigurationManager>();
        std::vector<std::string> v{ fast_copy_ar };
        auto argv = ConvertToArgv(v);
        CommandLineParser parser(argv.size() - 1, argv.data());
        parser.Subscribe(configuration);
        parser.Parse();
        configuration->CheckConfiguration();
    };
    EXPECT_ANY_THROW(f());
}

////////////////INVALID PATHS/////////////////////////
TEST(ConfigurationManagerTest, SourceDirectoryIsInvalid)
{
    CreateDir(DESTINATION_DIR);
    auto f = []()
    {
        auto configuration = std::make_shared<ConfigurationManager>();
        std::vector<std::string> v{ fast_copy_ar, source, "/path/to/source", destination, DESTINATION_DIR };
        auto argv = ConvertToArgv(v);
        CommandLineParser parser(argv.size() - 1, argv.data());
        parser.Subscribe(configuration);
        parser.Parse();
        configuration->CheckConfiguration();
    };
    EXPECT_ANY_THROW(f());
}

TEST(ConfigurationManagerTest, DestinationDirectoryIsInvalid)
{
    auto f = []()
    {
        auto configuration = std::make_shared<ConfigurationManager>();
        std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, "/path/to/source" };
        auto argv = ConvertToArgv(v);
        CommandLineParser parser(argv.size() - 1, argv.data());
        parser.Subscribe(configuration);
        parser.Parse();
        configuration->CheckConfiguration();
    };
    EXPECT_ANY_THROW(f());
}

TEST(ConfigurationManagerTest, DirectoryWithSpacesInName)
{
    CreateDir(DESTINATION_DIR_WITH_SPACE);
    auto configuration = std::make_shared<ConfigurationManager>();
    std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR_WITH_SPACE };
    auto argv = ConvertToArgv(v);
    CommandLineParser parser(argv.size() - 1, argv.data());
    parser.Subscribe(configuration);
    parser.Parse();
    configuration->CheckConfiguration();
    EXPECT_EQ(configuration->GetPath(PathType::Destination), std::string(DESTINATION_DIR_WITH_SPACE));
}

//////////////////////////THREADS COUNT//////////////////////////
TEST(ConfigurationManagerTest, ThreadsCountMoreThanSupported_DefaultValue)
{
    CreateDir(DESTINATION_DIR);

    auto configuration = std::make_shared<ConfigurationManager>();
    std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR };
    auto argv = ConvertToArgv(v);
    CommandLineParser parser(argv.size() - 1, argv.data());
    parser.Subscribe(configuration);
    parser.Parse();
    configuration->CheckConfiguration();
    EXPECT_EQ(configuration->GetThreadsCount(), std::jthread::hardware_concurrency());
}

TEST(ConfigurationManagerTest, ThreadsCountIsNull_DefaultValue)
{
    CreateDir(DESTINATION_DIR);

    auto configuration = std::make_shared<ConfigurationManager>();
    std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR, threads, "0" };
    auto argv = ConvertToArgv(v);
    CommandLineParser parser(argv.size() - 1, argv.data());
    parser.Subscribe(configuration);
    parser.Parse();
    configuration->CheckConfiguration();
    EXPECT_EQ(configuration->GetThreadsCount(), std::jthread::hardware_concurrency());
}
//////////////////////////OPTION PARAMETER IS INVALID////////////////////
TEST(ConfigurationManagerTest, LogLevelIsInvalid)
{
    CreateDir(DESTINATION_DIR);
    auto f = []()
    {
        auto configuration = std::make_shared<ConfigurationManager>();
        std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR, log_level, "aaaa" };
        auto argv = ConvertToArgv(v);
        CommandLineParser parser(argv.size() - 1, argv.data());
        parser.Subscribe(configuration);
        parser.Parse();
        configuration->CheckConfiguration();
    };
    EXPECT_ANY_THROW(f());
}

TEST(ConfigurationManagerTest, OperationModeIsInvalid)
{
    CreateDir(DESTINATION_DIR);
    auto f = []()
    {
        auto configuration = std::make_shared<ConfigurationManager>();
        std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR, mode, "aaaa" };
        auto argv = ConvertToArgv(v);
        CommandLineParser parser(argv.size() - 1, argv.data()); 
        parser.Subscribe(configuration);
        parser.Parse();
        configuration->CheckConfiguration();
    };
    EXPECT_ANY_THROW(f());
}