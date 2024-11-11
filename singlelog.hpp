// Copyright(c) 2016-2023, James Chapman
//
// Use of this source code is governed by a BSD -
// style license that can be found in the LICENSE file or
// at https://choosealicense.com/licenses/bsd-3-clause/

#pragma once

#include <array>
#include <atomic>
#include <codecvt>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include <locale>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>

namespace Uplinkzero
{

namespace
{
    // C++11 format converter
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfConverter;

    std::string ToNarrow(const std::wstring& inString)
    {
        return utfConverter.to_bytes(inString);
    }

    std::wstring ToWide(const std::string& inString)
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
        char timedisplay[64];
        struct tm buf;
    #ifdef WIN32
        auto err = localtime_s(&buf, &ttnow);
    #else
        auto err = localtime_r(&ttnow, &buf);
    #endif
        if (std::strftime(timedisplay, sizeof(timedisplay), "%F %T %z", &buf))
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
        return ToWide(CurrentDateTime());
    }
} // namespace

namespace Logging
{
    constexpr auto LogggerInternalBufferSize = 10240;

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
    class SingleLog final
    {
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
         * Delete copy constructor
         */
        SingleLog(SingleLog const& copy) = delete;
        SingleLog& operator=(SingleLog const& copy) = delete;

    public:
        /**
         * Return a reference to the instance of this class
         * C++11 handles thread safety and removes the need for manual locking
         * http://stackoverflow.com/questions/8102125/is-local-static-variable-initialization-thread-safe-in-c11
         * http://stackoverflow.com/questions/33114896/reentrancy-in-static-method-with-static-variable
         */
        static SingleLog& GetInstance()
        {
            static SingleLog instance;
            return instance;
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
        void SetConsoleLogLevel(const LogLevel& logLevel)
        {
            m_consoleLogLevel = logLevel;
        }

        /**
         * Set the minimum log level for the log file
         * L_TRACE, L_DEBUG, L_INFO, L_NOTICE, L_WARNING, ERROR, L_CRITICAL, L_OFF
         */
        void SetFileLogLevel(const LogLevel& logLevel)
        {
            m_fileLogLevel = logLevel;
        }

        /**
         * Set the path to the log file
         */
        void SetLogFilePath(const std::string& filePath)
        {
            m_filePath = filePath;
            m_fileOut.open(m_filePath, std::ios_base::out);
            m_fileOut.rdbuf()->pubsetbuf(m_writeBuffer.data(), LogggerInternalBufferSize);
        }

        /**
         * Set the path to the log file
         */
        void SetLogFilePath(const std::wstring& filePath)
        {
            SetLogFilePath(ToNarrow(filePath));
        }

        /**
         * Log the line to console and/or file
         */
        void LogIt(LogLevel level, const std::string& line)
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
        void Trace(const std::string& _module, const std::string& _message)
        {
            std::string level = "TRACE";
            LogIt(LogLevel::L_TRACE, MakeLogLine(level, _module, _message));
        }

        /**
         * Log TRACE level messages
         */
        void Trace(const std::wstring& _module, const std::wstring& _message)
        {
            std::wstring level = L"TRACE";
            LogIt(LogLevel::L_TRACE, MakeLogLine(level, _module, _message));
        }

        /**
         * Log DEBUG level messages
         */
        void Debug(const std::string& _module, const std::string& _message)
        {
            std::string level = "DEBUG";
            LogIt(LogLevel::L_DEBUG, MakeLogLine(level, _module, _message));
        }

        /**
         * Log DEBUG level messages
         */
        void Debug(const std::wstring& _module, const std::wstring& _message)
        {
            std::wstring level = L"DEBUG";
            LogIt(LogLevel::L_DEBUG, MakeLogLine(level, _module, _message));
        }

        /**
         * Log INFO level messages
         */
        void Info(const std::string& _module, const std::string& _message)
        {
            std::string level = "INFO";
            LogIt(LogLevel::L_INFO, MakeLogLine(level, _module, _message));
        }

        /**
         * Log INFO level messages
         */
        void Info(const std::wstring& _module, const std::wstring& _message)
        {
            std::wstring level = L"INFO";
            LogIt(LogLevel::L_INFO, MakeLogLine(level, _module, _message));
        }

        /**
         * Log NOTICE level messages
         */
        void Notice(const std::string& _module, const std::string& _message)
        {
            std::string level = "NOTICE";
            LogIt(LogLevel::L_NOTICE, MakeLogLine(level, _module, _message));
        }

        /**
         * Log NOTICE level messages
         */
        void Notice(const std::wstring& _module, const std::wstring& _message)
        {
            std::wstring level = L"NOTICE";
            LogIt(LogLevel::L_NOTICE, MakeLogLine(level, _module, _message));
        }

        /**
         * Log WARNING level messages
         */
        void Warning(const std::string& _module, const std::string& _message)
        {
            std::string level = "WARNING";
            LogIt(LogLevel::L_WARNING, MakeLogLine(level, _module, _message));
        }

        /**
         * Log WARNING level messages
         */
        void Warning(const std::wstring& _module, const std::wstring& _message)
        {
            std::wstring level = L"WARNING";
            LogIt(LogLevel::L_WARNING, MakeLogLine(level, _module, _message));
        }

        /**
         * Log ERROR level messages
         */
        void Error(const std::string& _module, const std::string& _message)
        {
            std::string level = "ERROR";
            LogIt(LogLevel::L_ERROR, MakeLogLine(level, _module, _message));
        }

        /**
         * Log ERROR level messages
         */
        void Error(const std::wstring& _module, const std::wstring& _message)
        {
            std::wstring level = L"ERROR";
            LogIt(LogLevel::L_ERROR, MakeLogLine(level, _module, _message));
        }

        /**
         * Log CRITICAL level messages
         */
        void Critical(const std::string& _module, const std::string& _message)
        {
            std::string level = "CRITICAL";
            LogIt(LogLevel::L_CRITICAL, MakeLogLine(level, _module, _message));
        }

        /**
         * Log CRITICAL level messages
         */
        void Critical(const std::wstring& _module, const std::wstring& _message)
        {
            std::wstring level = L"CRITICAL";
            LogIt(LogLevel::L_CRITICAL, MakeLogLine(level, _module, _message));
        }

    private:


        /**
         * Create a common format log line
         * Note: There might be a better way to produce UTF8 from ANSI text? This is "expensive".
         */
        inline std::string MakeLogLine(const std::string& _level, const std::string& _module,
                                       const std::string& _message)
        {
            std::stringstream ss;
            ss << "" << CurrentDateTime() << "  <" << _level << ">  " + _module + ":  " << _message << "\n";
            return ss.str();
        }

        inline std::string MakeLogLine(const std::wstring& _level, const std::wstring& _module,
                                       const std::wstring& _message)
        {
            std::wstringstream ss;
            ss << L"" << CurrentDateTimeW() << L"  <" << _level << L">  " + _module + L":  " << _message << L"\n";
            return ToNarrow(ss.str());
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

        

        LogLevel m_consoleLogLevel{LogLevel::L_INFO};
        LogLevel m_fileLogLevel{LogLevel::L_TRACE};
        std::ofstream m_fileOut{};
        std::string m_filePath{};
        std::array<char, LogggerInternalBufferSize> m_writeBuffer;

        std::mutex m_consoleLogDequeLock;
        std::mutex m_fstreamLogDequeLock;
        std::mutex m_fstreamLock;

        std::deque<std::string> m_consoleLogDeque{};
        std::deque<std::string> m_fstreamLogDeque{};

        std::atomic_bool m_exit{false};

        std::thread m_consoleWriter;
        std::thread m_fstreamWriter;
    };

}; // namespace Logging

namespace
{
    auto& g_globalSingleLogInstanceRef{ Uplinkzero::Logging::SingleLog::GetInstance()};

    class FunctionTrace final
    {
    public:
        explicit FunctionTrace(const std::string& functionName) : m_functionName{functionName}
        {
            std::stringstream ss;
            ss << ">>> Entering: " << m_functionName;
            g_globalSingleLogInstanceRef.Trace("FunctionTrace", ss.str());
        }

        ~FunctionTrace()
        {
            std::stringstream ss;
            ss << "<<< Exiting: " << m_functionName;
            g_globalSingleLogInstanceRef.Trace("FunctionTrace", ss.str());
        }

    private:
        std::string m_functionName;
    };
} // namespace

namespace StringTools
{
    template <typename... Args>
    std::string string_format(const std::string& format, Args&&... args)
    {
        auto size = std::snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args)...) + 1; // Extra space for '\0'
        if (size < 0)
        {
            return {};
        }
        std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
        std::snprintf(buffer.get(), size, format.c_str(), std::forward<Args>(args)...);
        return std::string(buffer.get(), buffer.get() + size - 1); // We don't want the '\0' inside
    }
} // namespace StringTools

#define LOG_FUNCTION_TRACE Uplinkzero::FunctionTrace tr(__func__);

#define LOG_TRACE(message) Uplinkzero::g_globalSingleLogInstanceRef.Trace(__func__, message);

#define LOG_DEBUG(message) Uplinkzero::g_globalSingleLogInstanceRef.Debug(__func__, message);

#define LOG_INFO(message) Uplinkzero::g_globalSingleLogInstanceRef.Info(__func__, message);

#define LOG_NOTICE(message) Uplinkzero::g_globalSingleLogInstanceRef.Notice(__func__, message);

#define LOG_WARNING(message) Uplinkzero::g_globalSingleLogInstanceRef.Warning(__func__, message);

#define LOG_ERROR(message) Uplinkzero::g_globalSingleLogInstanceRef.Error(__func__, message);

#define LOG_CRITICAL(message) Uplinkzero::g_globalSingleLogInstanceRef.Critical(__func__, message);

#define LOGF_TRACE(format, ...)                                                                                        \
    Uplinkzero::g_globalSingleLogInstanceRef.Trace(__func__, Uplinkzero::StringTools::string_format(format, __VA_ARGS__));

#define LOGF_DEBUG(format, ...)                                                                                        \
    Uplinkzero::g_globalSingleLogInstanceRef.Debug(__func__, Uplinkzero::StringTools::string_format(format, __VA_ARGS__));

#define LOGF_INFO(format, ...)                                                                                         \
    Uplinkzero::g_globalSingleLogInstanceRef.Info(__func__, Uplinkzero::StringTools::string_format(format, __VA_ARGS__));

#define LOGF_NOTICE(format, ...)                                                                                       \
    Uplinkzero::g_globalSingleLogInstanceRef.Notice(__func__, Uplinkzero::StringTools::string_format(format, __VA_ARGS__));

#define LOGF_WARNING(format, ...)                                                                                      \
    Uplinkzero::g_globalSingleLogInstanceRef.Warning(__func__, Uplinkzero::StringTools::string_format(format, __VA_ARGS__));

#define LOGF_ERROR(format, ...)                                                                                        \
    Uplinkzero::g_globalSingleLogInstanceRef.Error(__func__, Uplinkzero::StringTools::string_format(format, __VA_ARGS__));

#define LOGF_CRITICAL(format, ...)                                                                                     \
    Uplinkzero::g_globalSingleLogInstanceRef.Critical(__func__, Uplinkzero::StringTools::string_format(format, __VA_ARGS__));

}; // namespace Uplinkzero
