# SingleLog
## Description

SingleLog is a header only **C++14** singleton logging library. It's fast, convenient, compiles on Windows and Linux, and does what it says on the tin.


## Example

Using SingleLog is as easy as this:

```c++
#include "singlelog.hpp"

// Configure the logger
void SetupLogging()
{
    auto& logger { Uplinkzero::Logging::SingleLog::GetInstance() };
    logger.SetConsoleLogLevel(Uplinkzero::Logging::LogLevel::L_INFO);
    logger.SetFileLogLevel(Uplinkzero::Logging::LogLevel::L_TRACE);
    logger.SetLogFilePath("example.log");
}

// Logging via a reference to the logger
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
    std::string module = "MACRO thread";
    LOG_FUNCTION_TRACE;
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
user@machine> ./SingleLogExample 
2010-11-29 21:27:42 +0000  <CRITICAL>  Local logger ref thread:  Critial message
2010-11-29 21:27:42 +0000  <CRITICAL>  MacroLogging:  Critial message
2010-11-29 21:27:42 +0000  <ERROR>  Local logger ref thread:  Error message
2010-11-29 21:27:42 +0000  <ERROR>  MacroLogging:  Error message
2010-11-29 21:27:42 +0000  <WARNING>  Local logger ref thread:  Warning message
2010-11-29 21:27:42 +0000  <WARNING>  MacroLogging:  Warning message
2010-11-29 21:27:42 +0000  <NOTICE>  MacroLogging:  Notice message
2010-11-29 21:27:42 +0000  <NOTICE>  Local logger ref thread:  Notice message
2010-11-29 21:27:42 +0000  <INFO>  MacroLogging:  Info message
2010-11-29 21:27:42 +0000  <INFO>  Local logger ref thread:  Info message
```

And the following in the log file **"example.log"**:

```
user@machine> cat example.log
2010-11-29 21:27:42 +0000  <CRITICAL>  Local logger ref thread:  Critial message
2010-11-29 21:27:42 +0000  <TRACE>  FunctionTrace:  >>> Entering: MacroLogging
2010-11-29 21:27:42 +0000  <CRITICAL>  MacroLogging:  Critial message
2010-11-29 21:27:42 +0000  <ERROR>  Local logger ref thread:  Error message
2010-11-29 21:27:42 +0000  <ERROR>  MacroLogging:  Error message
2010-11-29 21:27:42 +0000  <WARNING>  Local logger ref thread:  Warning message
2010-11-29 21:27:42 +0000  <WARNING>  MacroLogging:  Warning message
2010-11-29 21:27:42 +0000  <NOTICE>  MacroLogging:  Notice message
2010-11-29 21:27:42 +0000  <NOTICE>  Local logger ref thread:  Notice message
2010-11-29 21:27:42 +0000  <INFO>  MacroLogging:  Info message
2010-11-29 21:27:42 +0000  <DEBUG>  MacroLogging:  Debug message
2010-11-29 21:27:42 +0000  <INFO>  Local logger ref thread:  Info message
2010-11-29 21:27:42 +0000  <TRACE>  MacroLogging:  Trace message
2010-11-29 21:27:42 +0000  <DEBUG>  Local logger ref thread:  Debug message
2010-11-29 21:27:42 +0000  <TRACE>  FunctionTrace:  <<< Exiting: MacroLogging
2010-11-29 21:27:42 +0000  <TRACE>  Local logger ref thread:  Trace message
2010-11-29 21:27:42 +0000  <TRACE>  FunctionTrace:  <<< Exiting: MacroLogging_v2

```

