#include "util/util.hpp"

using namespace tesis;

float Util::rad_to_deg( float rad )
{
    return rad * 180.0 / M_PI;
}

float Util::deg_to_rad( float deg )
{
    return deg * M_PI / 180.0;
}

float Util::distance( Point pt, Point pt2 )
{
    pt.x -= pt2.x;
    pt.y -= pt2.y;
    return sqrt( abs( pt.x * pt.x + pt.y * pt.y ) );
}

float Util::get_angle( Point pt, Point pt2, float deg )
{
    float angle = 0;

    pt.x -= pt2.x;
    pt.y -= pt2.y;

    if( pt.x != 0 )
    {
        angle = atan( abs( ( float )pt.y / ( float )pt.x ) );
        angle = rad_to_deg( angle );

        if( pt.x > 0 )
        {
            if( pt.y < 0 )
                angle = -( angle + 90 );
            else
                angle = -( 90 - angle );
        }
        else if( pt.y < 0 )
            angle += 90;
        else
            angle = 90 - angle;
    }

    return angle;
}

Point Util::get_xy_distance( int dist, float deg )
{
    Point pt;
    float rad = deg_to_rad( deg );

    pt.y = dist * sin( rad );
    pt.x = dist * cos( rad );
    return pt;
}

Point Util::mt_to_px( Point pt, cv::Size img_size )
{
    return Point();
}

int Util::mt_to_px( float mt, cv::Size img_size )
{
    return -1;
}

// TODO terminar esto.
Point Util::px_to_mt( Point pt, cv::Size img_size )
{
    return Point();
}

Point Util::rotate( Point pt, float deg )
{
    Point pt2;
    float rad = deg_to_rad( deg );

    pt2.x = pt.x * cos( rad ) - pt.y * sin( rad );
    pt2.y = pt.x * sin( rad ) + pt.y * cos( rad );

    return pt2;
}

cv::Point Util::to_cv_point( Point pt )
{
    return cv::Point(pt.x, pt.y);
}

bool Util::file_exists (std::string path) {
    struct stat buffer;   
    return (stat (path.c_str(), &buffer) == 0); 
  }
