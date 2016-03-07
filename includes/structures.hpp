#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <vector>
#include <iostream>
#include <string>

#include <boost/smart_ptr.hpp>

#include <opencv.hpp>

#include "config/configutarion.hpp"

namespace tesis
{

#ifndef MAX_ANGLE
#define MAX_ANGLE 180
#endif

typedef struct
{
  
    float x;
    float y;
    float z;
} Point;


};

#endif
