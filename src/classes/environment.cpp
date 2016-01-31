#include "classes/environment.hpp"

using namespace tesis;

Environment::Environment( std::string config_path )
{
    EnvironmentConfiguration env_configuration( config_path );

    this->env_config = env_configuration.get();
    this->camera_height = this->env_config.camera_height;
    this->video_camera.reset( new Video( this->env_config.camera_number ) );
}

std::vector< Point > Environment::getDestinations()
{
    // Esto podria hacerse por threads.
    // y que vaya cargando un array.
    // El array se modifica en medio de un mutex.
    std::vector<Point> destinations;

    for( int i = 0; i < this->env_config.safe_spot.size(); i++ )
    {
        Color destination = this->env_config.safe_spot[i];
        Point p = this->video_camera->trackColor( destination );

        destinations.push_back( p );
    }

    return destinations;
}

Point Environment::getRobotPosition()
{
    Point position = this->video_camera->trackColor( this->env_config.robot_id );

    this->robot_position = position;

    if( this->isRobotVisible() )
    {
        //  it keeps the last 10 positions of the robot;
        if( this->robot_positions.size() >= 10 )
        {
            //  removing first element
            this->robot_positions.erase( this->robot_positions.begin() );
        }
        // add element to the end
        this->robot_positions.push_back( position );
    }

    return position;
}

bool Environment::isRobotVisible()
{
    return ( this->robot_position.x != -1 );
}


Environment::~Environment()
{
}


