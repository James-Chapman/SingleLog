# SingleLog
SingleLog is a header only C++11 singleton logging library. It's fast, convenient, and does what it says on the tin.

It's as easy as:
```c++
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
