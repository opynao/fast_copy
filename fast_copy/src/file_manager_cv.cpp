#include "file_manager_cv.h"
#include "logger.h"
#include "spdlog/spdlog.h"
#include "string_conversion.h"

#include <regex>
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;
using namespace Utils;

namespace FastCopy
{
	FileManagerCV::FileManagerCV(const std::shared_ptr<ConfigurationManager>& configuration)
		: configuration_{ configuration },
		prepared{ false },
		processed{ true }
	{
	}

	void FileManagerCV::PrepareData()
	{
		LOG_I("Task <Prepare Data> created");

		std::unique_lock<std::mutex> lk(m);
		signalPrepareData_.wait(lk, [this]
			{ return processed; });

		for (auto& file : fs::recursive_directory_iterator(configuration_->GetPath(PathType::Source)))
		{
			if (IsFileMatchesMask(file) && fs::is_regular_file(file))
			{
				fs::path path = file.path();
				const String fileName = String(file.path().filename());
				LOG_I("In the directory {0} found file \"{1}\"", ConvertWstringToString(String(path.remove_filename())), ConvertWstringToString(fileName));

				fileStorage_.Push(file);

				if (fileStorage_.Size() == 100)
				{
					lk.unlock();
					prepared = true;
					signalPrepareData_.notify_all();
				}
			}
		}
		lk.unlock();
		prepared = true;
		signalPrepareData_.notify_all();
	}

	void FileManagerCV::ProcessData()
	{
		LOG_I("Task <Process Data> created");

		std::unique_lock<std::mutex> lk(m);
		signalPrepareData_.wait(lk, [this]
			{ return prepared; });

		while (!fileStorage_.IsEmpty())
		{
			const auto sourcePath = fileStorage_.Pop();
			const fs::path destinationPath = configuration_->GetPath(PathType::Destination);
			Copy(*sourcePath, destinationPath);

			if (fileStorage_.IsEmpty())
			{
				lk.unlock();
				processed = true;
				signalPrepareData_.notify_all();
			}
		}
		signalPrepareData_.notify_all();
	}

	void FileManagerCV::Copy(const fs::path& sourcePath, const fs::path& destinationPath) const
	{
		std::error_code ec;

		fs::copy_file(sourcePath, GetFinalPath(sourcePath, destinationPath), ec);

		if (ec == std::errc::file_exists)
		{
			LOG_I("In the destination directory file \"{}\" already exists", ConvertWstringToString(String(sourcePath.filename())));
		}
		else
		{
			LOG_I("End of copy file \"{}\" to the destination directory", ConvertWstringToString(String(sourcePath.filename())));
		}
	}

	bool FileManagerCV::IsFileMatchesMask(const fs::directory_entry& sourcePath) const
	{
		auto rx = std::regex{ configuration_->GetFileNameMask(), std::regex_constants::icase };
		return std::regex_search(ConvertWstringToString(String(sourcePath.path().filename())), rx);
	}

	fs::path FileManagerCV::GetFinalPath(const fs::path& sourcePath, fs::path destinationPath) const
	{
		const String currentExt = sourcePath.filename().extension();
		return destinationPath.replace_filename(sourcePath.filename().replace_extension(ConvertWstringToString(currentExt) + configuration_->GetAdditionalFileExtension()));
	}
} //! Fast Copy
