#include <iostream>

#include "parse.h"

using namespace QRCode;
using namespace std;

int main(int argc, char *argv[])
{
    std::string qrPath = "./images/helloworld1.png";

    if (argc == 2)
    {
        qrPath = argv[1];
    }

    QRParse oParse(qrPath, kZXing); // , kZXing

    string text = oParse.parse();

    cout << "text: "  << endl
        << endl
        << text << endl;

    return 0;
}