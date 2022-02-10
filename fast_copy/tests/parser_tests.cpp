#include "../include/cli_parser.h"
#include "../include/configuration_manager.h"
#include "../include/logger.h"
#include "../include/configuration_manager_mock.h"
#include "../include/helper_functions.h"

#include "gtest/gtest.h"

#include <limits>
#include <type_traits>
#include <cstring>

using namespace FastCopy;
using namespace helper_functions;

// To run tests you need to turn off LOG_ENABLED define in logger.h
// And to comment out LogInit in fast_copy.cpp
/////////////////////CONFIG PATH INVALID///////////////////
TEST(CommandLineParserTest, ConfigFileInvalidPath)
{
	auto f = []()
	{
		auto configuration = std::make_shared<ConfigurationManager>();
		std::vector<std::string> v{ fast_copy_ar, config, "/path/to/source" };
		auto argv = ConvertToArgv(v);
		CommandLineParser parser(argv.size() - 1, argv.data());
		parser.Subscribe(configuration);
		parser.Parse();
		configuration->CheckConfiguration();
	};
	EXPECT_ANY_THROW(f());
}
///////////////////OPTIONS MISSING//////////////////////
TEST(CommandLineParserTest, ConfigOptionMissing)
{
	auto f = []()
	{
		auto configuration = std::make_shared<ConfigurationManager>();
		std::vector<std::string> v{ fast_copy_ar, config };
		auto argv = ConvertToArgv(v);
		CommandLineParser parser(argv.size() - 1, argv.data());
		parser.Subscribe(configuration);
		parser.Parse();
		configuration->CheckConfiguration();
	};
	EXPECT_ANY_THROW(f());
}

TEST(CommandLineParserTest, DestinationDirectoryPathMissing)
{
	auto f = []()
	{
		auto configuration = std::make_shared<ConfigurationManager>();
		std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination };
		auto argv = ConvertToArgv(v);
		CommandLineParser parser(argv.size() - 1, argv.data());
		parser.Subscribe(configuration);
		parser.Parse();
		configuration->CheckConfiguration();
	};
	EXPECT_ANY_THROW(f());
}

TEST(CommandLineParserTest, ModeOptionMissing)
{
	CreateDir(DESTINATION_DIR);

	auto f = []()
	{
		auto configuration = std::make_shared<ConfigurationManager>();
		std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR, mode };
		auto argv = ConvertToArgv(v);
		CommandLineParser parser(argv.size() - 1, argv.data());
		parser.Subscribe(configuration);
		parser.Parse();
		configuration->CheckConfiguration();
	};
	EXPECT_ANY_THROW(f());
}
//////////////////////////EVENTS CALLING/////////////////////
TEST(CommandLineParserTest, EventsCalling)
{
	CreateDir(DESTINATION_DIR);

	auto configuration = std::make_shared<ConfigurationManagerMock>();
	EXPECT_CALL(*configuration, OnPath(PathType::Source, std::string(SOURCE_DIR)));
	EXPECT_CALL(*configuration, OnPath(PathType::Destination, std::string(DESTINATION_DIR)));
	EXPECT_CALL(*configuration, OnOperationMode("regular"));
	EXPECT_CALL(*configuration, OnThreadsNumber("6"));
	EXPECT_CALL(*configuration, OnAdditionalFileExtension(".swp"));
	EXPECT_CALL(*configuration, OnTimeBackup("1000"));
	EXPECT_CALL(*configuration, OnFileNameMask("."));
	EXPECT_CALL(*configuration, OnLogLevel("info"));

	std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR,
				  mode, "regular", threads, "6", addext, ".swp", mask, ".",
				  "--periodicity", "1000", log_level, "info" };
	auto argv = ConvertToArgv(v);
	CommandLineParser parser(argv.size() - 1, argv.data());
	parser.Subscribe(configuration);
	parser.Parse();
}

TEST(CommandLineParserTest, ConfigurationFileAndCliArguments)
{
	CreateDir(DESTINATION_DIR);
#if defined(_WIN32)
	const std::string str = "source = D:\\task\\programming_books\\\n"
		"destination = D:\\task\\test\\\n"
		"mode = backup\n"
		"periodicity = 10\n"
		"threads = 100\n"
		"addext = .swp\n"
		"mask = .\n"
		"loglevel = error\n";
#else
	const std::string str = "source = /mnt/d/task/programming_books/\n"
		"destination = /mnt/d/task/test/\n"
		"mode = backup\n"
		"periodicity = 10\n"
		"threads = 100\n"
		"addext = .swp\n"
		"mask = .\n"
		"loglevel = error\n";
#endif
	CreateF(str, CONFIG_INI);

	auto configuration = std::make_shared<ConfigurationManagerMock>();
	EXPECT_CALL(*configuration, OnPath(PathType::Source, std::string(SOURCE_DIR))).Times(2);
	EXPECT_CALL(*configuration, OnPath(PathType::Destination, std::string(DESTINATION_DIR))).Times(2);
	EXPECT_CALL(*configuration, OnOperationMode("regular"));
	EXPECT_CALL(*configuration, OnOperationMode("backup"));
	EXPECT_CALL(*configuration, OnThreadsNumber("6"));
	EXPECT_CALL(*configuration, OnThreadsNumber("100"));
	EXPECT_CALL(*configuration, OnAdditionalFileExtension(".swp")).Times(2);
	EXPECT_CALL(*configuration, OnTimeBackup("1000"));
	EXPECT_CALL(*configuration, OnTimeBackup("10"));
	EXPECT_CALL(*configuration, OnFileNameMask(".")).Times(2);
	EXPECT_CALL(*configuration, OnLogLevel("info"));
	EXPECT_CALL(*configuration, OnLogLevel("error"));

	std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR,
				  mode, "regular", threads, "6", addext, ".swp", mask, ".",
				  "--periodicity", "1000", log_level, "info", config, CONFIG_INI };
	auto argv = ConvertToArgv(v);
	CommandLineParser parser(argv.size() - 1, argv.data());
	parser.Subscribe(configuration);
	parser.Parse();
}
//////////////////////////SHORT PARAMETERS/////////////////////

TEST(CommandLineParserTest, ShortParameters)
{
	CreateDir(DESTINATION_DIR);
	auto configuration = std::make_shared<ConfigurationManagerMock>();
	EXPECT_CALL(*configuration, OnPath(PathType::Source, std::string(SOURCE_DIR)));
	EXPECT_CALL(*configuration, OnPath(PathType::Destination, std::string(DESTINATION_DIR)));
	EXPECT_CALL(*configuration, OnOperationMode("regular"));
	EXPECT_CALL(*configuration, OnThreadsNumber("6"));
	EXPECT_CALL(*configuration, OnAdditionalFileExtension(".swp"));
	EXPECT_CALL(*configuration, OnTimeBackup("1000"));
	EXPECT_CALL(*configuration, OnFileNameMask("."));
	EXPECT_CALL(*configuration, OnLogLevel("info"));

	std::vector<std::string> v{ fast_copy_ar, "-s", SOURCE_DIR, "-d", DESTINATION_DIR,
				  "-m", "regular", "-t", "6", "-a", ".swp", "-n", ".",
				  "-p", "1000", "-l", "info" };
	auto argv = ConvertToArgv(v);
	CommandLineParser parser(argv.size() - 1, argv.data());

	parser.Subscribe(configuration);
	parser.Parse();
}