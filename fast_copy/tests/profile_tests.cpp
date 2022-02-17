#include "../include/fast_copy.h"
#include "../include/profile.h"
#include "../include/helper_functions.h"
#include "../include/mem_usage.h"

#include "gtest/gtest.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace helper_functions;

// To run tests you need to turn off LOG_ENABLED define in logger.h
class SpeedTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		CreateDir(DESTINATION_DIR);
		SizeOfDir(SOURCE_DIR_3);
		std::cout << std::endl;
	}
};

// TEST_F(SpeedTest, CopyUtilityLinux)
// {
// 	std::cout << "cp utility in Linux runtime - " << profiler<>::duration(CopyUtilityLinux, SOURCE_DIR_3, DESTINATION_DIR).count() << " ms\n";
// 	calculateMemoryUsage();
// }

TEST_F(SpeedTest, FastCopyUtility)
{
	std::vector<std::string> v{fast_copy_ar, source, SOURCE_DIR_3, destination, DESTINATION_DIR};
	auto argv = ConvertToArgv(v);
	std::cout << "fast copy without logging runtime - " << profiler<>::duration(FastCopy::fast_copy, argv.size() - 1, argv.data()).count() << " ms\n";
	calculateMemoryUsage();
}

// TEST(SpeedTest1, FastCopyUtility_QueueSize)
// {
//     SizeOfDir(SOURCE_DIR);

//     size_t count = CalculateFileNumber(SOURCE_DIR);

//     for (size_t queueSize = 50; queueSize <= count; queueSize += 500)
//     {
//         CreateDir(DESTINATION_DIR);
//         std::string s = std::to_string(queueSize);
//         std::cout << "queue size - " << s << std::endl;
//         char *argv[] = {fast_copy_ar, source, SOURCE_DIR, destination, DESTINATION_DIR, queue, const_cast<char *>(s.c_str())};
//         std::cout << "fast copy without logging runtime - " << profiler<>::duration(FastCopy::fast_copy, (sizeof(argv) / sizeof(argv[0])), argv).count() << " ms\n";
//         calculateMemoryUsage();
//         std::cout << "------------------" << std::endl;
//     }
// }