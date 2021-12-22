#ifndef UBT_QRPARSEIMPL_H
#define UBT_QRPARSEIMPL_H

#include <string>

#include "types.h"

namespace QRCode
{
 enum ParseError
 {
    kNoError = 0,
    kOpenFail,
    kParseError,
    kOtherError
 };

 class QRParseImpl
 {
  public:
    explicit QRParseImpl(std::string qrPath)
    : qrPath_(qrPath)
    {
        
    }
    QRParseImpl() = default;
    virtual ~QRParseImpl() = default;
    virtual std::string parse() = 0;

  protected:
    std::string qrPath_;

    ParseError eError_;
    std::string errStr_;
 };
}; // QRCode

#endif // UBT_QRPARSEIMPL_H