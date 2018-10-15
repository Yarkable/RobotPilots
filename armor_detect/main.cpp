#include <QCoreApplication>
#include "method.h"
#include "v4l2.h"
//#include "serialport.h"
#include "handlecoordinate.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
        V4l2 set_cam;
        set_cam.v4l2_set("/dev/video1", 10, 100);




    Method armor;

    armor.detectBlueArmor();
    // armor.detectRedArmor();





//    V4l2 set_cam;
//    set_cam.v4l2_set("/dev/video1", 60, 100);
////    VideoCapture cam(0);

////    if (!cam.isOpened())
////    {
////        cout << "camera not find!" << endl;
////    }

//        Method armor;
//        Mat img = imread("/home/kevin/armor1.jpg");
//        Mat src_img, dst_img;
//        armor.set_img(img);

//        src_img = armor.getImg(0);

//        armor.gamma();


//        dst_img = armor.detectBlueArmor();

//        HandleCoordinate handle;
//        handle.findCoor(dst_img, src_img);
//        armor.showSrcImg();
//        imshow("dst_img", dst_img);
//        waitKey(0);






    return a.exec();
}
