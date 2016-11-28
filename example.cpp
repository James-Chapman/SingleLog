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

#include <string>
#include "singlelog.hpp"

void threadFunc()
{
    Uplinkzero::Logging::SingleLog * pLogger = Uplinkzero::Logging::SingleLog::getInstance();
    std::string module = "thread";
    pLogger->critical(module, "Critial message");
    pLogger->error(module, "Error message");
    pLogger->warning(module, "Warning message");
    pLogger->notice(module, "Notice message");
    pLogger->info(module, "Info message");
    pLogger->debug(module, "Debug message");
    pLogger->trace(module, "Trace message");
}

int main()
{
    Uplinkzero::Logging::SingleLog * pLogger = Uplinkzero::Logging::SingleLog::getInstance();
    std::string module = "main";
    pLogger->setConsoleLogLevel(Uplinkzero::Logging::L_INFO);
    pLogger->setFileLogLevel(Uplinkzero::Logging::L_TRACE);
    pLogger->setLogFilePath("example.log");

    std::thread t1(threadFunc);

    pLogger->critical(module, "Critial message");
    pLogger->error(module, "Error message");
    pLogger->warning(module, "Warning message");
    pLogger->notice(module, "Notice message");
    pLogger->info(module, "Info message");
    pLogger->debug(module, "Debug message");
    pLogger->trace(module, "Trace message");

    t1.join();

    return 0;
}