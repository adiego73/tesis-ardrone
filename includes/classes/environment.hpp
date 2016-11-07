#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <boost/thread.hpp>

#include "structures.hpp"
#include "util/util.hpp"
#include "core/data/videodata.hpp"

#include "classes/environment_configuration.hpp"
#include "classes/video.hpp"

namespace tesis
{
typedef struct {

	Point pos;
	int id;
	//float time;
	std::string comment;

} SafeSpot;

class Environment
{
public:
	Point trackRobotPosition();
	bool trackSpots();
	//SafeSpot nextDestination();

	Point getRobotPostionNormalized ( float robot_altitude );
	bool isRobotVisible();
	void updateFrame ( boost::shared_ptr<VideoData> videoProxy );
	void updateMorphology ( boost::shared_ptr<VideoData> videoProxy );

	//SafeSpot getNextDestination();
	SafeSpot* getSpot ( int id );
	EnvironmentConfig getConfiguration();
	float getConfigurationCameraHeight();
	Size getConfigurationSpaceSize();
	Point getRobotPosition();
	std::vector<SafeSpot> getSpots();

	std::string getVideosPath();

	Environment ( std::string config_path );
	~Environment();

private:
	EnvironmentConfig env_config;

	std::vector<SafeSpot> safe_spots;
	std::vector<SafeSpot> unsafe_spots;
	std::vector<Point> robot_positions;

	boost::shared_ptr<Video> video_camera;

	int next_destination;
	float camera_height;
	Point robot_position;
};


};
#endif // ENVIRONMENT_HPP
