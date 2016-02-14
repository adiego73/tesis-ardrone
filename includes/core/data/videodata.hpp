#ifndef VIDEODATA_H
#define VIDEODATA_H

#include <opencv.hpp>

#include <boost/thread.hpp>

namespace tesis
{

class VideoData
{
    public:
        VideoData();
        ~VideoData();

        void updateFrame( cv::Mat frame );
        cv::Mat readFrame();

    private:
        boost::mutex mutex;
        cv::Mat frame;
};

}
#endif // VIDEODATA_H
