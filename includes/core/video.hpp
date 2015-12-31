#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "structures.hpp"

namespace tesis {

class Video {
    
public:
    Video();
    ~Video();
    
private:
    cv::Mat frame;
    cv::Size img_size;
    
};



};
#endif // VIDEO_HPP
