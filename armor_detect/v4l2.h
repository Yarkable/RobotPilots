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
    void v4l2_set(const char* _file, int exposure, int saturation);
};


#endif // V4L2_H
