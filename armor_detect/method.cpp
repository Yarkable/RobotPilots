#include "method.h"



using namespace cv;
using namespace std;

Mat Method::yuvRed()
{
    gamma();
    cvtColor(src_img, dst_img, COLOR_BGR2YUV);
    inRange(dst_img, Scalar(0, 150, 110), Scalar(255, 180, 180), dst_img);
    morphologyEx(dst_img, dst_img, MORPH_OPEN, Mat(5, 5, CV_8U, Scalar(1)));
    morphologyEx(dst_img, dst_img, MORPH_CLOSE, Mat());
    return dst_img;
}

Mat Method::yuvBlue()
{
    Mat y, u, v;
    vector<Mat> split_img;
    cvtColor(src_img, dst_img, COLOR_BGR2YUV);
    split(dst_img, split_img);
    y = split_img[0];
    u = split_img[1];
    v = split_img[2];
    return v;

//    const Scalar low = 139 + 5;
//    const Scalar high = 255;
//    inRange(v, low, high, src_img);
//    return src_img;
}

Mat Method::labRed()
{
    // Mat dst_img = Mat(src_img.size(), src_img.type());
    gamma();
    cvtColor(src_img, dst_img, COLOR_BGR2Lab);
    const Scalar low(1, 154, 130);
    const Scalar high(255, 255, 255);
    inRange(dst_img, low, high, dst_img);
    morphologyEx(dst_img, dst_img, MORPH_OPEN, Mat(6, 6, CV_8U, Scalar(1)));
    morphologyEx(dst_img, dst_img, MORPH_CLOSE, Mat(6, 6, CV_8U, Scalar(1)));
    return dst_img;
}

Mat Method::labBlue()
{
    // Mat dst_img = Mat(dst_img.size(), dst_img.type());
    cvtColor(src_img, dst_img, COLOR_BGR2Lab);
    vector<Mat> a;
    split(dst_img, a);
    inRange(a[2], Scalar(80), Scalar(116), dst_img);
    morphologyEx(dst_img, dst_img, MORPH_OPEN, Mat());
    morphologyEx(dst_img, dst_img, MORPH_CLOSE, Mat());
    return dst_img;
}

Mat Method::hsvRed()
{
    Mat temp_img, dst_img = Mat(src_img.rows, src_img.cols, CV_8UC1);
    cvtColor(src_img, temp_img, COLOR_BGR2HSV);
    // imshow("temp_img", temp_img);
    for (int i = 0; i < src_img.rows; i++)
    {
        Vec3b* p = temp_img.ptr<Vec3b>(i);
        uchar* p1 = dst_img.ptr<uchar>(i);
        for (int j = 0; j < src_img.cols; j++)
        {
            // 根据需要调节参数值
            bool cdtH = (p[j][0] <= 6) || (p[j][0] >= 156 && p[j][0] <= 180);
            bool cdtS = (p[j][1] >= 43);
            bool cdtV = (p[j][2] >= 35);
            if (cdtH && cdtS && cdtV)
                p1[j] = 255;
            else
                p1[j] = 0;
        }
    }
    morphologyEx(dst_img, dst_img, MORPH_OPEN, Mat(9, 9, CV_8U, Scalar(1)));
    morphologyEx(dst_img, dst_img, MORPH_CLOSE, Mat());
    return dst_img;
}

Mat Method::hsvBlue()
{
    Mat temp_img, dst_img = Mat(src_img.rows, src_img.cols, CV_8UC1);
    cvtColor(src_img, temp_img, COLOR_BGR2HSV);

    // imshow("temp_img", temp_img);

    for (int i = 0; i < src_img.rows; i++)
    {
        Vec3b* p = temp_img.ptr<Vec3b>(i);
        uchar* p1 = dst_img.ptr<uchar>(i);
        for (int j = 0; j < src_img.cols; j++)
        {
            // 根据需要调节参数值
            bool cdtH = (p[j][0] >= 100 && p[j][0] <= 122);
            bool cdtS = (p[j][1] >= 45);
            bool cdtV = (p[j][2] >= 36);
            if (cdtH && cdtS && cdtV)
                p1[j] = 255;
            else
                p1[j] = 0;
        }
    }
    morphologyEx(dst_img, dst_img, MORPH_OPEN, Mat(18, 18, CV_8U, Scalar(1)));
    morphologyEx(dst_img, dst_img, MORPH_CLOSE, Mat());
    return dst_img;
}


Mat Method::detectRedArmor()
{
    VideoCapture cap;
    Mat frame;

    frame = cap.open("/home/kevin/视频/素材/10red_1.avi");
    if (!cap.isOpened())
    {
        cout << "file not find!" << endl;
    }

    // loop to process every frame
    while (cap.read(frame))
    {
        Mat preProcessImg;

        Mat binary_frame = Mat(frame.rows, frame.cols, CV_8UC1);
        cvtColor(frame, preProcessImg, COLOR_BGR2HSV);
        // split hsv image and merge to decrease the lightness

        vector<Mat> plane;
        split(preProcessImg, plane);
        plane[2] = plane[2] - 30 - 20 ;
        merge(plane, preProcessImg);

        Mat darkerImg = preProcessImg.clone();
        cvtColor(darkerImg, darkerImg, COLOR_HSV2BGR);

        for (int i = 0; i < frame.rows; i++)
        {
            Vec3b* p = preProcessImg.ptr<Vec3b>(i);
            uchar* p1 = binary_frame.ptr<uchar>(i);
            for (int j = 0; j < frame.cols; j++)
            {

                bool cdtH = (p[j][0] <= 6 + 4) || (p[j][0] >= 156 && p[j][0] <= 180);
                bool cdtS = (p[j][1] >= 43);
                bool cdtV = (p[j][2] >= 35 - 20);
                if (cdtH && cdtS && cdtV)
                    p1[j] = 255;
                else
                    p1[j] = 0;
            }
        }
        // morphologyEx(binary_frame, binary_frame, MORPH_OPEN, Mat(8, 8, CV_8U, Scalar(1)));
        // morphologyEx(binary_frame, binary_frame, MORPH_CLOSE, Mat(3 , 3 , CV_8U, Scalar(1)));
        Mat element = getStructuringElement(MORPH_RECT, Size(7, 7), Point(-1, -1));
        dilate(binary_frame, binary_frame, element);


        vector< vector<Point> > contours;
        vector<Vec4f> lanterns_msg;
        vector<Vec3f> minDiameterMsg;


        // find contours in binary image
        findContours(binary_frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < contours.size(); i++)
        {


            // if (contourArea(contours[i]) <= 50)
            //      continue



            RotatedRect rRect = minAreaRect(contours[i]);
            Point2f points[4];
            rRect.points(points);
            double width = rRect.size.width;
            double height = rRect.size.height;
            double appearent_height;
            double area = rRect.size.area();
            float angle = rRect.angle;
            float center_x = rRect.center.x;
            float center_y = rRect.center.y;
            // discard the ones whose area are too small
            if (area > 36)

            {
                if ((height - width < -6) && (angle < -66) ||
                        (height - width > 6) && (angle > -30) )
                {
                    if (angle > -45)
                    {
                        appearent_height = height;
                    }

                    else
                    {
                        appearent_height = width;
                    }

                    for (int i = 0; i < 4; i++)
                    {
                        int j = (i + 1) % 4;
                        line(darkerImg, points[i], points[j], Scalar(0, 255, 0), 1);
                    }
                // cout << contourArea(contours[i]) << endl;
                // cout << "Center Position: " << rRect.center << endl;
                    Vec4f msg(appearent_height, center_x, center_y, angle);

                    lanterns_msg.push_back(msg);
                }
            }

        }

        for (int i = 0; i < lanterns_msg.size(); i++)
        {
            for (int j = i + 1; j < lanterns_msg.size(); j++)
            {
                double height1 = lanterns_msg[i][0];
                double height2 = lanterns_msg[j][0];
                double center_x1 = lanterns_msg[i][1];
                double center_x2 = lanterns_msg[j][1];
                double center_y1 = lanterns_msg[i][2];
                double center_y2 = lanterns_msg[j][2];
                double angle1 = lanterns_msg[i][3];
                double angle2 = lanterns_msg[j][3];
                double diameter = sqrt(pow(abs(center_x1 - center_x2), 2) + pow(abs(center_y1 - center_y2), 2));
                Point2f centerCoordinate = Point2f((center_x1 + center_x2) / 2, (center_y1 + center_y2) / 2);
//                double area1 = lanterns_msg[i][2];
//                double area2 = lanterns_msg[j][2];
                double diffVertcal = abs(center_y1 - center_y2);
                double diffAngle = abs(angle1 - angle2);
                double diffHeight = abs(height1 - height2);
                double biasVertical = 20;
                double biasAngle = 9.6;
                double biasHeight = 20;

                Vec3f diameterMsg(diameter, centerCoordinate.x, centerCoordinate.y);
                if (  //diffAngle < biasAngle &&
                        diffVertcal < biasVertical &&
                        diameter >= 36 && diameter <= 108 + 30 &&
                        diffHeight < biasHeight)
                    minDiameterMsg.push_back(diameterMsg);


//                {
//                    circle(darkerImg, centerCoordinate, diameter / 2, Scalar(0, 255, 255), 1);
//                    cout << "diameter: " << diameter << "  diffAngle :" << diffAngle << endl;
//                }

            }
        }

        Mat ROI;

        // at least find one
        if (minDiameterMsg.size() > 0)
        {
            int min = 0;
            for (int i = 0; i < minDiameterMsg.size(); i++)
            {
                if (minDiameterMsg[i][0] < minDiameterMsg[min][0])
                    min = i;
            }

            circle(darkerImg, Point2f(minDiameterMsg[min][1], minDiameterMsg[min][2]), minDiameterMsg[min][0] / 2.0, Scalar(255, 0, 255), 3);

            ROI = darkerImg(Rect(minDiameterMsg[min][1] - minDiameterMsg[min][0] / 2.0 * 1.5,
                                     minDiameterMsg[min][2] - minDiameterMsg[min][0] / 2.0 * 1.5,
                                     int(minDiameterMsg[min][0] / 2 * 3), int(minDiameterMsg[min][0] / 2 * 3)));

        }

            imshow("ROI", ROI);


        imshow("src", frame);
        imshow("binary", binary_frame);
        imshow("frame", darkerImg);

        waitKey(10);
    }

}


/////////////////////////////////////////////////////////////////
//@projectName   armor_detect
//@author        kevin
//@brief         根据上一帧进行ROI截图操作，当上一帧存在ROI时，flag = 1,
//               否则flag = 0，当flag不同时坐标计算也不同，要保存上一帧的
//               状态在一个vector中
//@date          2018_10_28
/////////////////////////////////////////////////////////////////


void Method::detectBlueArmor(VideoCapture& cap)
{


    Mat frame;
    cap >> frame;

    bool flag = 0, roi = 0;
    vector<Vec4f> lastFrame; /* 定义上一帧 */

    /* loop to process every frame */
    while (cap.read(frame))
    {
        Mat clone = frame.clone();

        if (roi)
        {
            if (flag) //exists ROI
            {
                frame = frame(Rect(lastFrame[0][0], lastFrame[0][1], int(lastFrame[0][2]), int(lastFrame[0][3])));
            }
        }


        Mat preProcessImg;
        Mat binary_frame = Mat(frame.rows, frame.cols, CV_8UC1);
        cvtColor(frame, preProcessImg, COLOR_BGR2HSV);

        /* split hsv image and merge to decrease the lightness */
        vector<Mat> plane;
        split(preProcessImg, plane);
        plane[2] = plane[2] - 30 - 30 - 10 - 10;
        merge(plane, preProcessImg);

        /* get hsv img */
        Mat darkerImg = preProcessImg.clone();
        cvtColor(darkerImg, darkerImg, COLOR_HSV2BGR);

        /* get binary img */
        for (int i = 0; i < frame.rows; i++)
        {
            Vec3b* p = preProcessImg.ptr<Vec3b>(i);
            uchar* p1 = binary_frame.ptr<uchar>(i);
            for (int j = 0; j < frame.cols; j++)
            {

                bool cdtH = (p[j][0] >= 100 && p[j][0] <= 122);
                bool cdtS = (p[j][1] >= 45);
                bool cdtV = (p[j][2] >= 36 - 5);
                if (cdtH && cdtS && cdtV)
                    p1[j] = 255;
                else
                    p1[j] = 0;
            }
        }
        // morphologyEx(binary_frame, binary_frame, MORPH_OPEN, Mat(8, 8, CV_8U, Scalar(1)));
        // morphologyEx(binary_frame, binary_frame, MORPH_CLOSE, Mat(3 , 3 , CV_8U, Scalar(1)));
        Mat element = getStructuringElement(MORPH_RECT, Size(4 + 1, 4 + 1), Point(-1, -1));
        dilate(binary_frame, binary_frame, element);


        /* define vectors to contain msg */
        vector< vector<Point> > contours;
        vector<Vec4f> lanterns_msg;
        vector<Vec3f> minDiameterMsg;



        /* find contours in binary image */
        findContours(binary_frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        for (int i = 0; i < contours.size(); i++)
        {


            // if (contourArea(contours[i]) <= 50)
            //      continue



            RotatedRect rRect = minAreaRect(contours[i]);
            Point2f points[4];
            rRect.points(points);
            double width = rRect.size.width;
            double height = rRect.size.height;
            double appearent_height;
            double area = rRect.size.area();
            float angle = rRect.angle;
            float center_x = rRect.center.x;
            float center_y = rRect.center.y;

            /* discard the ones whose area are too small */
            if (area > 66)
            {
                if ((height - width < -6) && (angle < -66) ||
                        (height - width > 6) && (angle > -30) )
                {
                    if (angle > -45)
                    {
                        appearent_height = height;
                    }

                    else
                    {
                        appearent_height = width;
                    }

                    for (int i = 0; i < 4; i++)
                    {
                        int j = (i + 1) % 4;
                        line(darkerImg, points[i], points[j], Scalar(0, 255, 0), 1);
                    }
                // cout << contourArea(contours[i]) << endl;
                // cout << "Center Position: " << rRect.center << endl;
                    Vec4f msg(appearent_height, center_x, center_y, angle);

                    lanterns_msg.push_back(msg);
                }
            }

        }


        /*   find the actrual lanterns */
        for (int i = 0; i < lanterns_msg.size(); i++)
        {
            for (int j = i + 1; j < lanterns_msg.size(); j++)
            {


                double height1 = lanterns_msg[i][0];
                double height2 = lanterns_msg[j][0];
                double center_x1 = lanterns_msg[i][1];
                double center_x2 = lanterns_msg[j][1];
                double center_y1 = lanterns_msg[i][2];
                double center_y2 = lanterns_msg[j][2];
                double angle1 = lanterns_msg[i][3];
                double angle2 = lanterns_msg[j][3];
                double diameter = sqrt(pow(abs(center_x1 - center_x2), 2) + pow(abs(center_y1 - center_y2), 2));
                Point2f centerCoordinate = Point2f((center_x1 + center_x2) / 2, (center_y1 + center_y2) / 2);
//                double area1 = lanterns_msg[i][2];
//                double area2 = lanterns_msg[j][2];
                double diffVertcal = abs(center_y1 - center_y2);
                double diffAngle = abs(angle1 - angle2);
                double diffHeight = abs(height1 - height2);
                double biasVertical = 20;
                double biasAngle = 9.6;
                double biasHeight = 20;

                Vec3f diameterMsg(diameter, centerCoordinate.x, centerCoordinate.y);
                if (diffVertcal < biasVertical &&
                        diameter >= 40 && diameter <= 108 &&
                        diffHeight < biasHeight)

                    minDiameterMsg.push_back(diameterMsg);


//                {
//                    circle(darkerImg, centerCoordinate, diameter / 2, Scalar(0, 255, 255), 1);
//                    cout << "diameter: " << diameter << "  diffAngle :" << diffAngle << endl;
//                }

            }
        }

        Mat ROI ;

        /* sort, find the minimal daimeter and circle it */
        if (minDiameterMsg.size() > 0)
        {
            int min = 0;
            for (int i = 0; i < minDiameterMsg.size(); i++)
            {
                if (minDiameterMsg[i][0] < minDiameterMsg[min][0])
                    min = i;
            }

            double roi_left_top_x = minDiameterMsg[min][1] - minDiameterMsg[min][0] / 2.0 * 1.5;
            if (roi_left_top_x <= 0)
                roi_left_top_x = 0;
            double roi_left_top_y = minDiameterMsg[min][2] - minDiameterMsg[min][0] / 2.0 * 1.5;
            if (roi_left_top_y <= 0)
                roi_left_top_y = 0;
            int radius_x = int(minDiameterMsg[min][0] / 2 * 3);
            if (radius_x >= int(darkerImg.cols - roi_left_top_x))
                radius_x = int(darkerImg.cols - roi_left_top_x);
            int radius_y = int(minDiameterMsg[min][0] / 2 * 3);
            if (radius_y >= int(darkerImg.rows - roi_left_top_y))
                radius_y = int(darkerImg.rows - roi_left_top_y);

            ROI = darkerImg(Rect(roi_left_top_x,roi_left_top_y, radius_x, radius_y));


            if (flag)
            {
                circle(clone, Point2f(minDiameterMsg[min][1] + lastFrame[0][0], minDiameterMsg[min][2] + lastFrame[0][1]),
                        minDiameterMsg[min][0] / 2.0, Scalar(147, 20, 255), 2);
                cout << Point2f(minDiameterMsg[min][1] + lastFrame[0][0], minDiameterMsg[min][2] + lastFrame[0][1]) << endl;
                Vec4f roiMsg(roi_left_top_x + lastFrame[0][0],roi_left_top_y + lastFrame[0][1],
                            radius_x, radius_y);
                lastFrame.clear();
                lastFrame.push_back(roiMsg);
            }
            if (!flag)
            {
                circle(clone, Point2f(minDiameterMsg[min][1], minDiameterMsg[min][2]),
                        minDiameterMsg[min][0] / 2.0, Scalar(147, 20, 255), 2);
                cout << Point2f(minDiameterMsg[min][1], minDiameterMsg[min][2]) << endl;
                Vec4f roiMsg(roi_left_top_x,roi_left_top_y,radius_x, radius_y);
                lastFrame.clear();
                lastFrame.push_back(roiMsg);
            }
        }



        if (!ROI.empty())
        {
            imshow("ROI", ROI);
            roi = true;
            flag = true;
           // ROI_search_region.push_back(ROI);

        }
        else
        {
            roi = false;
            flag = false;
        }



        imshow("clone", clone);
        imshow("src", frame);
        imshow("binary", binary_frame);
        imshow("light_adjusted", darkerImg);

        waitKey(0);
    }
}



















void Method::histoEqual()
{
    if (src_img.channels() != 1)
    {
        vector<Mat>temp;
        split(src_img, temp);
        for (int i = 0; i < src_img.channels(); i++)
        {
            equalizeHist(temp[i], temp[i]);
        }
        merge(temp, dst_img);
    }
}


void Method::gamma()
{
    vector<Mat> split_img;

    split(src_img, split_img);
    for (int i = 0; i < 3; i ++)
    {
        split_img[i].convertTo(split_img[i], CV_32F, 1.0 / 255, 0);
        // 这是提高亮度检测颜色
        // pow(split_img[i], 0.5, split_img[i]);
        // 这是降低亮度检测装甲板
        pow(split_img[i], 4 - 1 - 1, split_img[i]);
        split_img[i].convertTo(split_img[i], CV_8U, 255, 0);
    }
    merge(split_img,src_img);
}


void Method::set_img(Mat img)
{
    src_img = img;
    if (src_img.empty())
    {
        std::cout << "loading img error!" << std::endl;
    }
}



void Method::showSrcImg()
{

    imshow("src_img", src_img);
    waitKey(33);
}

Mat Method::getImg(int n)
{
    if (n == 0)
        return src_img;
    if (n == 1)
        return dst_img;
}




//    // 采集素材
////    cam.set(CV_CAP_PROP_FRAME_WIDTH,1280);
////    cam.set(CV_CAP_PROP_FRAME_HEIGHT,720);

//    Mat frame;
//    VideoCapture cap(0);
//    if (!cap.isOpened())
//    {
//        cout << "camera not find!" << endl;
//    }
////        cap.set(CV_CAP_PROP_FRAME_WIDTH,1280);
////        cap.set(CV_CAP_PROP_FRAME_HEIGHT,720);
//    int fps = cap.get(CV_CAP_PROP_FPS);
//    Size size = Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
//    VideoWriter write;

//    write.open("opencv1.avi", CV_FOURCC('M', 'P', 'E', 'G'), fps, size);
//    cout << fps << endl;
//    while(1)
//    {
//        cap >> frame;
//        imshow("frame", frame);
//        write << frame;
//        waitKey(10);
//    }

//=====================another job===========================




//    while (true)
//    {
//        if (!cam.isOpened())
//        {
//            cout << "error" << endl;
//            break;
//        }

//        Mat frame, src_img, dst_img;
//        cam >> frame;

//        Method videoProcess;
//        videoProcess.set_img(frame);
//        src_img = videoProcess.getSrcImg();
//        dst_img = videoProcess.hsvRed();

//        HandleCoordinate handle;
//        handle.findCoor(dst_img, src_img);
//        videoProcess.showSrcImg();


//    }



//            // sort by y
//            for(int i = 0; i < 4; i++)
//            {
//                for(int j = i + 1; j < 4; j++)
//                {
//                    if(points[i].y > points[j].y)
//                    {
//                        Point2f temp;
//                        temp = points[i];
//                        points[i] = points[j];
//                        points[j] = temp;
//                    }
//                }
//            }
//            //sort by x
//            if(points[0].x > points[1].x)
//            {
//                Point2f temp0;
//                temp0 = points[0];
//                points[0] = points[1];
//                points[1] = temp0;
//            }
//            if(points[2].x < points[3].x)
//            {
//                Point2f temp1;
//                temp1 = points[2];
//                points[2] = points[3];
//                points[3] = temp1;
//            }

//            double width_x = abs(points[1].x - points[0].x);
//            double width_y = abs(points[1].y - points[0].y);
//            double height_x = abs(points[2].x - points[1].x);
//            double height_y = abs(points[2].y - points[1].y);
//            double width = sqrt(pow(width_x, 2) + pow(width_y, 2));
//            double height = sqrt(pow(height_x, 2) + pow(height_y, 2));

//            if ((height < width) || (width > 20))
//                continue;
