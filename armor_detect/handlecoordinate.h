#ifndef HANDLECOORDINATE_H
#define HANDLECOORDINATE_H
#include "serialport.h"
#include "method.h"
#include "vector"
#include "algorithm"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;




class HandleCoordinate
{
public:
    void findCoor(Mat src_img, Mat dst_img);
    void sendCoor();

private:
    vector<Point2f> coordinate_center;


};

#endif // HANDLECOORDINATE_H
