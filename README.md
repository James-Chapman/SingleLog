# SingleLog
## Description

SingleLog is a header only C++11 singleton logging library. It's fast, convenient, and does what it says on the tin.


## Note

* Trace level logging is only available in debug builds. In release builds the lowest logging level is debug.


## Example

Using SingleLog is as easy as this:

```c++
#include <string>
#include "singlelog.hpp"

int main()
{
    Uplinkzero::Logging::SingleLog * pLogger = Uplinkzero::Logging::SingleLog::getInstance();
    std::string module = "main";
    pLogger->setConsoleLogLevel(Uplinkzero::Logging::L_INFO);
    pLogger->setFileLogLevel(Uplinkzero::Logging::L_TRACE);
    pLogger->setLogFilePath("example.log");

    pLogger->critical(module, "Critial message");
    pLogger->error(module, "Error message");
    pLogger->warning(module, "Warning message");
    pLogger->notice(module, "Notice message");
    pLogger->info(module, "Info message");
    pLogger->debug(module, "Debug message");
    pLogger->trace(module, "Trace message");

    return 0;
}
```

Which, when run will produce the following in the console:

```
2016-11-01 11:53:11  <CRITICAL>  main:  Critial message
2016-11-01 11:53:11  <ERROR>  main:  Error message
2016-11-01 11:53:11  <WARNING>  main:  Warning message
2016-11-01 11:53:11  <NOTICE>  main:  Notice message
2016-11-01 11:53:11  <INFO>  main:  Info message
```

And the following in the log file **"example.log"**:

```
2016-11-01 11:53:11  <CRITICAL>  main:  Critial message
2016-11-01 11:53:11  <ERROR>  main:  Error message
2016-11-01 11:53:11  <WARNING>  main:  Warning message
2016-11-01 11:53:11  <NOTICE>  main:  Notice message
2016-11-01 11:53:11  <INFO>  main:  Info message
2016-11-01 11:53:11  <DEBUG>  main:  Debug message
2016-11-01 11:53:11  <TRACE>  main:  Trace message
```

