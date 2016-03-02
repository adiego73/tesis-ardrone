#include "core/threads.hpp"

using namespace tesis;

void camera_thread( boost::shared_ptr<MessageServer> messageServer, boost::shared_ptr<Environment> env, boost::shared_ptr<VideoData> videoData )
{
    std::cout << "camera thread" << std::endl;
    
    bool quit = false;
    bool autocontrol = false; 

    messageServer->announce( "camera/robot_position/x" );
    messageServer->announce( "camera/robot_position/y" );
    messageServer->announce( "camera/robot_position/z" );
    messageServer->announce( "camera/destination/x" );
    messageServer->announce( "camera/destination/y" );
    messageServer->announce( "camera/destination/z" );
    messageServer->announce( "camera/destination/id" );
    messageServer->announce( "camera/robot_found" );
    messageServer->announce( "camera/elapsed_time" );

    std::vector<Point> destinations = env->getDestinations();

    std::string robot_visible;

    long time = 0;
    int trackDestinations = 10;
    auto start = std::chrono::high_resolution_clock::now();

    while( !quit )
    {
        // I think this is the simplest way to pass the frame to the gui thread.
        env->updateFrame(videoData);
        
        if (trackDestinations >= 10)
        {
            env->trackDestinations();
            destinations = env->getDestinations();
            trackDestinations = 0;
        }
        trackDestinations++;
        
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start );

        start = std::chrono::high_resolution_clock::now();

        // El robot debe mandar su altura cada tanto.
        std::string altitude_msg = messageServer->get( "robot/altitude", "0" );
        float robot_altitude = std::stof( altitude_msg );

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
        
        bool go_next_destination;
        std::string go_next_destination_str = messageServer->get( "gui/go_next_destination", "true");
        std::istringstream( go_next_destination_str ) >> std::boolalpha >> go_next_destination;

        if (autocontrol)
        {
            // Calcular distancia y tomar el tiempo siempre que la distancia sea menor que X 
            // robot_position y env.getNextDestination()
            
        }
        
        if( go_next_destination )
        {
            Point next_destination = env->nextDestination();

            messageServer->publish( "camera/destination/x", std::to_string( next_destination.x ) );
            messageServer->publish( "camera/destination/y", std::to_string( next_destination.y ) );
            messageServer->publish( "camera/destination/z", std::to_string( next_destination.z ) );
            messageServer->publish( "camera/destination/id", std::to_string( next_destination.z ) );
	    // TODO esto es un hack. GUI deberia publicar sus propios mensajes.
            messageServer->publish( "gui/go_next_destination", "false" );
        }

        std::string finish_msg = messageServer->get( "gui/finish", "false" );
        std::istringstream( finish_msg ) >> std::boolalpha >> quit;

    }

}
