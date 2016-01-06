#include "classes/environment.hpp"

using namespace tesis;

Environment::Environment()
{

}

Environment::~Environment()
{
    delete this->camera_video;
}


