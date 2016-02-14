#include "core/threads.hpp"

using namespace tesis;

void camera_thread( std::string env_config_path, boost::shared_ptr<MessageServer> messageServer, boost::shared_ptr<VideoData> videoData )
{
    Environment env( env_config_path );

    bool quit = false;

    messageServer->announce( "camera/robot_position/x" );
    messageServer->announce( "camera/robot_position/y" );
    messageServer->announce( "camera/robot_position/z" );
    messageServer->announce( "camera/destination/x" );
    messageServer->announce( "camera/destination/y" );
    messageServer->announce( "camera/robot_found" );
    messageServer->announce( "camera/elapsed_time" );

    std::vector<Point> destinations = env.getDestinations();
    int destination_index = 0;

    std::string robot_visible;

    long time = 0;
    auto start = std::chrono::high_resolution_clock::now();

    while( !quit )
    {
        // I think this is the simplest way to pass the frame to the gui thread.
        env.updateFrame(videoData);
        
        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>( end - start );

        start = std::chrono::high_resolution_clock::now();

        // El robot debe mandar su altura cada tanto.
        std::string altitude_msg = messageServer->get( "robot/altitude", "0" );
        float robot_altitude = std::stof( altitude_msg );

        Point robot_position = env.getRobotPostionNormalized( robot_altitude );

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

        robot_visible = env.isRobotVisible() ? "true" : "false";

        messageServer->publish( "camera/robot_found", robot_visible );

        
        // TODO ver como manejo esto porque no estoy seguro.
        bool go_next_destination;
        std::string go_next_destination_str = messageServer->get( "gui/go_next_destination", "false" );
        std::istringstream( go_next_destination_str ) >> std::boolalpha >> go_next_destination;

        if( go_next_destination )
        {
            if( destination_index >= destinations.size() )
            {
                destination_index = 0;
            }

            Point next_destination = destinations[destination_index];

            destination_index++;

            messageServer->publish( "camera/destination/x", std::to_string( next_destination.x ) );
            messageServer->publish( "camera/destination/y", std::to_string( next_destination.y ) );
        }

        std::string finish_msg = messageServer->get( "gui/finish", "false" );
        std::istringstream( finish_msg ) >> std::boolalpha >> quit;

    }

}
