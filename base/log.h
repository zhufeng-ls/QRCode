#ifndef QRCODE_BASE_LOG_H
#define QRCODE_BASE_LOG_H

#include <string.h>

#include "Timestamp.h"

namespace QRCode
{

#ifdef USE_TIMEZONE
class TimeZone;
#endif

class Logger
{
 public:
    enum LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NUM_LOG_LEVELS,
    };
 
 // complie time calculation of basename of source file
    class SourceFile
    {
     public:
        template<int N>
        SourceFile(const char (&arr)[N])
           : data_(arr),
            size_(N - 1)
        {
            const char *slash = strrchr(data_, '/');
            if (slash)
            {
                data_ = slash + 1;
                size_ -= static_cast<int>(data_ - arr);
            }
        }

        explicit SourceFile(const char* filename)
           : data_(filename)
        {
            const char* slash = strrchr(filename, '/');
            if (slash)
            {
                data_ = slash + 1;
            }

            size_ = static_cast<int>(strlen(data_));
        }

        const char *data_;
        int size_;

     private:

    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    Logger(SourceFile file, int line, bool toAbort);
    ~Logger();

#ifdef USE_LOGSTREAM
    LogStream& stream()
    {
        return impl_.stream_;
    }
#endif

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();
    
    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);
    
#ifdef USE_TIMEZONE
    static void setTimeZone(const TimeZone& tz);
#endif

 private:

    class Impl
    {
     public:
        typedef Logger::LogLevel LogLevel;
        
        Impl(LogLevel level, int old_errno, const SourceFile& file, int line);

        void formatTime();
        void finish();

        Timestamp time_;

#ifdef USE_LOGSTREAM
        LogStream stream_;
#endif

        LogLevel level_;
        int line_;
        SourceFile basename_;
    };

    Impl impl_;
};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
    return g_logLevel;
}



#define LOG_TRACE if (QRCode::Logger::logLevel() <= QRCode::Logger::TRACE) \
    QRCode::Logger(__FILE__, __LINE__, QRCode::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (QRCode::Logger::logLevel() <= QRCode::Logger::DEBUG) \
    QRCode::Logger(__FILE__, __LINE__, QRCode::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (QRCode::Logger::logLevel() <= QRCode::Logger::INFO) \
    QRCode::Logger(__FILE__, __LINE__, QRCode::Logger::INFO, __func__).stream()

#define LOG_WARN QRCode::Logger(__FILE__, __LINE__, QRCode::Logger::WARN).stream()
#define LOG_ERROR QRCode::Logger(__FILE__, __LINE__, QRCode::Logger::ERROR).stream()
#define LOG_FATAL QRCode::Logger(__FILE__, __LINE__, QRCode::Logger::FATAL).stream()
#define LOG_SYSERR QRCode::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL QRCode::Logger(__FILE__, __LINE__, true).stream()

const char* strerror_tl(int savedErrno);

#define CHECK_NOTNULL(val) \
    ::QRCode::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

template<typename T>
    T * CheckNotNull(Logger::SourceFile file, int line, const char* names, T* ptr)
{
    if (!ptr)
    {
        Logger(file, line, Logger::FATAL).stream() << names;
    }
    return ptr;
}

}; // namespace QRCode

#endif // QRCODE_BASE_LOG_H
