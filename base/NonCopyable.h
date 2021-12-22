#ifndef QRCODE_BASE_NONCOPYABLE_H
#define QRCODE_BASE_NONCOPYABLE_H

namespace QRCode
{
class NonCopyable
{
 public:
    NonCopyable(const NonCopyable&) = delete;
    
    void operator = (const NonCopyable&) = delete;
 
 protected:
    NonCopyable() = default;
    ~NonCopyable() = default;

 private:

};

}; // namespace QRCode

#endif // QRCODE_BASE_NONCOPYABLE_H