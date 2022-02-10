#include "../include/cli_parser.h"
#include "../include/configuration_manager.h"
#include "../include/logger.h"
#include "../include/settings_parser.h"
#include "../include/configuration_manager_mock.h"
#include "../include/helper_functions.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <limits>
#include <type_traits>
#include <cstring>
#include <fstream>

using namespace FastCopy;
using namespace helper_functions;

// To run tests you need to turn off LOG_ENABLED define in logger.h
// And to comment out LogInit in fast_copy.cpp
TEST(IniSettingsParserTest, EventsGenerating)
{
    const std::string str = "source = /mnt/d/task/programming_books/\n"
                            "destination = /mnt/d/task/test/\n"
                            "mode = regular\n"
                            "periodicity = 1000\n"
                            "threads = 6\n"
                            "addext = .swp\n"
                            "mask = .\n"
                            "loglevel = info\n";
    CreateF(str, CONFIG_INI);

    auto configuration = std::make_shared<ConfigurationManagerMock>();

    EXPECT_CALL(*configuration, OnPath(PathType::Source, "/mnt/d/task/programming_books/"));
    EXPECT_CALL(*configuration, OnPath(PathType::Destination, "/mnt/d/task/test/"));
    EXPECT_CALL(*configuration, OnOperationMode("regular"));
    EXPECT_CALL(*configuration, OnThreadsNumber("6"));
    EXPECT_CALL(*configuration, OnAdditionalFileExtension(".swp"));
    EXPECT_CALL(*configuration, OnFileNameMask("."));
    EXPECT_CALL(*configuration, OnTimeBackup("1000"));
    EXPECT_CALL(*configuration, OnLogLevel("info"));

    char *ar[] = {fast_copy_ar, config, CONFIG_INI};
    CommandLineParser parser(sizeof(ar) / sizeof(ar[0]), ar);
    parser.Subscribe(configuration);
    parser.Parse();
}

TEST(JsonSettingsParserTest, EventsGenerating)
{
#if defined(_WIN32)
    const std::string str = "{\n\"source\" : \"D:\\task\\programming_books\\\",\n"
                            "\"destination\" : \"D:\\task\\test\\\",\n"
                            "\"mode\" : \"regular\",\n"
                            "\"periodicity\" : \"1000\",\n"
                            "\"threads\" : \"6\",\n"
                            "\"addext\" : \".swp\",\n"
                            "\"mask\" : \".\",\n"
                            "\"loglevel\" : \"info\"\n}";
#else
    const std::string str = "{\n\"source\" : \"/mnt/d/task/programming_books/\",\n"
        "\"destination\" : \"/mnt/d/task/test/\",\n"
        "\"mode\" : \"regular\",\n"
        "\"periodicity\" : \"1000\",\n"
        "\"threads\" : \"6\",\n"
        "\"addext\" : \".swp\",\n"
        "\"mask\" : \".\",\n"
        "\"loglevel\" : \"info\"\n}";
#endif
    CreateF(str, CONFIG_JSON);

    auto configuration = std::make_shared<ConfigurationManagerMock>();
#if defined(_WIN32)
    EXPECT_CALL(*configuration, OnPath(PathType::Source, "D:\\task\\programming_books\\"));
    EXPECT_CALL(*configuration, OnPath(PathType::Destination, "D:\\task\\test\\"));
#else
    EXPECT_CALL(*configuration, OnPath(PathType::Source, "/mnt/d/task/programming_books/"));
    EXPECT_CALL(*configuration, OnPath(PathType::Destination, "/mnt/d/task/test/"));
#endif
    EXPECT_CALL(*configuration, OnOperationMode("regular"));
    EXPECT_CALL(*configuration, OnThreadsNumber("6"));
    EXPECT_CALL(*configuration, OnAdditionalFileExtension(".swp"));
    EXPECT_CALL(*configuration, OnFileNameMask("."));
    EXPECT_CALL(*configuration, OnTimeBackup("1000"));
    EXPECT_CALL(*configuration, OnLogLevel("info"));

    char *ar[] = {fast_copy_ar, config, CONFIG_JSON};
    CommandLineParser parser(sizeof(ar) / sizeof(ar[0]), ar);
    parser.Subscribe(configuration);
    parser.Parse();
}