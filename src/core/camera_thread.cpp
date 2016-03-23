#include "core/threads.hpp"

using namespace tesis;

void camera_thread( boost::shared_ptr<MessageServer> messageServer, boost::shared_ptr<Environment> env, boost::shared_ptr<VideoData> videoData )
{
    std::cout << "camera thread" << std::endl;

    bool quit = false;
    bool autocontrol = true;
    bool start_count = true;
    auto start_time = std::chrono::high_resolution_clock::now();
    bool autocontrol_go_next_destination = false;

    messageServer->announce( "camera/robot_position/x" );
    messageServer->announce( "camera/robot_position/y" );
    messageServer->announce( "camera/robot_position/z" );
    
    messageServer->announce( "camera/destination/count");
    for(int i = 0; i < env->getConfiguration().safe_spot.size();i++)
    {
      messageServer->announce( "camera/destination/" + std::to_string(i) + "/x" );
      messageServer->announce( "camera/destination/" + std::to_string(i) + "/y" );
      messageServer->announce( "camera/destination/" + std::to_string(i) + "/z" );
      messageServer->announce( "camera/destination/" + std::to_string(i) + "/id" );
      messageServer->announce( "camera/destination/" + std::to_string(i) + "/time" );
      messageServer->announce( "camera/destination/" + std::to_string(i) + "/comment" );
    }
    messageServer->announce( "camera/robot_found" );
    messageServer->announce( "camera/elapsed_time" );

    std::vector<SafeSpot> destinations = env->getDestinations();

    std::string robot_visible;

    long time = 0;
    int trackDestinations = 0;
    auto start = std::chrono::high_resolution_clock::now();

    while( !quit )
    {
        // I think this is the simplest way to pass the frame to the gui thread.
        env->updateFrame( videoData );
	env->updateMorphology( videoData );
	
        if( trackDestinations >= 10 )
        {
            env->trackDestinations();
            destinations = env->getDestinations();
            trackDestinations = 0;
	    
	    for(int i = 0; i < destinations.size();i++)
	    {
	      messageServer->publish( "camera/destination/" + std::to_string(i) + "/x", destinations.at(i).pos.x );
	      messageServer->publish( "camera/destination/" + std::to_string(i) + "/y", destinations.at(i).pos.y );
	      messageServer->publish( "camera/destination/" + std::to_string(i) + "/z", destinations.at(i).pos.z );
	      messageServer->publish( "camera/destination/" + std::to_string(i) + "/id", destinations.at(i).id );
	      messageServer->publish( "camera/destination/" + std::to_string(i) + "/time", destinations.at(i).time );
	      messageServer->publish( "camera/destination/" + std::to_string(i) + "/comment", destinations.at(i).comment );
	    }
	    messageServer->publish( "camera/destination/count", destinations.size());
	}

        trackDestinations++;

        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start );

        start = std::chrono::high_resolution_clock::now();

        // El robot debe mandar su altura cada tanto.
        float robot_altitude = messageServer->getFloat( "robot/altitude", "0" );

        Point robot_position = env->getRobotPostionNormalized( robot_altitude );

        messageServer->publish( "camera/robot_position/x", std::to_string( robot_position.x ) );
        messageServer->publish( "camera/robot_position/y", std::to_string( robot_position.y ) );
        messageServer->publish( "camera/robot_position/z", std::to_string( robot_altitude ) );

        // if the robot was previously found, time should be elapsed_time
        if( messageServer->get( "camera/robot_found" ).find( "true" ) != std::string::npos )
        {
            time = elapsed_time.count();
        }
        else
        {
            // if not, it should be added to elapsed_time
            time += elapsed_time.count();
        }

        messageServer->publish( "camera/elapsed_time", std::to_string( time ) );

        robot_visible = env->isRobotVisible() ? "true" : "false";

        messageServer->publish( "camera/robot_found", robot_visible );
/*
        bool gui_go_next_destination;
        std::string go_next_destination_str = messageServer->get( "gui/go_next_destination", "true" );
        std::istringstream( go_next_destination_str ) >> std::boolalpha >> gui_go_next_destination;

	
        if( autocontrol )
        {
            SafeSpot next_dest = env->getNextDestination();
            float distance = Util::distance( robot_position, next_dest.pos );

            // if distance between robot and destination is <= 50
            if( robot_position.x != -1 && distance <=  30 )//30cm de distancia
            {
                if( start_count )
                {
                    start_time = std::chrono::high_resolution_clock::now();
                    start_count = false;
                }

                auto end_time = std::chrono::high_resolution_clock::now();
                auto elapsed_time_over_spot = std::chrono::duration_cast<std::chrono::seconds>( end_time - start_time );

                if( elapsed_time_over_spot.count() >= 2 )
                {
                    autocontrol_go_next_destination = true;
                }
            }
            else
            {
                start_count = true;
                autocontrol_go_next_destination = false;
            }
        }

        if( gui_go_next_destination || autocontrol_go_next_destination )
        {
            env->nextDestination();
	    // TODO esto es un hack. GUI deberia publicar sus propios mensajes.
            messageServer->publish( "gui/go_next_destination", "false" );
        }
	messageServer->publish( "camera/destination/x", std::to_string( env->getNextDestination().pos.x ) );
	messageServer->publish( "camera/destination/y", std::to_string( env->getNextDestination().pos.y ) );
	messageServer->publish( "camera/destination/z", std::to_string( env->getNextDestination().pos.z ) );
	messageServer->publish( "camera/destination/id", std::to_string( env->getNextDestination().id ) );
*/
        quit = messageServer->getBool( "gui/finish", "false" );

    }

}
