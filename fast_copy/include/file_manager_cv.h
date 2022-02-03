#pragma once

#include "types.h"
#include "logger.h"
#include "configuration_manager.h"
#include "threadsafe_queue.h"

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
    class FileManagerCV
    {
    public:
        FileManagerCV(const std::shared_ptr<ConfigurationManager> &configuration);
        void PrepareData();
        void ProcessData();

        FileManagerCV(const FileManagerCV &) = delete;
        FileManagerCV &operator=(const FileManagerCV &) = delete;
        FileManagerCV(FileManagerCV &&) = delete;
        FileManagerCV &operator=(FileManagerCV &&) = delete;
        ~FileManagerCV() = default;

    private:
        bool IsFileMatchesMask(const fs::directory_entry &sourcePath) const;
        std::optional<fs::path> FileHasFound(const fs::path &destinationPath, const fs::path &fileName) const;
        bool IsEqualFiles(const fs::path &sourcePath, const fs::path &destinationPath) const;
        void Copy(const fs::path &sourcePath, const fs::path &destinationPath) const;
        fs::path GetFinalPath(const fs::path &sourcePath, fs::path destinationPath) const;

    private:
        std::shared_ptr<ConfigurationManager> configuration_;
        Utils::ThreadsafeQueue<fs::path> fileStorage_;
        std::condition_variable signalPrepareData_;
        std::condition_variable signalProcessData_;
        bool prepared;
        bool processed;
        std::mutex m;
    };
} //! Fast Copy