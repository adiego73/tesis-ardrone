#include "classes/environment.hpp"

using namespace tesis;

Environment::Environment( std::string config_path ) : next_destination( 0 )
{
    EnvironmentConfiguration env_configuration( config_path );

    this->env_config = env_configuration.get();
    this->camera_height = this->env_config.camera_height;
    this->video_camera.reset( new Video( this->env_config.camera_number ) );
}

void Environment::trackDestinations()
{
    for( int i = 0; i < this->env_config.safe_spot.size(); i++ )
    {
        Color destination = this->env_config.safe_spot[i];
        Point p = this->video_camera->trackColor( destination );
        

        if( p.x != -1 || p.y !=  -1 )
        {
	    p = Util::ipoint_to_rpoint( p, this->video_camera->getFrameSize(), this->env_config.space );
	    p.z = i;
	    
            int e = 0;

            // find safe spot position
            while( e < this->safe_spots.size() && this->safe_spots[e].z !=  i )
            {
                e++;
            }
            // update safe spot position
            if( e < this->safe_spots.size() )
            {
                this->safe_spots[e] = p;
            }
            else
            {
                this->safe_spots.push_back( p );
            }
        }
    }
}

std::vector< Point > Environment::getDestinations()
{
    return this->safe_spots;
}

Point Environment::trackRobotPosition()
{
    Point position = this->video_camera->trackColor( this->env_config.robot_id );

    this->robot_position = position;

    return position;
}

Point Environment::getRobotPostionNormalized( float robot_altitude )
{
    auto normalize = [this, robot_altitude]( const float frame_size, const float r_pos )
    {
        float value_px = ( frame_size / 2 ) + ( r_pos - ( frame_size / 2 ) ) * ( env_config.camera_height - robot_altitude ) / env_config.camera_height;
        return value_px;
    };

    Point robot_position = this->trackRobotPosition();
    robot_position.x = normalize( this->video_camera->getFrameWidth(), robot_position.x );
    robot_position.y = normalize( this->video_camera->getFrameHeight(), robot_position.y );

    robot_position = Util::ipoint_to_rpoint( robot_position, this->video_camera->getFrameSize(), this->env_config.space );

    if( this->isRobotVisible() )
    {
        //  it keeps the last 10 positions of the robot;
        if( this->robot_positions.size() >= 10 )
        {
            //  removing first element
            this->robot_positions.erase( this->robot_positions.begin() );
        }

        // add element to the end
        this->robot_positions.push_back( robot_position );
    }
    else
    {
        robot_position.x = -1;
        robot_position.y = -1;
    }

    return robot_position;
}


bool Environment::isRobotVisible()
{
    return ( this->robot_position.x != -1 );
}

Point Environment::nextDestination()
{
    if( this->safe_spots.empty() )
    {
        Point ret;
        ret.x = -1;
        ret.y = -1;
        return ret;
    }
    this->next_destination++;
    
    if( this->next_destination >= this->safe_spots.size() )
    {
        this->next_destination = 0;
    }

    Point next_destination = this->safe_spots[this->next_destination];

    

    return next_destination;
}

void Environment::updateFrame( boost::shared_ptr< VideoData > videoProxy )
{
    this->video_camera->capture();

    videoProxy->updateFrame( this->video_camera->getFrame() );
}

float Environment::getConfigurationCameraHeight()
{
    return this->camera_height;
}

Size Environment::getConfigurationSpaceSize()
{
    return this->env_config.space;
}

Point Environment::getNextDestination()
{
  if (this->safe_spots.size() > this->next_destination)
      return this->safe_spots[this->next_destination];
  else  {
        Point ret;
        ret.x = -1;
        ret.y = -1;
        return ret;
   }
}

Point Environment::getRobotPosition()
{
    return this->robot_position;
}

Environment::~Environment()
{
}





