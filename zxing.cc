#include "zxing.h"

#include <zxing/common/Counted.h>
#include <zxing/Binarizer.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/Result.h>
#include <zxing/ReaderException.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/Exception.h>
#include <zxing/common/IllegalArgumentException.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/DecodeHints.h>
#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/MatSource.h>
#include <zxing/datamatrix/DataMatrixReader.h>

// opencv 支持
#include <opencv2/opencv.hpp>

// fork支持
#include <sys/types.h>
#include <unistd.h>

using namespace QRCode;
using namespace std;
using namespace cv;

ZXing::~ZXing()
{

}

string ZXing::parse()
{
    errStr_ = "";
    eError_ = kNoError;

    string result;

    if (methodName_ == ZXing::kUseLib)
    {
        result = parseByLib();
    }
    else if (methodName_ == ZXing::kUseExec)
    {
        result = parseByExec();
    }

    return result;
}

string ZXing::parseByLib()
{
    cv::Mat matSrc = cv::imread(qrPath_, 1);
    if (!matSrc.data) 
    {   
        eError_ = kParseError;

        char buf[100];

        sprintf(buf, "read image error: %s, \n", qrPath_.c_str());
        errStr_ = buf;
        
        return "";
    }

    string content;
    
    cv::Mat matGray;
    cv::cvtColor(matSrc, matGray, COLOR_BGR2GRAY);

   // try 
    {
        zxing::Ref<zxing::LuminanceSource> source = MatSource::create(matGray);
        int width = source->getWidth();
        int height = source->getHeight();
        fprintf(stdout, "image width: %d, height: %d\n", width, height);

        zxing::Ref<zxing::Reader> reader;
        reader.reset(new zxing::datamatrix::DataMatrixReader);

        zxing::Ref<zxing::Binarizer> binarizer(new zxing::GlobalHistogramBinarizer(source));
        zxing::Ref<zxing::BinaryBitmap> bitmap(new zxing::BinaryBitmap(binarizer));
        //开始解码
        zxing::Ref<zxing::Result> result(
                reader->decode(bitmap, zxing::DecodeHints(zxing::DecodeHints::DATA_MATRIX_HINT)));

        std::string str2 = result->getText()->getText();

        printf("str2: %s\n", str2);
        content += str2;
    }
    // catch (zxing::Exception &e) 
    // {
    //     //Export your failure to read the code here
    //     eError_ = kParseError;

    //     char buf[100];

    //     fprintf(stderr, "read error: %s, %s\n", qrPath_.c_str(), e.what());
        
    //     errStr_ = buf;
    // }

    return content;
}

string ZXing::parseByExec()
{
    char dir[256];
    getcwd(dir, 256);
    
    char cmd[1024];
    sprintf(cmd, "/home/jim/code/QRCode/build/bin/zxing %s", qrPath_.c_str());
    
    FILE *fstream = NULL;
    fstream = popen(cmd, "r"); // 从管道中读入

    string buffer;
    char buff[1024];

    if (fstream)
    {
        while (fgets(buff, sizeof(buff), fstream))
        {
            buffer += buff;
        }
        
        return buffer;
    }
    else
    {
        printf("error --------------\n");
    }

    return "";
}