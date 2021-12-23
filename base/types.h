#ifndef QRCODE_BASE_TYPES_H
#define QRCODE_BASE_TYPES_H

#include <stdint.h>
#include <string.h>
#include <string>


#ifndef NDEBUG
#include <assert.h>
#endif

namespace QRCode
{

using std::string;

inline void memZero(void* p, size_t n)
{
    memset(p, 0, n);
}

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
    return f;
}

template<typename To, typename From>
inline To down_cast(From* f)
{
    // 只是为了确保 From 是 To 的子类型，
    // 可以开启优化将其优化掉
    if (false)
    {
        implicit_cast<From*, To>(0);
    }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
    assert(f == NULL || dynamic_cast<To>(f) != NULL); // RTTI： debug mode only!
#endif

    return static_cast<To>(f);
}

}; // namespace QRCode

#endif // QRCODE_BASE_TYPES_H