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
    
    rpoint.z = ipoint.z;
    rpoint.x = ipoint.x * space_size.width / img_size.width ;
    rpoint.y = ipoint.y * space_size.height / img_size.height;
    
    return rpoint;
}

// Conversion from a point in the image to a point into the space (from px to cm)
Point Util::rpoint_to_ipoint( Point rpoint, cv::Size img_size, Size space_size )
{
    Point ipoint;
    
    ipoint.z = rpoint.z;
    ipoint.x = rpoint.x * img_size.width / space_size.width;
    ipoint.y = rpoint.y * img_size.height / space_size.height;

    return ipoint;
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

float Util::distance( Point opoint, Point dpoint )
{
    Point point;
    point.x = opoint.x - dpoint.x;
    point.y = opoint.y - dpoint.y;
    return std::sqrt( std::abs( point.x * point.x + point.y * point.y ) );
}

// PRIVATE
Util::Util()
{

}

Util::~Util()
{

}

