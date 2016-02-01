#ifndef UTIL_H
#define UTIL_H

#include <math.h>

#include <opencv.hpp>

#include "structures.hpp"

namespace tesis
{

class Util
{
    public:
        static float rad_to_deg( float rad );
        static float deg_to_rad( float deg );
        static Point rotate( Point pt, float deg );
        static float get_angle( Point pt, Point pt2, float deg );
        static float distance( Point pt, Point pt2 );
        static cv::Point to_cv_point( Point pt );
        static Point get_xy_distance( int dist, float deg );
        static Point px_to_mt( const Point pt, const cv::Size img_size );
        static int mt_to_px( float mt, cv::Size img_size );
        static Point mt_to_px( Point pt, cv::Size img_size );

    private:
        Util();
        ~Util();
};

};
#endif // UTIL_H
