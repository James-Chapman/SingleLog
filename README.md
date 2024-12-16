# SingleLog
## Description

SingleLog is a header only **C++14** singleton logging library. It's fast, convenient, compiles on Windows and Linux, and does what it says on the tin. The example is C++20 and will build with **scons**.


## Example

Using SingleLog is as easy as this:

```c++
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
```

Which, when run will produce the following in the console:

```
user@machine> ./SingleLogExample 
2024-12-16 20:27:40 +0000  <CRITICAL>  Local logger ref thread:  Critial message
2024-12-16 20:27:40 +0000  <ERROR>  Local logger ref thread:  Error message
2024-12-16 20:27:40 +0000  <WARNING>  Local logger ref thread:  Warning message
2024-12-16 20:27:40 +0000  <NOTICE>  Local logger ref thread:  Notice message
2024-12-16 20:27:40 +0000  <INFO>  Local logger ref thread:  Info message
2024-12-16 20:27:40 +0000  <CRITICAL>  MacroLogging:  Critial message
2024-12-16 20:27:40 +0000  <CRITICAL>  MacroLogging_v2:  Critical message 1
2024-12-16 20:27:40 +0000  <ERROR>  MacroLogging:  Error message
2024-12-16 20:27:40 +0000  <WARNING>  MacroLogging:  Warning message
2024-12-16 20:27:40 +0000  <NOTICE>  MacroLogging:  Notice message
2024-12-16 20:27:40 +0000  <ERROR>  MacroLogging_v2:  Error message 2
2024-12-16 20:27:40 +0000  <INFO>  MacroLogging:  Info message
2024-12-16 20:27:40 +0000  <WARNING>  MacroLogging_v2:  Warning message 3
2024-12-16 20:27:40 +0000  <NOTICE>  MacroLogging_v2:  Notice message 4
2024-12-16 20:27:40 +0000  <INFO>  MacroLogging_v2:  Info message 5
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:41 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:41 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:41 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:41 +0000  <INFO>  operator():  Bar
```

And the following in the log file **"example.log"**:

```
user@machine> cat example.log
2024-12-16 20:27:40 +0000  <CRITICAL>  Local logger ref thread:  Critial message
2024-12-16 20:27:40 +0000  <ERROR>  Local logger ref thread:  Error message
2024-12-16 20:27:40 +0000  <WARNING>  Local logger ref thread:  Warning message
2024-12-16 20:27:40 +0000  <NOTICE>  Local logger ref thread:  Notice message
2024-12-16 20:27:40 +0000  <TRACE>  FunctionTrace:  >>> Entering: MacroLogging
2024-12-16 20:27:40 +0000  <TRACE>  FunctionTrace:  >>> Entering: MacroLogging_v2
2024-12-16 20:27:40 +0000  <INFO>  Local logger ref thread:  Info message
2024-12-16 20:27:40 +0000  <CRITICAL>  MacroLogging:  Critial message
2024-12-16 20:27:40 +0000  <DEBUG>  Local logger ref thread:  Debug message
2024-12-16 20:27:40 +0000  <TRACE>  Local logger ref thread:  Trace message
2024-12-16 20:27:40 +0000  <ERROR>  MacroLogging:  Error message
2024-12-16 20:27:40 +0000  <CRITICAL>  MacroLogging_v2:  Critical message 1
2024-12-16 20:27:40 +0000  <WARNING>  MacroLogging:  Warning message
2024-12-16 20:27:40 +0000  <NOTICE>  MacroLogging:  Notice message
2024-12-16 20:27:40 +0000  <ERROR>  MacroLogging_v2:  Error message 2
2024-12-16 20:27:40 +0000  <INFO>  MacroLogging:  Info message
2024-12-16 20:27:40 +0000  <DEBUG>  MacroLogging:  Debug message
2024-12-16 20:27:40 +0000  <WARNING>  MacroLogging_v2:  Warning message 3
2024-12-16 20:27:40 +0000  <TRACE>  MacroLogging:  Trace message
2024-12-16 20:27:40 +0000  <NOTICE>  MacroLogging_v2:  Notice message 4
2024-12-16 20:27:40 +0000  <INFO>  MacroLogging_v2:  Info message 5
2024-12-16 20:27:40 +0000  <TRACE>  FunctionTrace:  <<< Exiting: MacroLogging
2024-12-16 20:27:40 +0000  <DEBUG>  MacroLogging_v2:  Debug message 6
2024-12-16 20:27:40 +0000  <TRACE>  MacroLogging_v2:  Trace message 7
2024-12-16 20:27:40 +0000  <TRACE>  FunctionTrace:  <<< Exiting: MacroLogging_v2
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:40 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:40 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:41 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:41 +0000  <INFO>  operator():  Bar
2024-12-16 20:27:41 +0000  <INFO>  operator():  Foo
2024-12-16 20:27:41 +0000  <INFO>  operator():  Bar

```

