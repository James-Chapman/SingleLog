/*******************************************************************************
 * Copyright (c) 2016-2022 James Chapman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright Notice and this permission Notice shall be included in all
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

#pragma once

#include <atomic>
#include <codecvt>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace Uplinkzero
{

    namespace
    {
        // C++11 format converter
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfConverter;

        std::string ToUTF8(const std::wstring& inString)
        {
            return utfConverter.to_bytes(inString);
        }

        std::wstring ToUTF16(const std::string& inString)
        {
            return utfConverter.from_bytes(inString);
        }
    




    /**
     * Get current date/time, format is YYYY-MM-DD HH:mm:ss
     * ref: http://en.cppreference.com/w/cpp/chrono/c/wcsftime
     */
    std::string CurrentDateTime()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t ttnow = std::chrono::system_clock::to_time_t(now);
        char timedisplay[100];
        struct tm buf;
        errno_t err = localtime_s(&buf, &ttnow);
        if (std::strftime(timedisplay, sizeof(timedisplay), "%F %T", &buf))
        {
            return timedisplay;
        }
        return "";
    }

    /**
     * Get current date/time, format is YYYY-MM-DD HH:mm:ss
     * ref: http://en.cppreference.com/w/cpp/chrono/c/wcsftime
     */
    std::wstring CurrentDateTimeW()
    {
        return ToUTF16(CurrentDateTime());
    }
}

    namespace Logging
    {
        /**
         * Levels of logging available
         */
        enum class LogLevel
        {
            L_TRACE = 100,
            L_DEBUG = 200,
            L_INFO = 300,
            L_NOTICE = 400,
            L_WARNING = 500,
            L_ERROR = 600,
            L_CRITICAL = 700,
            L_OFF = 1000
        };

        /**
         * Logger class
         */
        class SingleLog
        {
        public:
            /**
             * Return a reference to the instance of this class
             * C++11 handles thread safety and removes the need for manual locking
             * http://stackoverflow.com/questions/8102125/is-local-static-variable-initialization-thread-safe-in-c11
             * http://stackoverflow.com/questions/33114896/reentrancy-in-static-method-with-static-variable
             */
            static SingleLog *GetInstance()
            {
                static SingleLog instance;
                return &instance;
            }

            /**
             * Destructor
             */
            ~SingleLog()
            {
                if (!m_exit.load())
                {
                    m_exit.store(true);
                    m_consoleWriter.join();
                    m_fstreamWriter.join();
                }
                if (m_fileOut.is_open())
                {
                    m_fileOut << "\n\n";
                    m_fileOut.close();
                }
            }

            /**
             * Set the minimum log level for the console
             * L_TRACE, L_DEBUG, L_INFO, L_NOTICE, L_WARNING, ERROR, L_CRITICAL, L_OFF
             */
            void SetConsoleLogLevel(const LogLevel &logLevel)
            {
                m_consoleLogLevel = logLevel;
            }

            /**
             * Set the minimum log level for the log file
             * L_TRACE, L_DEBUG, L_INFO, L_NOTICE, L_WARNING, ERROR, L_CRITICAL, L_OFF
             */
            void SetFileLogLevel(const LogLevel &logLevel)
            {
                m_fileLogLevel = logLevel;
            }

            /**
             * Set the path to the log file
             */
            void SetLogFilePath(const std::string &filePath)
            {
                m_filePath = filePath;
                m_fileOut.open(m_filePath, std::ios_base::out);
            }

            /**
             * Set the path to the log file
             */
            void SetLogFilePath(const std::wstring &filePath)
            {
                SetLogFilePath(ToUTF8(filePath));
            }

            /**
             * Log the line to console and/or file
             */
            void LogIt(LogLevel level, const std::string &line)
            {
                if (m_consoleLogLevel <= level)
                {
                    ConsoleLog(line);
                }
                if ((m_fileLogLevel <= level) && (m_fileOut.is_open()))
                {
                    FileLog(line);
                }
            }

            /**
             * Log TRACE level messages
             */
            void Trace(const std::string &_module, const std::string &_message)
            {
                std::string level = "TRACE";
                LogIt(LogLevel::L_TRACE, MakeLogLine(level, _module, _message));
            }

            /**
             * Log TRACE level messages
             */
            void Trace(const std::wstring &_module, const std::wstring &_message)
            {
                std::wstring level = L"TRACE";
                LogIt(LogLevel::L_TRACE, MakeLogLine(level, _module, _message));
            }

            /**
             * Log DEBUG level messages
             */
            void Debug(const std::string &_module, const std::string &_message)
            {
                std::string level = "DEBUG";
                LogIt(LogLevel::L_DEBUG, MakeLogLine(level, _module, _message));
            }

            /**
             * Log DEBUG level messages
             */
            void Debug(const std::wstring &_module, const std::wstring &_message)
            {
                std::wstring level = L"DEBUG";
                LogIt(LogLevel::L_DEBUG, MakeLogLine(level, _module, _message));
            }

            /**
             * Log INFO level messages
             */
            void Info(const std::string &_module, const std::string &_message)
            {
                std::string level = "INFO";
                LogIt(LogLevel::L_INFO, MakeLogLine(level, _module, _message));
            }

            /**
             * Log INFO level messages
             */
            void Info(const std::wstring &_module, const std::wstring &_message)
            {
                std::wstring level = L"INFO";
                LogIt(LogLevel::L_INFO, MakeLogLine(level, _module, _message));
            }

            /**
             * Log NOTICE level messages
             */
            void Notice(const std::string &_module, const std::string &_message)
            {
                std::string level = "NOTICE";
                LogIt(LogLevel::L_NOTICE, MakeLogLine(level, _module, _message));
            }

            /**
             * Log NOTICE level messages
             */
            void Notice(const std::wstring &_module, const std::wstring &_message)
            {
                std::wstring level = L"NOTICE";
                LogIt(LogLevel::L_NOTICE, MakeLogLine(level, _module, _message));
            }

            /**
             * Log WARNING level messages
             */
            void Warning(const std::string &_module, const std::string &_message)
            {
                std::string level = "WARNING";
                LogIt(LogLevel::L_WARNING, MakeLogLine(level, _module, _message));
            }

            /**
             * Log WARNING level messages
             */
            void Warning(const std::wstring &_module, const std::wstring &_message)
            {
                std::wstring level = L"WARNING";
                LogIt(LogLevel::L_WARNING, MakeLogLine(level, _module, _message));
            }

            /**
             * Log ERROR level messages
             */
            void Error(const std::string &_module, const std::string &_message)
            {
                std::string level = "ERROR";
                LogIt(LogLevel::L_ERROR, MakeLogLine(level, _module, _message));
            }

            /**
             * Log ERROR level messages
             */
            void Error(const std::wstring &_module, const std::wstring &_message)
            {
                std::wstring level = L"ERROR";
                LogIt(LogLevel::L_ERROR, MakeLogLine(level, _module, _message));
            }

            /**
             * Log CRITICAL level messages
             */
            void Critical(const std::string &_module, const std::string &_message)
            {
                std::string level = "CRITICAL";
                LogIt(LogLevel::L_CRITICAL, MakeLogLine(level, _module, _message));
            }

            /**
             * Log CRITICAL level messages
             */
            void Critical(const std::wstring &_module, const std::wstring &_message)
            {
                std::wstring level = L"CRITICAL";
                LogIt(LogLevel::L_CRITICAL, MakeLogLine(level, _module, _message));
            }

        private:
            /**
             * Private Constructor
             */
            SingleLog()
                : m_consoleLogLevel(LogLevel::L_TRACE), m_fileLogLevel(LogLevel::L_TRACE), m_filePath(""), m_exit(false)
            {
                m_consoleWriter = std::thread(&SingleLog::ConsoleWriter, this);
                m_fstreamWriter = std::thread(&SingleLog::FstreamWriter, this);
            }

            /**
             * Copy constructor, we don't want it since this is a Singleton
             */
            SingleLog(SingleLog const &copy) = delete;
            SingleLog &operator=(SingleLog const &copy) = delete;

            /**
             * Create a common format log line
             * Note: There might be a better way to produce UTF8 from ANSI text? This is "expensive".
             */
            inline std::string MakeLogLine(const std::string &_level, const std::string &_module, const std::string &_message)
            {
                std::stringstream ss;
                ss << "" << CurrentDateTime() << "  <" << _level << ">  " + _module + ":  " << _message << "\n";
                return ss.str();
            }

            inline std::string MakeLogLine(const std::wstring &_level, const std::wstring &_module, const std::wstring &_message)
            {
                std::wstringstream ss;
                ss << L"" << CurrentDateTimeW() << L"  <" << _level << L">  " + _module + L":  " << _message << L"\n";
                return ToUTF8(ss.str());
            }

            /**
             * Log message to console deque
             */
            inline void ConsoleLog(std::string _s)
            {
                std::lock_guard<std::mutex> lock(m_consoleLogDequeLock);
                m_consoleLogDeque.push_back(_s);
            }

            /**
             * Log message to file deque
             */
            inline void FileLog(std::string _s)
            {
                std::lock_guard<std::mutex> lock(m_fstreamLogDequeLock);
                m_fstreamLogDeque.push_back(_s);
            }

            /**
             * Write messages to the console.
             */
            void ConsoleWriter()
            {
                //
                while (1)
                {
                    bool consoleLogEmpty = m_consoleLogDeque.empty();
                    if (!consoleLogEmpty)
                    {
                        std::lock_guard<std::mutex> lock(m_consoleLogDequeLock);
                        std::string s = m_consoleLogDeque.front();
                        m_consoleLogDeque.pop_front();
                        std::cout << s;
                    }

                    if ((m_exit.load()) && (consoleLogEmpty))
                    {
                        break;
                    }

                    // Short sleep to avoid consuming high CPU
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            }

            /**
             * Write messages to the log file.
             */
            void FstreamWriter()
            {
                //
                while (1)
                {
                    bool fstreamLogEmpty = m_fstreamLogDeque.empty();
                    if (!fstreamLogEmpty)
                    {
                        std::string s;
                        {
                            std::lock_guard<std::mutex> lock(m_fstreamLogDequeLock);
                            s = m_fstreamLogDeque.front();
                            m_fstreamLogDeque.pop_front();
                        }
                        {
                            std::lock_guard<std::mutex> lock(m_fstreamLock);
                            m_fileOut.flush();
                            m_fileOut << s;
                        }
                    }

                    if ((m_exit.load()) && (fstreamLogEmpty))
                    {
                        break;
                    }

                    // Short sleep to avoid consuming high CPU
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                }
            }

            LogLevel m_consoleLogLevel; // Min level for console logs
            LogLevel m_fileLogLevel;    // Min level for file logs
            std::ofstream m_fileOut;    // File output stream
            std::string m_filePath;     // Log file path

            std::mutex m_consoleLogDequeLock;
            std::mutex m_fstreamLogDequeLock;
            std::mutex m_fstreamLock;

            std::deque<std::string> m_consoleLogDeque;
            std::deque<std::string> m_fstreamLogDeque;

            std::atomic_bool m_exit;

            std::thread m_consoleWriter;
            std::thread m_fstreamWriter;
        };

    }; // namespace Logging

    namespace
    {
        auto __globalLoggerPtr = Uplinkzero::Logging::SingleLog::GetInstance();

        class FunctionTrace
        {
        public:
            FunctionTrace(const std::string &functionName)
                : m_functionName{functionName}
            {
                Uplinkzero::Logging::SingleLog *m_logger = Uplinkzero::Logging::SingleLog::GetInstance();
                std::stringstream ss;
                ss << " ---> Entering function: " << m_functionName;
                __globalLoggerPtr->Trace("FunctionTrace", ss.str());
            }

            ~FunctionTrace()
            {
                std::stringstream ss;
                ss << " <--- Exiting function: " << m_functionName;
                __globalLoggerPtr->Trace("FunctionTrace", ss.str());
            }

        private:
            std::string m_functionName;
        };
    }

#define LOG_TRACE_FUNCTION() \
    Uplinkzero::FunctionTrace tr(__func__);

#define LOG_TRACE(module, message) \
    Uplinkzero::__globalLoggerPtr->Trace(module, message);

#define LOG_DEBUG(module, message) \
    Uplinkzero::__globalLoggerPtr->Debug(module, message);

#define LOG_INFO(module, message) \
    Uplinkzero::__globalLoggerPtr->Info(module, message);

#define LOG_NOTICE(module, message) \
    Uplinkzero::__globalLoggerPtr->Notice(module, message);

#define LOG_WARNING(module, message) \
    Uplinkzero::__globalLoggerPtr->Warning(module, message);

#define LOG_ERROR(module, message) \
    Uplinkzero::__globalLoggerPtr->Error(module, message);

#define LOG_CRITICAL(module, message) \
    Uplinkzero::__globalLoggerPtr->Critical(module, message);

}; // namespace Uplinkzero
