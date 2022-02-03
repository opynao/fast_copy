#include "../include/fast_copy.h"
#include "../include/configuration_manager_mock.h"
#include "../include/helper_functions.h"
#include "../include/string_conversion.h"

#include "gtest/gtest.h"

#include <limits>
#include <type_traits>
#include <cstring>

using namespace FastCopy;
using namespace Utils;
using namespace helper_functions;

class FileManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        CreateDir(SOURCE_DIR_2);

        for (const auto &filename : filenames)
        {
            const std::string fileName = std::string(SOURCE_DIR_2) + filename;
            CreateF("", fileName.c_str());
        }

        CreateDir(DESTINATION_DIR_1);
    }

    const std::vector<std::string> filenames{"test1.doc", "test2.doc", "test3.doc"};
    int counter{};
};

// To run tests you need to turn off LOG_ENABLED define in logger.h
// And to comment out LogInit in fast_copy.cpp

TEST_F(FileManagerTest, FileNameMask_FileDoesNotMatchMask)
{
    char filename_mask[]{"\\.txt"};
    std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR_2, destination, DESTINATION_DIR_1, mask, filename_mask };
    auto argv = ConvertToArgv(v);
    fast_copy(argv.size() - 1, argv.data());

    EXPECT_TRUE(fs::is_empty(fs::path(DESTINATION_DIR_1)));
    assert(false);
}

TEST_F(FileManagerTest, FileNameMask_FileMatchesMask)
{
    char filename_mask[]{"\\.doc"};
    std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR_2, destination, DESTINATION_DIR_1, mask, filename_mask };
    auto argv = ConvertToArgv(v);
    fast_copy(argv.size() - 1, argv.data());

    for (auto &file : fs::recursive_directory_iterator(fs::path(DESTINATION_DIR_1)))
        EXPECT_EQ(ConvertWstringToString(String(file.path().filename())), filenames[counter++]);
}

////////////////Additional File Extension///////////////////////////////////////////////////
TEST_F(FileManagerTest, AdditionalFileExtension)
{
    char filename_extension[]{".swp"};
    std::vector<std::string> v{ fast_copy_ar, source, SOURCE_DIR_2, destination, DESTINATION_DIR_1, addext, filename_extension };
    auto argv = ConvertToArgv(v);
    fast_copy(argv.size() - 1, argv.data());

    int count{};
    for (auto &file : fs::recursive_directory_iterator(fs::path(DESTINATION_DIR_1)))
    {
        if (file.path().extension() == filename_extension)
            ++count;
    }
    EXPECT_EQ(count, 3);
}