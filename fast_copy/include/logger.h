#pragma once
#include "configuration_manager.h"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include <memory>
#include <iostream>

// #define LOG_ENABLED
#define LOGF std::cerr << __FUNCTION__ << " = " << __LINE__ << std::endl
#define PR(x) std::cerr << #x << "=" << x << std::endl
#define LOG(message) std::cerr << message << std::endl
#define LOGN(message, number) std::cerr << message << number << std::endl

#define LOG_I1(msg) spdlog::get("logger")->info(msg);
#define LOG_I2(msg, arg1) spdlog::get("logger")->info(msg, arg1);
#define LOG_I3(msg, arg1, arg2) spdlog::get("logger")->info(msg, arg1, arg2);
#define GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define LOG_I_MACRO_CHOOSER(...) \
    GET_4TH_ARG(__VA_ARGS__, LOG_I3, LOG_I2, LOG_I1, )
#define LOG_I(...)                   \
    LOG_I_MACRO_CHOOSER(__VA_ARGS__) \
    (__VA_ARGS__)

#define LOG_W1(msg) spdlog::get("logger")->warn(msg);
#define LOG_W2(msg, arg1) spdlog::get("logger")->warn(msg, arg1);
#define LOG_W3(msg, arg1, arg2) spdlog::get("logger")->warn(msg, arg1, arg2);
#define GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define LOG_W_MACRO_CHOOSER(...) \
    GET_4TH_ARG(__VA_ARGS__, LOG_W3, LOG_W2, LOG_W1, )
#define LOG_W(...)                   \
    LOG_W_MACRO_CHOOSER(__VA_ARGS__) \
    (__VA_ARGS__)

#define LOG_E(msg) Utils::LogError(msg);

namespace FastCopy
{
    class ConfigurationManager;
}

namespace Utils
{
    void LogInit();
    void LogConfiguration(const std::shared_ptr<FastCopy::ConfigurationManager> &);
    void SetLogLevel(const FastCopy::LogLevel &);
    void LogError(const std::string &);
    void FlushLog();
} // !Utils