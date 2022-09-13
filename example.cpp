/*******************************************************************************
* Copyright (c) 2016 James Chapman
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
********************************************************************************/

#include "singlelog.hpp"

// Configure the logger
void SetupLogging()
{
    Uplinkzero::Logging::SingleLog * pLogger = Uplinkzero::Logging::SingleLog::GetInstance();
    pLogger->SetConsoleLogLevel(Uplinkzero::Logging::LogLevel::L_INFO);
    pLogger->SetFileLogLevel(Uplinkzero::Logging::LogLevel::L_TRACE);
    pLogger->SetLogFilePath("example.log");
}

// Logging via a local pointer
void LocalPointerLogging()
{
    std::string module = "Local pointer thread";
    Uplinkzero::Logging::SingleLog * pLogger = Uplinkzero::Logging::SingleLog::GetInstance();
    pLogger->Critical(module, "Critial message");
    pLogger->Error(module, "Error message");
    pLogger->Warning(module, "Warning message");
    pLogger->Notice(module, "Notice message");
    pLogger->Info(module, "Info message");
    pLogger->Debug(module, "Debug message");
    pLogger->Trace(module, "Trace message");
}

// Logging with macros. This is the recommended logging option.
void MacroLogging()
{
    std::string module = "MACRO thread";
    LOG_TRACE_FUNCTION();
    LOG_CRITICAL(module, "Critial message");
    LOG_ERROR(module, "Error message");
    LOG_WARNING(module, "Warning message");
    LOG_NOTICE(module, "Notice message");
    LOG_INFO(module, "Info message");
    LOG_DEBUG(module, "Debug message");
    LOG_TRACE(module, "Trace message");
}

int main()
{
    SetupLogging();
    std::thread t1(LocalPointerLogging);
    std::thread t2(MacroLogging);
    t1.join();
    t2.join();
    return 0;
}
