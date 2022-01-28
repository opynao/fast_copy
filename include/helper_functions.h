#pragma once

#include <string>

namespace helper_functions
{
    extern char SOURCE_DIR[];
    extern char SOURCE_DIR_1[];
    extern char SOURCE_DIR_2[];

    extern char DESTINATION_DIR[];
    extern char DESTINATION_DIR_1[];
    extern char DESTINATION_DIR_2[];
    extern char DESTINATION_DIR_WITH_SPACE[];
    extern char CONFIG_INI[];
    extern char CONFIG_JSON[];

    extern char CP[];
    extern char SIZE[];

    extern char fast_copy_ar[];
    extern char source[];
    extern char destination[];
    extern char threads[];
    extern char mask[];
    extern char addext[];
    extern char config[];
    extern char log_level[];
    extern char mode[];
    extern char queue[];
    extern char invalid_path[];

    void CreateFile(const std::string &str, const char *DIR);
    void CreateDirectory(const char *DIR);
    void CopyUtilityLinux(const char *SRC, const char *DEST);
    void SizeOfDir(const char *DIR);
    size_t CalculateFileNumber(const char *DIR);
} // !Helper