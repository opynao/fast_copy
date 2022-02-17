#pragma once

#include "types.h"
#include "logger.h"
#include "configuration_manager.h"
#include "threadsafe_queue.h"
#include "progress_bar.h"

#include <memory>
#include <filesystem>
#include <queue>
#include <optional>
#include <thread>
#include <semaphore>
#include <string>

namespace fs = std::filesystem;

namespace FastCopy
{
    class FileManager
    {
    public:
        FileManager(const std::shared_ptr<ConfigurationManager> &configuration);
        void PrepareData();
        void ProcessData();

        FileManager(const FileManager &) = delete;
        FileManager &operator=(const FileManager &) = delete;
        FileManager(FileManager &&) = delete;
        FileManager &operator=(FileManager &&) = delete;
        ~FileManager() = default;

    private:
        bool IsFileMatchesMask(const fs::directory_entry &sourcePath) const;
        std::optional<fs::path> FileHasFound(const fs::path &destinationPath, const fs::path &fileName) const;
        void Copy(const fs::path &sourcePath, const fs::path &destinationPath) const;
        fs::path GetFinalPath(const fs::path &sourcePath, fs::path destinationPath) const;

    private:
        std::shared_ptr<ConfigurationManager> configuration_;
        Utils::ThreadsafeQueue<fs::path> fileStorage_;
        std::binary_semaphore signalPrepareData_;
        std::condition_variable var;
        std::mutex m;
        bool processed {true};
        std::counting_semaphore<8> signalProcessData_;
    };
} //! Fast Copy