#ifndef V4L2_H
#define V4L2_H

#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include<iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
class V4l2
{
public:
    V4l2(const char* file, int exposure_value, int saturation_value);
    void set_v4l2();
    void get_v4l2();

private:
    const char* _file;
    int exposure;
    int saturation;
};



#endif // V4L2_H
