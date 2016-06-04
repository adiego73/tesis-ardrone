#include "classes/environment.hpp"

using namespace tesis;

Environment::Environment ( std::string config_path ) : next_destination ( 0 )
{
	EnvironmentConfiguration env_configuration ( config_path );

	this->env_config = env_configuration.get();
	this->camera_height = this->env_config.camera_height;
	this->video_camera.reset ( new Video ( this->env_config.camera_number ) );
}

void Environment::trackSpots()
{
	for ( int i = 0; i < this->env_config.safe_spot.size(); i++ ) {
		SafeSpot sp;

		Spot destination = this->env_config.safe_spot[i];
		sp.pos = this->video_camera->trackColor ( destination.color );
		sp.id = destination.id;
		sp.comment = destination.comment;
		sp.time = destination.time;

		//color hardcodeado
		/*if(i == 0)
		{
		  sp.pos.x = 220;
		  sp.pos.y = 140;
		}
		else if(i == 1)
		{
		  sp.pos.x = 420;
		  sp.pos.y = 340;
		}*/
		if ( sp.pos.x != -1 || sp.pos.y !=  -1 ) {
			sp.pos = Util::ipoint_to_rpoint ( sp.pos, this->video_camera->getFrameSize(), this->env_config.space );

			int e = 0;

			// find safe spot position
			while ( e < this->safe_spots.size() && this->safe_spots[e].id !=  sp.id ) {
				e++;
			}

			// update safe spot position
			if ( e < this->safe_spots.size() ) {
				if ( Util::distance ( this->safe_spots[e].pos, sp.pos ) < 30 )
					this->safe_spots[e] = sp;
			} else {
				this->safe_spots.push_back ( sp );
			}
		}
	}
}

std::vector< SafeSpot > Environment::getSpots()
{
	return this->safe_spots;
}

Point Environment::trackRobotPosition()
{
	Point position;

	if ( this->env_config.robot_id.size() == 2 )
		position = this->video_camera->trackColor ( this->env_config.robot_id[0] );
	else if ( this->env_config.robot_id.size() == 2 )
		position = this->video_camera->trackColor ( this->env_config.robot_id[0], this->env_config.robot_id[1] );

	this->robot_position = position;

	return position;
}

Point Environment::getRobotPostionNormalized ( float robot_altitude )
{
	auto normalize = [this, robot_altitude] ( const float frame_size, const float r_pos ) {
		float value_px = ( frame_size / 2 ) + ( r_pos - ( frame_size / 2 ) ) * ( env_config.camera_height - robot_altitude ) / env_config.camera_height;
		return value_px;
	};

	Point robot_position = this->trackRobotPosition();
	robot_position.x = normalize ( this->video_camera->getFrameWidth(), robot_position.x );
	robot_position.y = normalize ( this->video_camera->getFrameHeight(), robot_position.y );

	robot_position = Util::ipoint_to_rpoint ( robot_position, this->video_camera->getFrameSize(), this->env_config.space );

	if ( this->isRobotVisible() ) {
		//  it keeps the last 10 positions of the robot;
		if ( this->robot_positions.size() >= 10 ) {
			//  removing first element
			this->robot_positions.erase ( this->robot_positions.begin() );
		}

		// add element to the end
		this->robot_positions.push_back ( robot_position );
	} else {
		robot_position.x = -1;
		robot_position.y = -1;
	}

	return robot_position;
}


bool Environment::isRobotVisible()
{
	return ( this->robot_position.x != -1 );
}
/*
SafeSpot Environment::nextDestination()
{
	if ( this->safe_spots.empty() ) {
		SafeSpot ret;
		ret.pos.x = -1;
		ret.pos.y = -1;
		ret.pos.z = -1;
		ret.id = 0;
		ret.comment = "";
		ret.time = 0;
		return ret;
	}

	this->next_destination++;

	if ( this->next_destination >= this->safe_spots.size() ) {
		this->next_destination = 0;
	}

	SafeSpot next_destination = this->safe_spots[this->next_destination];



	return next_destination;
}*/

void Environment::updateFrame ( boost::shared_ptr< VideoData > videoProxy )
{
	this->video_camera->capture();

	videoProxy->updateFrame ( this->video_camera->getFrame() );
}

void Environment::updateMorphology ( boost::shared_ptr< VideoData > videoProxy )
{
	videoProxy->updateMorphology ( this->video_camera->getMorphology() );
}

float Environment::getConfigurationCameraHeight()
{
	return this->camera_height;
}

Size Environment::getConfigurationSpaceSize()
{
	return this->env_config.space;
}

std::string Environment::getVideosPath()
{
	return this->env_config.path_videos;
}

SafeSpot* Environment::getSpot ( int id )
{
	// find safe spot position
	int e = 0;
	while ( e < this->safe_spots.size() && this->safe_spots[e].id !=  id ) {
		e++;
	}

	// update safe spot position
	if ( e < this->safe_spots.size() )
		return &this->safe_spots[e];
	else
		return NULL;

}
/*
SafeSpot Environment::getNextDestination()
{
	if ( this->safe_spots.size() > this->next_destination )
		return this->safe_spots[this->next_destination];
	else {
		SafeSpot ret;
		ret.pos.x = -1;
		ret.pos.y = -1;
		ret.pos.z = -1;
		ret.id = 0;
		ret.comment = "";
		ret.time = 0;
		return ret;
	}
}
*/
Point Environment::getRobotPosition()
{
	return this->robot_position;
}

EnvironmentConfig Environment::getConfiguration()
{
	return this->env_config;
}

Environment::~Environment()
{
}





