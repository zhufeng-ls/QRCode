#ifndef QRCODE_BASE_TIMEZONE_H
#define QRCODE_BASE_TIMEZONE_H

#include "Copyable.h"

#include <memory>
#include <time.h> 

// 时区类

namespace QRCode
{

class TimeZone : public QRCode::Copyable
{
 public:
    explicit TimeZone(const char* zoneFile);
    TimeZone(int eastOfUtc, const char* tzname);
    TimeZone() = default;

    bool valid()
    {
        return static_cast<bool>(data_);
    }

    struct tm toLocalTime(time_t secondsSinceEpoch) const;
    time_t fromLocalTime(const struct tm&) const;

    static struct tm toUtcTime(time_t secondsSinceEpoch, bool yday = false);
    static time_t fromUtcTime(const struct tm&);
    static time_t fromUtcTime(int year, int month, int day,
        int hour, int minute, int seconds);
    
    struct Data
    {
        vector<detail::Transition> transitions;
        vector<detail::LocalTime> localtimes;
        vector<string> names;
        string abbreviation; // 缩写
    };

 private:
    std::shared_ptr<Data> data_;
};

}; // namespace QRCode

#endif // QRCODE_BASE_TIMEZONE_H