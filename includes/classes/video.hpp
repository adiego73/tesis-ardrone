#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <opencv.hpp>
#include <opencv2/video.hpp>
#include <assert.h>

# include "structures.hpp"

namespace tesis
{

class Video
{

    public:
        std::vector<Point> transformToBlackAndWhite ( cv::Mat &image, const Color color );
        std::vector<std::vector<Point>> transformToBlackAndWhite ( cv::Mat &image, const std::vector<Color> color, int length);
        cv::Mat getFrame();
        cv::Size getFrameSize();
        
        Video(int camera);
        ~Video();

    private:
        cv::VideoCapture cap;
        cv::Mat frame;
        cv::Size img_size;
        int fps;


};



};
#endif // VIDEO_HPP
