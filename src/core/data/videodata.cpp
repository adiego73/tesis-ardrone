#include "core/data/videodata.hpp"

#include <iostream>

using namespace tesis;

VideoData::VideoData()
{

}

VideoData::~VideoData()
{

}

cv::Mat VideoData::readFrame()
{
    boost::lock_guard<boost::mutex> lock( this->mutex );

    if( this->frame.empty() )
    {
        cv::Size s( 640, 480 );
        cv::Mat black_frame( s, CV_8UC3 );

        return black_frame;
    }

    return this->frame;
}

void VideoData::updateFrame( cv::Mat frame )
{
    boost::lock_guard<boost::mutex> lock( this->mutex );
    this->frame = frame.clone();
}


cv::Mat VideoData::readMorphology()
{
    boost::lock_guard<boost::mutex> lock( this->mutex );

    if( this->morphology.empty() )
    {
        cv::Size s( 640, 480 );
        cv::Mat black_frame( s, CV_8UC3 );

        return black_frame;
    }

    return this->morphology;
}

void VideoData::updateMorphology( cv::Mat morphology )
{
    boost::lock_guard<boost::mutex> lock( this->mutex );
    this->morphology = morphology.clone();
}