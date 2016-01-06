#include "classes/video.hpp"

using namespace tesis;

Video::Video ( int camera ) : cap(camera)
{
    this->cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    this->cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    this->img_size.height = this->cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    this->img_size.width = this->cap.get(CV_CAP_PROP_FRAME_WIDTH);
    this->fps = this->cap.get(CV_CAP_PROP_FPS);
}


Video::~Video()
{
    this->cap.release();
}

cv::Mat Video::getFrame()
{
    cv::Mat frame;
    this->cap.read(frame);
    return frame;
}

cv::Size Video::getFrameSize()
{
    cv::Size s;
    return s;
}


std::vector< std::vector< Point > > Video::transformToBlackAndWhite ( cv::Mat &image, const std::vector< tesis::Color > color, int length )
{
    std::vector<std::vector<Point>> puntos ( length );
    // accept only char type matrices
    assert ( image.depth() != sizeof ( uchar ) );

    switch ( image.channels() )
    {
        case 3:
            cv::MatIterator_<cv::Vec3b> it, end;

            for ( it = image.begin<cv::Vec3b>(), end = image.end<cv::Vec3b>(); it != end; ++it )
            {
                cv::Scalar scalar ( ( *it ) [0], ( *it ) [1], ( *it ) [2] );
                char f = 255;

                for ( int i = 0; i < length; i++ )
                {
            //                     TODO reemplazar esto
            //                     if ( this->isSimilarColorHSV3 ( scalar, color[i] ) )
            //                     {
                    Point p;
                    p.x = it.pos().x;
                    p.y = it.pos().y;

                    puntos[i].push_back ( p );

                    f = i * 100;
            //                     }
                }

                ( *it ) [0] = f;
                ( *it ) [1] = f;
                ( *it ) [2] = f;

            }
        break;

    }

    return puntos;
}

std::vector< Point > Video::transformToBlackAndWhite ( cv::Mat &image, const Color color )
{

}
