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
        static Point rpoint_to_ipoint( Point pt, cv::Size img_size );
        static Point ipoint_to_rpoint( const Point pt, const cv::Size img_size );
        static bool file_exists( std::string path );
        
        static float SPACE_SIZE_X = 239.0f;
        static float SPACE_SIZE_Y = 174.0f;

    private:
        Util();
        ~Util();
};

};
#endif // UTIL_H
