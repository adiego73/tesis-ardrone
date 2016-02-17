#include "core/threads.hpp"
#include "util/keys.hpp"

using namespace tesis;

void gui_thread( boost::shared_ptr<MessageServer> messageServer, boost::shared_ptr<VideoData> videoProxy )
{
    bool quit;
    cv::Mat frame;
    std::string window_name = "Video Camera";

    cv::namedWindow( window_name, cv::WINDOW_AUTOSIZE );

    frame = videoProxy->readFrame();

    cv::imshow( window_name, frame );

    messageServer->announce( "gui/action/takeoff" );
    messageServer->announce( "gui/action/land" );
    messageServer->announce( "gui/action/autocontrol" );
    messageServer->announce( "gui/action/emergency" );
    messageServer->announce( "gui/finish" );
    messageServer->announce( "gui/go_next_destination" );

    while( !quit )
    {
        frame = videoProxy->readFrame();

        cv::imshow( window_name, frame );

        int key_pressed = cv::waitKey( 1 );

        switch( key_pressed )
        {
            case key::SPACE:
            {
                bool takeoff = messageServer->get( "gui/action/takeoff", "false" ).find( "false" ) != std::string::npos;
                std::cout << "takeoff: " << std::noboolalpha << takeoff << std::endl;

                if( takeoff )
                {
                    messageServer->publish( "gui/action/takeoff", "true" );
                    messageServer->publish( "gui/action/land", "false" );
                }
                else
                {
                    messageServer->publish( "gui/action/takeoff", "false" );
                    messageServer->publish( "gui/action/land", "true" );
                }

                break;
            }

            case key::S:
            {
                bool autocontrol = messageServer->get( "gui/action/autocontrol", "false" ).find( "false" ) != std::string::npos;
                std::cout << "autocontrol: " << std::noboolalpha << autocontrol << std::endl;
                messageServer->publish( "gui/action/autocontrol", autocontrol ? "true" : "false" );
                break;
            }

            case key::Q:
            case key::ESC:
            {
                messageServer->publish( "gui/finish", "true" );
                quit = true;
                break;
            }

            case key::N:
            {
                messageServer->publish( "gui/go_next_destination", "true");
                break;
            }

            case key::L:
            {
                messageServer->publish( "gui/action/land", "true" );
                break;
            }

            case key::T:
            {
                std::vector<std::string> topics = messageServer->topics();

                for( std::string topic : topics )
                {
                    std::cout << topic << std::endl;
                }
            }
        }
    }
}
