#include "util/util.hpp"
#include <boost/concept_check.hpp>

using namespace tesis;

float Util::deg_to_rad( float deg )
{
    return deg * M_PI / 180.0;
}

// Conversion from a point in the image to a point into the space (from px to cm)
Point Util::ipoint_to_rpoint( Point ipoint, cv::Size img_size, Size space_size )
{
    Point rpoint;
    
    rpoint.x = ipoint.x * space_size.width / img_size.width ;
    rpoint.y = ipoint.y * space_size.height / img_size.height;
    
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

// PRIVATE
Util::Util()
{

}

Util::~Util()
{

}
