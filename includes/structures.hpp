#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <vector>
#include <iostream>
#include <string>
#include <opencv.hpp>
#include <ardrone.h>

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

typedef struct
{
    float vx;
    float vy;
    float vz;
} Velocity;




};

#endif
