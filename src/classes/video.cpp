#include "classes/video.hpp"

using namespace tesis;

Video::Video( int camera ) : cap( camera )
{
    if( !this->cap.isOpened() )
    {
        std::ostringstream message;
        message << "The camera " << camera << " couldn't be opened";
        throw std::invalid_argument(message.str());
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
    cv::Mat frame = this->getFrame();
    cv::Size size;
    size.height = frame.rows;
    size.width = frame.cols;
    
    return size;
}


std::vector< std::vector< Point > > Video::transformToBlackAndWhite( cv::Mat &image, const std::vector< tesis::Color > color, int length )
{
    std::vector<std::vector<Point>> puntos( length );
    // accept only char type matrices
    CV_Assert( image.depth() != sizeof( uchar ) );

    switch( image.channels() )
    {
        case 3:
            cv::MatIterator_<cv::Vec3b> it, end;

            for( it = image.begin<cv::Vec3b>(), end = image.end<cv::Vec3b>(); it != end; ++it )
            {
                cv::Scalar scalar( ( *it ) [0], ( *it ) [1], ( *it ) [2] );
                char f = 255;

                for( int i = 0; i < length; i++ )
                {
                    // TODO reemplazar esto
                    if( this->isSimilarColor( scalar, color[i] ) )
                    {
                        Point p;
                        p.x = it.pos().x;
                        p.y = it.pos().y;

                        puntos[i].push_back( p );

                        f = i * 100;
                    }
                }

                ( *it ) [0] = f;
                ( *it ) [1] = f;
                ( *it ) [2] = f;

            }

            break;

    }

    return puntos;
}

std::vector< Point > Video::transformToBlackAndWhite( cv::Mat &image, const Color color )
{
    std::vector<Point> puntos;
    // accept only char type matrices
    CV_Assert( image.depth() != sizeof( uchar ) );

    switch( image.channels() )
    {
        case 3:
            cv::MatIterator_<cv::Vec3b> it, end;

            for( it = image.begin<cv::Vec3b>(), end = image.end<cv::Vec3b>(); it != end; ++it )
            {
                cv::Scalar scalar( ( *it ) [0], ( *it ) [1], ( *it ) [2] );
                // va a ser 255 * 1 ó 255 * 0.
                uchar f = 255 * ! this->isSimilarColor( scalar, color );

                ( *it ) [0] = f;
                ( *it ) [1] = f;
                ( *it ) [2] = f;

                if( f == 0 )
                {
                    Point p;
                    p.x = it.pos().x;
                    p.y = it.pos().y;
                    puntos.push_back( p );
                }
            }

            break;
    }

    return puntos;
}

bool Video::isSimilarColor( cv::Scalar scalar, Color color )
{
    return ( scalar.val[0] >= color.Hue.min && scalar.val[0] <= color.Hue.max
             && scalar.val[1] >= color.Saturation.min && scalar.val[1] <= color.Saturation.max
             && scalar.val[2] >= color.Value.min && scalar.val[2] <= color.Value.max );
}

bool Video::_isSimilarColor( cv::Scalar scalar, Color color )
{
    return (
               ( ( scalar.val[0] < 9 || ( scalar.val[0] > color.Hue.min && scalar.val[0] < color.Hue.max ) )
                 && scalar.val[1] > color.Saturation.min && scalar.val[1] < color.Saturation.max
                 && scalar.val[2] > color.Value.min && scalar.val[2] < color.Value.max ) ||
               ( ( scalar.val[0] < 9 || ( scalar.val[0] > color.Hue.min && scalar.val[0] < color.Hue.max ) )
                 && scalar.val[1] > color.Saturation.min + 100
                 && scalar.val[1] < color.Saturation.max
                 && scalar.val[2] > color.Value.min - 40
                 && scalar.val[2] < color.Value.max )
           );
}



