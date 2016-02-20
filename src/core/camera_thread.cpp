#include "core/threads.hpp"


using namespace tesis;

void camera_thread( std::string env_config_path, boost::shared_ptr<MessageServer> messageServer, boost::shared_ptr<VideoData> videoData )
{
    std::cout << "camera thread" << std::endl;
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
//     int a = 1;
//     for (Point dest : destinations)
//     {
//         std::cout << a << ") x: " << dest.x << " y: " << dest.y << std::endl;
//         a++;
//     }
    
    int destination_index = 0;

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
        float robot_altitude = messageServer->get( "robot/altitude", 0.0f );

        Point robot_position = env.getRobotPostionNormalized( robot_altitude );

        messageServer->publish( "camera/robot_position/x", robot_position.x );
        messageServer->publish( "camera/robot_position/y", robot_position.y );
        messageServer->publish( "camera/robot_position/z", robot_altitude );

        // if the robot was previously found, time should be elapsed_time
        if( messageServer->get( "camera/robot_found", false) == true)
        {
            time = elapsed_time.count();
        }
        else
        {
            // if not, it should be added to elapsed_time
            time += elapsed_time.count();
        }

        messageServer->publish( "camera/elapsed_time", time );

        messageServer->publish( "camera/robot_found", env.isRobotVisible() );
        
        bool go_next_destination = messageServer->get( "gui/go_next_destination", true);

        if( go_next_destination )
        {
            if( destination_index >= destinations.size() )
            {
                destination_index = 0;
            }

            Point next_destination = destinations[destination_index];

            destination_index++;

            messageServer->publish( "camera/destination/x", next_destination.x );
            messageServer->publish( "camera/destination/y", next_destination.y );
            
            // TODO esto es un hack. GUI deberia publicar sus propios mensajes.
            messageServer->publish( "gui/go_next_destination", false );
        }

        quit = messageServer->get( "gui/finish", false );
    }

}
