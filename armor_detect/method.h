#ifndef METHOD_H
#define METHOD_H
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "time.h"
#include "iostream"
#include "algorithm"
#include "vector"
#include "string"

using namespace cv;
class Method
{   
public:
    Mat yuvRed();
    Mat yuvBlue();
    Mat labRed();
    Mat labBlue();
    Mat hsvRed();
    Mat hsvBlue();
    Mat detectRedArmor();
    void detectBlueArmor(VideoCapture &cap);
    void gamma();
    void histoEqual();
    void set_img( Mat img);
    void showSrcImg();
    Mat getImg(int n);
private:
    Mat src_img;
    Mat dst_img;
};

#endif // METHOD_H
