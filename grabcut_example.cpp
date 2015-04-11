#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int LU_X,LU_Y,RB_X,RB_Y;
int status=0;
Mat Mask;
void MouseCallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
	status++;
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	if(status==1)
	{
		LU_X=x;
		LU_Y=y;
	}
	else if(status==2)
	{
		RB_X=x;
		RB_Y=y;
	}
     }

     if  ( event == EVENT_RBUTTONDOWN )
     {
        cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	Mask.data[Mask.cols*x+y]=1;
     }
}

int main( )
{
 // Open another image
    Mat image;
    image= cv::imread("1.JPG");
    cv::namedWindow("OriginalImage");
    cv::imshow("OriginalImage",image);
    setMouseCallback("OriginalImage",MouseCallBackFunc,NULL);
    Mask.rows=image.rows;
    Mask.cols=image.cols;
    // define bounding rectangle 
    //cv::Rect rectangle(250,270,image.cols-250,image.rows-280);
    //while(status<2);
    waitKey(0);
    cv::Rect rectangle(LU_X,LU_Y,RB_X-LU_X,RB_Y-LU_Y);

    cv::Mat result; // segmentation result (4 possible values)
    cv::Mat bgModel,fgModel; // the models (internally used)

    // GrabCut segmentation
    cv::grabCut(image,    // input image
                    result,   // segmentation result
                            rectangle,// rectangle containing foreground 
                            bgModel,fgModel, // models
                            10,        // number of iterations
                            cv::GC_INIT_WITH_RECT); // use rectangle
    // Get the pixels marked as likely foreground
    cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
    // Generate output image
    cv::Mat foreground(image.size(),CV_8UC3,cv::Scalar(255,255,255));
    image.copyTo(foreground,result); // bg pixels not copied

    // draw rectangle on original image
    cv::rectangle(image, rectangle, cv::Scalar(255,255,255),1);
    cv::namedWindow("ImageWithRectangle");
    cv::imshow("ImageWithRectangle",image);

    // display result
    cv::namedWindow("Segmented Image");
    cv::imshow("Segmented Image",foreground);
    waitKey();
    cv::grabCut(image,    // input image
                    Mask,   // segmentation result
                            rectangle,// rectangle containing foreground 
                            bgModel,fgModel, // models
                            10,        // number of iterations
                            cv::GC_INIT_WITH_MASK); // use rectangle
     
    return 0;
}


