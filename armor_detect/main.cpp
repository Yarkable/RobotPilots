#include "method.h"
#include "v4l2.h"
//#include "serialport.h"
#include "handlecoordinate.h"



using namespace std;
using namespace cv;

int main()
{

    int mode = 0;
    V4l2 camera("/dev/video1", 100, 100);
    camera.set_v4l2();
    camera.get_v4l2();

    Method armorDetect;
    if (mode == 0)  // read loacl video
    {
        VideoCapture cap("/home/kevin/out_blue_90.avi");
        if (!cap.isOpened())
        {
            cout << "camera not find!" << endl;
        }
        armorDetect.detectBlueArmor(cap);
    }

    if (mode == 1)  // capture camera
    {
        VideoCapture cap(1);
        if (!cap.isOpened())
        {
            cout << "camera not find!" << endl;
        }
        armorDetect.detectBlueArmor(cap);
    }







    return 0;
}
