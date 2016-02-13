#include "classes/video.hpp"

using namespace tesis;

Video::Video( int camera ) : cap( camera )
{
    if( !this->cap.isOpened() )
    {
        std::ostringstream message;
        message << "The camera " << camera << " couldn't be opened";
        throw std::invalid_argument( message.str() );
    }

    this->cap.set( CV_CAP_PROP_FRAME_WIDTH, 640 );
    this->cap.set( CV_CAP_PROP_FRAME_HEIGHT, 480 );
    this->img_size.height = this->cap.get( CV_CAP_PROP_FRAME_HEIGHT );
    this->img_size.width = this->cap.get( CV_CAP_PROP_FRAME_WIDTH );
    this->fps = this->cap.get( CV_CAP_PROP_FPS );
}


Video::~Video()
{
    this->cap.release();
}

cv::Mat Video::getFrame()
{
    cv::Mat frame;
    this->cap.read( frame );
    return frame;
}

cv::Size Video::getFrameSize()
{
    return this->img_size;
}

float Video::getFrameHeight()
{
    return this->getFrameSize().height;
}

float Video::getFrameWidth()
{
    return this->getFrameSize().width;
}

Point Video::trackColor( Color color )
{
    Point pos;
    cv::Mat frame = this->getFrame();
    cv::Size imageSize = this->getFrameSize();
    
    cv::Mat frame_th = cv::Mat( imageSize, CV_8UC1 );
    cv::Mat frame_th2 = cv::Mat( imageSize, CV_8UC1 );
    cv::Mat img_2 = cv::Mat( imageSize, CV_8UC3 );

    cv::Mat morphKernel = cv::getStructuringElement( cv::MORPH_RECT, cv::Size( 5, 5 ), cv::Point( 1, 1 ) );

    cv::Scalar thresholdMin;
    cv::Scalar thresholdMax;
    
    thresholdMin.val[0] = color.Hue.min;
    thresholdMin.val[1] = color.Saturation.min;
    thresholdMin.val[2] = color.Value.min;
    
    thresholdMax.val[0] = color.Hue.max;
    thresholdMax.val[1] = color.Saturation.max;
    thresholdMax.val[2] = color.Value.max;
    
    cv::cvtColor( frame, img_2, CV_BGR2HSV );
    cv::inRange( img_2,thresholdMin, thresholdMax , frame_th );

    cv::morphologyEx( frame_th, frame_th2, CV_MOP_OPEN, morphKernel );

    cv::Moments moments_1;
    cv::Moments moments_2;

    double moment10;
    double moment01;
    double area, r;

    moments_1 = cv::moments( frame_th2, 1 );

    moment10 = moments_1.m10;
    moment01 = moments_1.m01;
    area = moments_1.m00;

    if( area > 3 )
    {
        pos.x = moment10 / area;
        pos.y = moment01 / area;
    }
    else
    {
        pos.x = -1;
        pos.y = -1;
    }

    return pos;
}




