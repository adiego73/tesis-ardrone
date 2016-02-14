#include "core/data/videodata.hpp"

using namespace tesis;

VideoData::VideoData()
{

}

VideoData::~VideoData()
{

}

cv::Mat VideoData::readFrame()
{
    boost::lock_guard<boost::mutex> lock(this->mutex);
    return this->frame;
}

void VideoData::updateFrame( cv::Mat frame )
{
    boost::lock_guard<boost::mutex> lock(this->mutex);
        
    this->frame = frame.clone();
}
