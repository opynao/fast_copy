#pragma once
#include "cli_parser.h"
#include "configuration_manager.h"
#include "logger.h"
#include "settings_parser.h"
#include "events.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

class ConfigurationManagerMock : public FastCopy::ParserEvents
{
public:
    MOCK_METHOD(void, OnHelp, (), (override));
    MOCK_METHOD(void, OnPath, (const FastCopy::PathType &type, const param_t &path), (override));
    MOCK_METHOD(void, OnOperationMode, (const param_t &mode), (override));
    MOCK_METHOD(void, OnAdditionalFileExtension, (const param_t &extension), (override));
    MOCK_METHOD(void, OnThreadsNumber, (const param_t &threads), (override));
    MOCK_METHOD(void, OnTimeBackup, (const param_t &backupTime), (override));
    MOCK_METHOD(void, OnFileNameMask, (const param_t &mask), (override));
    MOCK_METHOD(void, OnLogLevel, (const param_t &level), (override));
    MOCK_METHOD(void, OnQueueSize, (const param_t &queueSize), (override));
};
