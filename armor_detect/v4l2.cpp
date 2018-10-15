#include "v4l2.h"
#include "fcntl.h"
#include "unistd.h"
#include <linux/videodev2.h>
#include <sys/ioctl.h>
using namespace std;
using namespace cv;

void V4l2::v4l2_set(const char* _file, int exposure, int saturation)
{
    int Handle0 = open(_file, O_RDWR);
    struct v4l2_control ctrl0;
    ctrl0.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    ctrl0.value = exposure;
    ioctl(Handle0, VIDIOC_S_CTRL, &ctrl0);


    struct v4l2_control ctrl1;
    ctrl1.id = V4L2_CID_SATURATION;
    ctrl1.value = saturation;
    ioctl(Handle0, VIDIOC_S_CTRL, &ctrl1);


//    struct v4l2_control ctrl1;
//    ctrl1.id = V4L2_AUTO_FOCUS_RANGE_AUTO;


    cout << "exposure: " << exposure << endl;
    cout << "saturation: " << saturation << endl;

//    cout <<"white_balance: " << white_balance << endl;
//    cout <<"gain: " << gain << endl;


}
