#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "structures.hpp"

namespace tesis
{

class Video
{

    public:
        Point trackColor(Color color);
        cv::Mat getFrame();
        cv::Size getFrameSize();
        float getFrameWidth();
        float getFrameHeight();
        void capture();
        void drawPoint(Point point, int thickness = 1);

        Video( int camera );
        ~Video();

    private:
        cv::VideoCapture cap;
        cv::Mat frame;
        cv::Size img_size;
        int fps;
};



};
#endif // VIDEO_HPP
