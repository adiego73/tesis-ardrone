#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "structures.hpp"
#include <math.h>

namespace tesis
{

class Video
{

    public:
        Point trackColor(Color color);
        cv::Mat getFrame();
        cv::Size getFrameSize();

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
