#include "helper_functions.h"
#include "logger.h"

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace helper_functions
{
#if defined(_WIN32)
    char SOURCE_DIR[]{ "D:\\task\\programming_books\\" };
    char SOURCE_DIR_1[]{ "D:\\task\\Windows\\" };
    char SOURCE_DIR_2[]{ "D:\\task\\test1\\" };

    char DESTINATION_DIR[]{ "D:\\task\\test\\" };
    char DESTINATION_DIR_1[]{ "D:\\task\\test2\\" };
    char DESTINATION_DIR_2[]{ "D:\\task\\test3\\" };
    char DESTINATION_DIR_WITH_SPACE[]{ "D:\\task\\test\ new\\" };

    char CONFIG_INI[]{ "D:\\task\\config.ini" };
    char CONFIG_JSON[]{ "D:\\task\\config.json" };
#else
    char SOURCE_DIR[]{"/mnt/d/task/programming_books/"};
    char SOURCE_DIR_1[]{"/mnt/d/task/Windows/"};
    char SOURCE_DIR_2[]{"/mnt/d/task/test1/"};

    char DESTINATION_DIR[]{ "/mnt/d/task/test/" };
    char DESTINATION_DIR_1[]{ "/mnt/d/task/test2/" };
    char DESTINATION_DIR_2[]{ "/mnt/d/task/test3/" };
    char DESTINATION_DIR_WITH_SPACE[]{ "/mnt/d/task/test\\ new/" };

    char CONFIG_INI[]{ "/mnt/d/task/config.ini" };
    char CONFIG_JSON[]{ "/mnt/d/task/config.json" };
#endif

    char CP[]{"cp -r "};
    char SIZE[]{"du -sh "};

    char fast_copy_ar[]{"fast_copy"};
    char source[]{"--source"};
    char destination[]{"--destination"};
    char threads[]{"--threads"};
    char mask[]{"--mask"};
    char addext[]{"--addext"};
    char config[]{"--config"};
    char log_level[]{"--loglevel"};
    char mode[]{"--mode"};
    char queue[]{"--queue"};

    void CreateDir(const char *DIR)
    {
        if (fs::exists(std::string(DIR)))
            fs::remove_all(DIR);

        fs::create_directory(DIR);
    }

    void CreateF(const std::string &str, const char *DIR)
    {
        std::ofstream config(DIR);

        config << str;

        config.close();
    }

    void CopyUtilityLinux(const char *SRC, const char *DEST)
    {
        const std::string cp = std::string(CP) + std::string(SRC) + " " + std::string(DEST);
        system(cp.c_str());
    }

    void SizeOfDir(const char *DIR)
    {
        const std::string size = std::string(SIZE) + std::string(DIR);
        system(size.c_str());
    }

    size_t CalculateFileNumber(const char *DIR)
    {
        size_t count{};
        fs::path p1{DIR};

        for (auto &file : std::filesystem::recursive_directory_iterator(p1))
        {
            if (fs::is_regular_file(file))
                ++count;
        }

        std::cout << "# of files in " << DIR << ": " << count << '\n';
        return count;
    }
    
    std::vector<char*> ConvertToArgv(const std::vector<std::string>& arguments)
    {
        std::vector<char*> argv;
        for (const auto& arg : arguments)
            argv.push_back((char*)arg.data());
        argv.push_back(nullptr);
        return argv;
    }
}