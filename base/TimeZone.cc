#include "TimeZone.h"

#include <vector>
#include <stdio.h>

#include "NonCopyable.h"
#include "Date.h"


namespace QRCode
{

namespace detail
{

// Transition: 过渡时期
struct Transition
{
    time_t gmttime;
    time_t localtime;

    int localtimeIdex;

    Transition(time_t t, time_t l, int localIdx)
        : gmttime(t), localtime(l), localtimeIdex(localIdx)
    {
    }
};

struct Comp
{
    bool compareGmt;

    Comp(bool gmt)
        : compareGmt(gmt)
    {
    }

    bool operator()(const Transition& lhs, const Transition& rhs) const
    {
        if (compareGmt)
        {
            return lhs.gmttime < rhs.gmttime;
        }
        else
        {
            return lhs.localtime < rhs.localtime;
        }
    }

    bool equal(const Transition& lhs, const Transition& rhs) const
    {
        if (compareGmt)
        {
            return lhs.gmttime == rhs.gmttime;
        }
        else
        {
            return lhs.localtime == rhs.localtime;
        }
    }
};

struct  Localtime
{
    time_t gmtOffset;
    bool isDst;
    int arrbIdx;

    Localtime(time_t offset, bool dst, int arrb)
        : gmtOffset(offset), isDst(dst), arrbIdx(arrb)
    {
    }
};

inline void fillHMS(unsigned seconds, struct tm* utc)
{
    utc->tm_sec = seconds % 60;

    unsigned minites = seconds / 60;

    utc->tm_min = minites % 60;
    utc->tm_hour = minites / 60;
}

}; // namespace detail

const int kSecondsPerDay = 24 * 60 * 60;
    
}; // namespace QRCode 

using namespace QRCode;
using namespace std;

namespace QRCode
{

namespace detail
{

class File : protected NonCopyable
{
 public:
    File(const char* file)
        : fp_(::fopen(file, "rb"))
    {
    }

    ~File()
    {
        if (fp_)
        {
            ::close(fp_);
        }
    }

    bool valid() const { return fp_; }

    string readBytes(int n)
    {
        char buf[n];
        ssize_t nr = ::fread(buf, 1, n, fp_);
        if (nr != n)
        {
            throw logic_error("no enough data");
        }

        return string(buf, n);
    }

    int32_t readInt32()
    {
        int32_t x = 0;
        ssize_t nr = ::fread(&x, 1, sizeof(int32_t), fp_);
        
        if (nr != sizeof(int32_t))
        {
            throw logic_error("bad int32_t data");
        }

        return be32toh(x);
    }

    uint8_t readUInt8()
    {
        uint8_t x = 0;
        ssize_t nr = ::fread(&x, 1, sizeof(uint8_t), fp_);

        if (nr != sizeof(uint8_t))
        {
            throw logic_error
        }
    }

 private:
    FILE* fp_;
};

bool readTimeZoneFile(const char* zonefile， struct TimeZone::Data* data)
{
    File f(zonefile);

    if (f.valid())
    {
        try
        {
            string head = f.readBytes(4);

            if (head != "TZif")
            {
                throw logic_error("bad head");
            }

            string version = f.readBytes(1);
            f.readBytes(15);

            int32_t isgmtcnt = f.readInt32();
            int32_t isstdcnt = f.readInt32();
            int32_t leapcnt = f.readInt32();
            int32_t timecnt = f.readInt32();
            int32_t typecnt = f.readInt32();
            int32_t charcnt = f.readInt32();
        
            vector<int32_t> trans;
            vector<int> localtimes;
            trans.reserve(timecnt);

            for (int i = 0; i < timecnt; ++i)
            {
                trans.push_back(f.readInt32());
            }

            for (int i = 0; i < timecnt; ++i)
            {
                uint8_t local = f.readUInt8();
                localtimes.push_back(local);
            }

            for (int i = 0; i < typecnt; ++i)
            {
                int32_t gmtoff = f.readInt32();
                uint8_t isdst = f.readUInt8();
                uint8_t abbrind = f.readUInt8();

                data->localtimes.push_back(Localtime(gmtoff, isdst, abbrind));
            }

            for (int i = 0; i < timecnt; ++i)
            {
                int localIdx = localtimes[i];
                time_t localtime = trans[i] + data->locatimes[localIdx].gmtOffset;
                data->transitions.push_back(Transition(trans[i], localtime, localIdx));
            }

            data->abbreviation = f.readBytes(charcnt);

            for (int i = 0; i < leapcnt; ++i)
            {
                // ? 为啥言删掉
            }

            (void)isstdcnt;
            (void)isgmtcnt;
        }

        catch(logic_error& e)
        {
            fprintf(stderr, "%s\n", e.what());
        }
    }

    return true;
}

const Localtime* findLocalTime(const TimeZone::Data& data, Transition sentry, Comp comp)
{
    const Localtime* local = NULL;

    if (data.transitions.empty() || comp(sentry, data.transitions.front()))
    {
        local = &data.localtimes.front();
    }
    else
    {
        vector<Transition>::const_iterator transI = lower_bound(data.transitions.begin(),
            data.transitions.end(), sentry, comp);

        if (transI != data.transitions.end())
        {
            if (!comp.equal(sentry, *transI))
            {
                assert(transI != data.transitions.begin());
                --transI;
            }
            
            local = &data.localtimes[transI->localtimeIdex];
        }
        else
        {
            local = &data.localtimes[data.transitions.back().localtimeIdex];
        }
    }

    return local;
}

} // detail

} // namespace QRCode

TimeZone::TimeZone(const char* zonefile)
    : data_(new TimeZone::Data)
{
    if (!detail::readTimeZoneFile(zonefile, data_.get()))
    {
        data_.reset();
    }
}

TimeZone::TimeZone(int eastOfUtc, const char* name)
    : data_(new TimeZone::Data)
{
    data_->localtimes.push_back(detail::Localtime(eastOfUtc, false, 0));
    data_->abbreviation = name;
}

