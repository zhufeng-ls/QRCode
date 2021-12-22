#include "parse.h"

#include "zbar.h"
#include "zxing.h"

using namespace QRCode;

QRParse::~QRParse()
{

}

std::string QRParse::parse()
{
    if (methodName_ == kZBar)
    {
        return ZBar(qrPath_).parse();
    }
    else if (methodName_ == kZXing)
    {
        return ZXing(qrPath_, ZXing::kUseExec).parse(); 
    }
    
}