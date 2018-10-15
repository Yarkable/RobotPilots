#include "handlecoordinate.h"
using namespace std;
using namespace cv;

void HandleCoordinate::findCoor(Mat binary_img, Mat background_img)
{
    vector< vector<Point> > contours;
    vector<RotatedRect> armors;


    findContours(binary_img, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); i++)
    {

        RotatedRect rRect = minAreaRect(contours[i]);
        Point2f points[4];
        rRect.points(points);

        for (int i = 0; i < 4; i++)
        {
            int j = (i + 1) % 4;
            line(background_img, points[i], points[j], Scalar(0, 255, 0), 1);
        }
    cout << "Center Position: " << rRect.center << endl;
    }
}




void HandleCoordinate::sendCoor()
{
    Point coor;
    SerialPort port;

    int x = coor.x;
    int y = coor.y;
    vector<char> msg_x;
    vector<char> msg_y;

    for (int i = 0; ; i ++)
    {
        int x_remainder = 0;
        int y_remainder = 0;
        char x_remainder_char = '0';
        char y_remainder_char = '0';

        if (x != 0)
        {
            x_remainder = x % 10;
            x_remainder_char = x % 10 + '0';
            x = x / 10;
            msg_x.push_back(x_remainder_char);
        }

        if (y != 0)
        {
            y_remainder = y % 10;
            y_remainder_char = y % 10 + '0';
            y = y / 10;
            msg_y.push_back(y_remainder_char);
        }

        if (x == 0 && y == 0)
        {
            break;
        }
    }
    char coordinate[9] = {'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E'};
    int counter = 0;
    coordinate[counter++] = 'X';
    for (int count_x = 0; count_x < msg_x.size(); count_x++)
    {
        coordinate[counter] = msg_x[msg_x.size() - count_x - 1];
        counter++;
    }
    coordinate[counter++] = 'Y';
    for (int count_y = 0; count_y < msg_y.size(); count_y++)
    {
        coordinate[counter] = msg_y[msg_y.size() - count_y - 1];
        counter++;
    }
    port.Send(coordinate);
}
