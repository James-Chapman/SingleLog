// Copyright(c) 2016-2024, James Chapman
//
// Use of this source code is governed by a BSD -
// style license that can be found in the LICENSE file or
// at https://choosealicense.com/licenses/bsd-3-clause/

#include "singlelog.hpp"

#include "Foo.hpp"
#include "Bar.hpp"

// Configure the logger
void SetupLogging()
{
    auto& logger { Uplinkzero::Logging::SingleLog::GetInstance() };
    logger.SetConsoleLogLevel(Uplinkzero::Logging::LogLevel::L_INFO);
    logger.SetFileLogLevel(Uplinkzero::Logging::LogLevel::L_TRACE);
    logger.SetLogFilePath("example.log");
}

// Logging via a local reference to the logger
void LocalRefLogging()
{
    std::string module = "Local logger ref thread";
    auto& logger { Uplinkzero::Logging::SingleLog::GetInstance() };
    logger.Critical(module, "Critial message");
    logger.Error(module, "Error message");
    logger.Warning(module, "Warning message");
    logger.Notice(module, "Notice message");
    logger.Info(module, "Info message");
    logger.Debug(module, "Debug message");
    logger.Trace(module, "Trace message");
}

// Logging with macros. This is the recommended logging option.
void MacroLogging()
{
    LOG_FUNCTION_TRACE;
    LOG_CRITICAL("Critial message");
    LOG_ERROR("Error message");
    LOG_WARNING("Warning message");
    LOG_NOTICE("Notice message");
    LOG_INFO("Info message");
    LOG_DEBUG("Debug message");
    LOG_TRACE("Trace message");
}

// Logging with macros. This is the recommended logging option.
void MacroLogging_v2()
{
    std::string module = "MACRO thread";
    LOG_FUNCTION_TRACE;
    LOGF_CRITICAL("%s message %d", "Critical", 1);
    LOGF_ERROR("%s message %d", "Error", 2);
    LOGF_WARNING("%s message %d", "Warning", 3);
    LOGF_NOTICE("%s message %d", "Notice", 4);
    LOGF_INFO("%s message %d", "Info", 5);
    LOGF_DEBUG("%s message %d", "Debug", 6);
    LOGF_TRACE("%s message %d", "Trace", 7);
}

int main()
{
    SetupLogging();
    std::jthread t1(LocalRefLogging);
    std::jthread t2(MacroLogging);
    std::jthread t3(MacroLogging_v2);
    Uplinkzero::Foo foo{};
    Uplinkzero::Bar bar{};
    return 0;
}
