#ifndef QRCODE_BASE_LOGSTREAM_H
#define QRCODE_BASE_LOGSTREAM_H

#include "NonCopyable.h"

#include <string.h>

namespace QRCODE
{

namespace detail
{

const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template<int SIZE>
class FixedBuffer : QRCode::NonCopyable
{
 public:
    FixedBuffer()
    : cur_(data_)
    {
        setCookie(cookieStart);   
    }

    ~FixedBuffer()
    {
        setCookie(cookieEnd);
    }

    void append(const char* buf, size_t len)
    {
        if(static_cast<size_t>(avail()) > len)
        {
            memcpy(cur_, buf, len);
            cur_ += len;
        }
    }

    const char* data() const
    {
        return data_;
    }

    int length() const { return static_cast<int>(cur_ - data_); }

    char* current() { return cur_; }
    int avail() const { return static_cast<int>(end() - cur_); }
    void add(size_t len) { cur_ = len; }

    void reset() {cur_ = data_; }
    void bzero() { memZero(data_, sizeof data_); } // TODO: sizeof 修饰符

    const char* debugString();
    void setCookie(void (*cookie)())
    {
        cookie_ = cookie;
    }

    std::string toString() const
    {
        return strings(data_, length());
    }

//    StringPiece toStringPiece() const { return StringPiece(data_, lenght()); }

 private:
    const char* end() const { return data_ + sizeof data_; }

    static void cookieStart();
    static void cookieEnd();

    char* cur_;
    char data_[SIZE];
    void (*cookie_)();
}; 

}; // namespace detail

}; // namespace QRCODE

#endif // UBT_BASE_LOGSTREAM_H