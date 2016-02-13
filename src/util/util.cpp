#include "util/util.hpp"
#include <boost/concept_check.hpp>

using namespace tesis;

float Util::deg_to_rad( float deg )
{
    return deg * M_PI / 180.0;
}

// Conversion from a point in the space to a point into image (from cm to px)
Point Util::rpoint_to_ipoint( Point rpoint, cv::Size img_size )
{
    Point ipoint;
    
    ipoint.x = rpoint.x * img_size.width / Util::SPACE_SIZE_X;
    ipoint.y = rpoint.y * img_size.height / Util::SPACE_SIZE_Y;
    
    return ipoint;
}
// Conversion from a point in the image to a point into the space (from px to cm)
Point Util::ipoint_to_rpoint( Point ipoint, cv::Size img_size )
{
    Point rpoint;
    
    rpoint.x = ipoint.x * Util::SPACE_SIZE_X / img_size.width ;
    rpoint.y = ipoint.y * Util::SPACE_SIZE_Y / img_size.height;
    
    return rpoint;
}

Point Util::rotate( Point point, float deg )
{
    Point rotated;
    float rad = Util::deg_to_rad( deg );

    rotated.x = point.x * std::cos( rad ) - point.y * std::sin( rad );
    rotated.y = point.x * std::sin( rad ) + point.y * std::cos( rad );

    return rotated;
}
bool Util::file_exists( std::string path )
{
    struct stat buffer;
    return ( stat( path.c_str(), &buffer ) == 0 );
}
