#include "v4l2.h"
#include "fcntl.h"
#include "unistd.h"
#include <linux/videodev2.h>
#include <sys/ioctl.h>
using namespace std;
using namespace cv;

V4l2::V4l2(const char* file, int exposure_value, int saturation_value)
{
    _file = file;
    exposure = exposure_value;
    saturation = saturation_value;
}


void V4l2::set_v4l2()
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

}

void V4l2::get_v4l2()
{
    cout << "cam param" << endl;
    cout << "exposure: " << exposure << endl;
    cout << "saturation: " << saturation << endl;
}
