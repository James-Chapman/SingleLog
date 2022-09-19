# SingleLog
## Description

SingleLog is a header only C++11 singleton logging library. It's fast, convenient, compiles on Windows and Linux, and does what it says on the tin.


## Note

* By design, trace level logging is only available in debug builds. In release builds the lowest logging level is debug.


## Example

Using SingleLog is as easy as this:

```c++
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
```

Which, when run will produce the following in the console:

```
2010-12-01 13:01:12  <CRITICAL>  Local pointer thread:  Critial message
2010-12-01 13:01:12  <ERROR>  Local pointer thread:  Error message
2010-12-01 13:01:12  <CRITICAL>  MACRO thread:  Critial message
2010-12-01 13:01:12  <WARNING>  Local pointer thread:  Warning message
2010-12-01 13:01:12  <NOTICE>  Local pointer thread:  Notice message
2010-12-01 13:01:12  <ERROR>  MACRO thread:  Error message
2010-12-01 13:01:12  <INFO>  Local pointer thread:  Info message
2010-12-01 13:01:12  <WARNING>  MACRO thread:  Warning message
2010-12-01 13:01:12  <NOTICE>  MACRO thread:  Notice message
2010-12-01 13:01:12  <INFO>  MACRO thread:  Info message
```

And the following in the log file **"example.log"**:

```
2010-12-01 13:01:12  <TRACE>  FunctionTrace:   ---> Entering function: MacroLogging
2010-12-01 13:01:12  <CRITICAL>  Local pointer thread:  Critial message
2010-12-01 13:01:12  <ERROR>  Local pointer thread:  Error message
2010-12-01 13:01:12  <CRITICAL>  MACRO thread:  Critial message
2010-12-01 13:01:12  <WARNING>  Local pointer thread:  Warning message
2010-12-01 13:01:12  <NOTICE>  Local pointer thread:  Notice message
2010-12-01 13:01:12  <ERROR>  MACRO thread:  Error message
2010-12-01 13:01:12  <INFO>  Local pointer thread:  Info message
2010-12-01 13:01:12  <WARNING>  MACRO thread:  Warning message
2010-12-01 13:01:12  <DEBUG>  Local pointer thread:  Debug message
2010-12-01 13:01:12  <NOTICE>  MACRO thread:  Notice message
2010-12-01 13:01:12  <TRACE>  Local pointer thread:  Trace message
2010-12-01 13:01:12  <INFO>  MACRO thread:  Info message
2010-12-01 13:01:12  <DEBUG>  MACRO thread:  Debug message
2010-12-01 13:01:12  <TRACE>  MACRO thread:  Trace message
2010-12-01 13:01:12  <TRACE>  FunctionTrace:   <--- Exiting function: MacroLogging
```

