#ifndef UBT_QRCODE_PARSE_H
#define UBT_QRCODE_PARSE_H

#include <string>

#include "types.h"

namespace QRCode
{
    enum ParseCode
    {
        kZBar = 0,
        kZXing
    };

    class QRParse
    {
     public:
        explicit QRParse(std::string qrPath, ParseCode methodName = kZBar):
        qrPath_(qrPath), methodName_(methodName)
        {

        }
        ~QRParse();

        std::string parse();

     private:
        std::string qrPath_;
        int methodName_;
    };
}; // QRCode



#endif // UBT_QRCODE_PARSE_H