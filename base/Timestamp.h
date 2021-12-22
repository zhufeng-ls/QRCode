#ifndef QRCODE_BASE_TIMESTAMP_H
#define QRCODE_BASE_TIMESTAMP_H

#include "Copyable.h"

#include <stdint.h> // int64_t
#include <algorithm> // swap
#include <string>

namespace QRCode
{

class Timestamp : public QRCode::Copyable
{
 public:
    Timestamp()
       : microSecondSinceEpoch_(0)
    {
    }

    explicit Timestamp(int64_t microSecondsSinceEpochArg)
       : microSecondSinceEpoch_(microSecondsSinceEpochArg)
    {

    }

    void swap(Timestamp& t)
    {
        std::swap(microSecondSinceEpoch_, t.microSecondSinceEpoch_);
    }

    std::string toString();
    std::string toFormattedString(bool showMicroSeconds = true) const;

    bool valid() const
    {
        return microSecondSinceEpoch_ > 0;
    }

    int64_t microSecondsSinceEpoch() const 
    {
        return microSecondSinceEpoch_;
    }

    time_t secondsSinceEpoch() const
    {
        return static_cast<time_t>(microSecondSinceEpoch_ / kMicroSecondsPerSecond);
    }

    static Timestamp now();
    static Timestamp invalid()
    {
        return Timestamp();
    }

    static Timestamp fromUnixTime(time_t t, int microseconds)
    {
        return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
    }

    static const int kMicroSecondsPerSecond = 1000 * 1000;

 private:
    int64_t microSecondSinceEpoch_;
};

inline bool operator < (Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator <= (Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() <= rhs.microSecondsSinceEpoch();
}


inline bool operator == (Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline bool operator != (Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() != rhs.microSecondsSinceEpoch();
}

inline bool operator > (Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() > rhs.microSecondsSinceEpoch();
}

inline bool operator >= (Timestamp lhs, Timestamp rhs)
{
    return lhs.microSecondsSinceEpoch() >= rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high, Timestamp low)
{
    int64_t diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
    return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp t, double seconds)
{
    int64_t delta = static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
    return Timestamp(t.microSecondsSinceEpoch + delta);
}


}; // namespace QRCode

#endif // QRCODE_BASE_TIMESTAMP_H