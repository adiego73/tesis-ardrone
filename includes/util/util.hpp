#ifndef UTIL_H
#define UTIL_H

#include <sys/stat.h>
#include <cmath>
#include <string>

#include <opencv.hpp>

#include "structures.hpp"

namespace tesis
{

class Util
{
    public:
        static float deg_to_rad( float deg );
        static Point rotate( Point pt, float deg );
        static Point ipoint_to_rpoint( const Point pt, const cv::Size img_size, Size space_size );
        static bool file_exists( std::string path );

    private:
        Util();
        ~Util();
};

};
#endif // UTIL_H

