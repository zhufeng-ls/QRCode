#ifndef UBT_ZXING_H
#define UBT_ZXING_H

#include "qrParseImpl.h"

namespace QRCode 
{
 class ZXing: public QRParseImpl
 {
  public:
    enum MethodName
    {
        kUseLib = 0,
        kUseExec
    };

  public:
    ZXing(std::string qrPath, int methodName = kUseLib)
    : methodName_(methodName)
    {
        qrPath_ =  qrPath;
    }
    ~ZXing();

    void setMethod(MethodName method);
    std::string parse() override;

  private:
    std::string parseByLib();
    std::string parseByExec();

    int methodName_;

    
 };   

}; // UBT_ZXING_H

#endif // UBT_ZXING_H