#include "fast_copy.h"
#include "logger.h"
#include "threads_creator.h"
#include "config_initialize.h"
#include "mem_usage.h"
#include "file_manager_cv.h"

#if (__LINUX__ || __gnu_linux__ || __linux__ || __linux || linux)
#include <unistd.h>
#elif (_WIN32)
#include "win_service.h"
#endif

#include <thread>
#include <memory>
#include <filesystem>
#include <exception>
#include <csignal>

using namespace Utils;

void FastCopy::fast_copy(int argc, char *argv[])
{
#if defined LOG_ENABLED
    LogInit();
#endif
    auto config = InitializeConfig(argc, argv);

    if (config->GetHelp())
        return;

    config->CheckConfiguration();

    SetLogLevel(config->GetLogLevel());

    if (config->GetOperationMode() == OperatingMode::REGULAR)
    {
        LogConfiguration(config);

        ThreadsCreator creator(config);
        creator.Create<FastCopy::FileManagerCV>();
    }
    else
    {
        FlushLog();
#if (_WIN32)
        // SERVICE_TABLE_ENTRY ServiceTable[] =
        //{
        //     {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        //     {NULL, NULL}
        // };

        // if (StartServiceCtrlDispatcher(ServiceTable) == FALSE)
        //{
        //     GetLastError();
        // }
#elif (__LINUX__ || __gnu_linux__ || __linux__ || __linux || linux)
        daemon(1, 1);

        LogInit();
        LogConfiguration(config);
        SetLogLevel(config->GetLogLevel());
#endif
#if (__LINUX__ || __gnu_linux__ || __linux__ || __linux || linux)
        while (true)
        {
            ThreadsCreator creator(config);
            creator.Create<FastCopy::FileManager>();
            sleep(config->GetTimeBackup());
        }
#endif
    }
}
