#include "zbar.h"

#include <iostream>    
  
#include <opencv2/opencv.hpp>
#include <zbar/zbar.h>

using namespace std;    
using namespace zbar;  
using namespace cv;    

using namespace QRCode;

ZBar::ZBar(std::string qrPath)
{
    qrPath_ = qrPath;
    eError_ = kNoError;
}

void ZBar::init()
{

}

string ZBar::parse()
{
    eError_ = kNoError;

    ImageScanner scanner;    
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);  

	Mat img = imread(qrPath_,0);    
	Mat imgout;    

	cvtColor(img,imgout,CV_GRAY2RGB);    

	int width = img.cols;    
	int height = img.rows;    
	uchar *raw = (uchar *)img.data;     

	Image image(width, height, "Y800", raw, width * height);   

	int n = scanner.scan(image); 

    string result = "";

	for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)  
	{    
		vector<Point> vp;   

		cout<<"Decoded：êo"<<endl<<symbol->get_type_name()<<endl<<endl;  
		cout<<"Symbol：êo"<<endl<<symbol->get_data()<<endl<<endl;     

        result += symbol->get_data();

		int n = symbol->get_location_size();    

		for (int i=0; i<n; i++)  
		{    
			vp.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));   
		}    

		RotatedRect r = minAreaRect(vp);    
		Point2f pts[4];    
		r.points(pts);    
		
        Point textPoint(pts[1]);  
		
        string codeNum=symbol->get_data();  

		for(int i=0; i<4; i++)  
		{    
			line(imgout, pts[i], pts[(i+1) % 4], Scalar(255, 0, 0), 3);

			textPoint.x >= pts[i].x ? textPoint.x = pts[i].x : textPoint.x = textPoint.x;  
			textPoint.y >= pts[i].y ? textPoint.y = pts[i].y : textPoint.y = textPoint.y;  
	        
            putText(imgout, codeNum, textPoint, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 1, 8, false);               
		}    

		cout << "Angle: " << r.angle << endl;    
	}    

	//imshow("imgout.jpg", imgout);      
	//waitKey(); 

    return result;
}