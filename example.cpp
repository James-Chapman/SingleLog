// Copyright(c) 2016-2023, James Chapman
//
// Use of this source code is governed by a BSD -
// style license that can be found in the LICENSE file or
// at https://choosealicense.com/licenses/bsd-3-clause/

#include "singlelog.hpp"

// Configure the logger
void SetupLogging()
{
    auto loggerPtr = Uplinkzero::Logging::SingleLog::GetInstance();
    loggerPtr->SetConsoleLogLevel(Uplinkzero::Logging::LogLevel::L_INFO);
    loggerPtr->SetFileLogLevel(Uplinkzero::Logging::LogLevel::L_TRACE);
    loggerPtr->SetLogFilePath("example.log");
}

// Logging via a local pointer
void LocalPointerLogging()
{
    std::string module = "Local pointer thread";
    auto loggerPtr = Uplinkzero::Logging::SingleLog::GetInstance();
    loggerPtr->Critical(module, "Critial message");
    loggerPtr->Error(module, "Error message");
    loggerPtr->Warning(module, "Warning message");
    loggerPtr->Notice(module, "Notice message");
    loggerPtr->Info(module, "Info message");
    loggerPtr->Debug(module, "Debug message");
    loggerPtr->Trace(module, "Trace message");
}

// Logging with macros. This is the recommended logging option.
void MacroLogging()
{
    LOG_FUNCTION_TRACE();
    LOG_CRITICAL("Critial message");
    LOG_ERROR("Error message");
    LOG_WARNING("Warning message");
    LOG_NOTICE("Notice message");
    LOG_INFO("Info message");
    LOG_DEBUG("Debug message");
    LOG_TRACE("Trace message");
}

// Logging with macros. This is the recommended logging option.
void MacroLogging2()
{
    std::string module = "MACRO thread";
    LOG_FUNCTION_TRACE();
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
    std::thread t1(LocalPointerLogging);
    std::thread t2(MacroLogging);
    std::thread t3(MacroLogging2);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}
