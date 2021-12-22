#ifndef UBT_ZBAR_H
#define UBT_ZBAR_H

#include "qrParseImpl.h"

namespace QRCode
{
    class ZBar: public QRParseImpl
    {
     public:
        explicit ZBar(std::string qrPath);
        
        std::string parse() override;
     private:
        void init();
    };
}; // QRCode

#endif // UBT_ZBAR_H