#include "core/threads.hpp"
#include "util/keys.hpp"

using namespace tesis;

#ifndef START
# define START   90
#endif

#ifndef AMPLIAR_Y
# define AMPLIAR_Y   710
#endif

#ifndef AMPLIAR_Y2
# define AMPLIAR_Y2  90
#endif

#ifndef AMPLIAR_X
# define AMPLIAR_X   3
#endif

#ifndef CENTRO_Y_PITCH
# define CENTRO_Y_PITCH  (START + AMPLIAR_Y2)
#endif

#ifndef CENTRO_Y_ROLL
# define CENTRO_Y_ROLL   (START + AMPLIAR_Y2 * 3)
#endif

#ifndef CENTRO_Y_YAW
# define CENTRO_Y_YAW    (START + AMPLIAR_Y2 * 5)
#endif


// indice 0: kp
// indice 1: ki
// indice 2: kd
// indice 3: set
// indice 4: actual value
typedef std::vector<std::tuple<float, float, float, float, float>> VectorPIDValues;

void get_graphics_frame( cv::Mat& frame, VectorPIDValues pitch, VectorPIDValues roll, VectorPIDValues yaw, VectorPIDValues altitude );
void update_vector_pid_values( boost::shared_ptr<MessageServer> server, VectorPIDValues& pitch, VectorPIDValues& roll, VectorPIDValues& yaw, VectorPIDValues& altitude );

void gui_thread( boost::shared_ptr<MessageServer> messageServer, boost::shared_ptr<VideoData> videoProxy )
{
    bool quit;
    bool show_graphics = false;
    cv::Mat frame;
    cv::Mat graphics_frame( cv::Size( 640, 630 ), CV_8UC3, cv::Scalar( 255, 255, 255 ) );
    std::string window_name = "Video Camera";
    std::string graphics_window_name = "Grafics";

    VectorPIDValues pitch_values;
    VectorPIDValues roll_values;
    VectorPIDValues yaw_values;
    VectorPIDValues altitude_values;

    cv::namedWindow( window_name, cv::WINDOW_AUTOSIZE );

    cv::namedWindow( graphics_window_name, cv::WINDOW_AUTOSIZE );

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

        if( show_graphics )
        {
            update_vector_pid_values( messageServer, pitch_values, roll_values , yaw_values, altitude_values );
            get_graphics_frame( graphics_frame, pitch_values, roll_values, yaw_values, altitude_values );

            cv::imshow( graphics_window_name, graphics_frame );
        }

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
                messageServer->publish( "gui/go_next_destination", "true" );
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

            case key::D:
            {
                show_graphics = show_graphics ? false : true;

                if( !show_graphics )
                    cv::destroyWindow( graphics_window_name );

                break;
            }
        }
    }

    cv::destroyAllWindows();
}

void get_graphics_frame( cv::Mat& frame, VectorPIDValues pitch, VectorPIDValues roll, VectorPIDValues yaw, VectorPIDValues altitude )
{
    //********************  PARA EL PITCH  ************
    //LINEA CENTRO PITCH
    cv::line( frame, cv::Point( 0, CENTRO_Y_PITCH ), cv::Point( frame.size().width, CENTRO_Y_PITCH ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );
    //LINEA ARRIBA PITCH
    cv::line( frame, cv::Point( 0, CENTRO_Y_PITCH + 1 * AMPLIAR_Y2 ), cv::Point( frame.size().width, CENTRO_Y_PITCH + 1 * AMPLIAR_Y2 ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );
    //LINEA ABAJO PITCH
    cv::line( frame, cv::Point( 0, CENTRO_Y_PITCH - 1 * AMPLIAR_Y2 ), cv::Point( frame.size().width, CENTRO_Y_PITCH - 1 * AMPLIAR_Y2 ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );

    //PUNTOS
    for( int i = 0; i < int( pitch.size() - 1 ); i++ )
    {
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<0>( pitch[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<0>( pitch[i + 1] ) * AMPLIAR_Y ), CV_RGB( 0, 180, 0 ), 1, cv::LINE_8, 0 );
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<1>( pitch[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<1>( pitch[i + 1] ) * AMPLIAR_Y ), CV_RGB( 0, 180, 0 ), 1, cv::LINE_8, 0 );
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<2>( pitch[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<2>( pitch[i + 1] ) * AMPLIAR_Y ), CV_RGB( 0, 0, 200 ), 1, cv::LINE_8, 0 );

        //amarillo
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<3>( pitch[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<3>( pitch[i + 1] ) * AMPLIAR_Y ),  CV_RGB( 200, 200, 0 ), 2, cv::LINE_8, 0 );
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<4>( pitch[i] ) * AMPLIAR_Y / 30 ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<4>( pitch[i + 1] ) * AMPLIAR_Y / 30 ), CV_RGB( 0, 255, 255 ), 2, cv::LINE_8, 0 );
    }

    //********************  PARA EL ROLL  ************
    //LINEA CENTRO ROLL
    cv::line( frame, cv::Point( 0, CENTRO_Y_ROLL ), cv::Point( frame.size().width, CENTRO_Y_ROLL ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );
    //LINEA ARRIBA ROLL
    cv::line( frame, cv::Point( 0, CENTRO_Y_ROLL + 1 * AMPLIAR_Y2 ), cv::Point( frame.size().width, CENTRO_Y_ROLL + 1 * AMPLIAR_Y2 ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );
    //LINEA ABAJO ROLL
    cv::line( frame, cv::Point( 0, CENTRO_Y_ROLL - 1 * AMPLIAR_Y2 ), cv::Point( frame.size().width, CENTRO_Y_ROLL - 1 * AMPLIAR_Y2 ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );

    for( int i = 0; i < int( roll.size() - 1 ); i++ )
    {
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<0>( roll[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<0>( roll[i + 1] ) * AMPLIAR_Y ), CV_RGB( 180, 0, 0 ), 1, cv::LINE_8, 0 );
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<1>( roll[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<1>( roll[i + 1] ) * AMPLIAR_Y ), CV_RGB( 0, 180, 0 ), 1, cv::LINE_8, 0 );
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<2>( roll[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<2>( roll[i + 1] ) * AMPLIAR_Y ), CV_RGB( 0, 0, 200 ), 1, cv::LINE_8, 0 );

        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<3>( roll[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<3>( roll[i + 1] ) * AMPLIAR_Y ), CV_RGB( 200, 200, 0 ), 2, cv::LINE_8, 0 );
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<4>( roll[i] ) * AMPLIAR_Y / 30 ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<4>( roll[i + 1] ) * AMPLIAR_Y / 30 ), CV_RGB( 0, 255, 255 ), 2, cv::LINE_8, 0 );

    }

    //********************  PARA LA ALTITUDE  ************
    //LINEA CENTRO YAW
    cv::line( frame, cv::Point( 0, CENTRO_Y_YAW ), cv::Point( frame.size().width, CENTRO_Y_YAW ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );
    //LINEA ARRIBA YAW
    cv::line( frame, cv::Point( 0, CENTRO_Y_YAW + 1 * AMPLIAR_Y2 ), cv::Point( frame.size().width, CENTRO_Y_YAW + 1 * AMPLIAR_Y2 ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );
    //LINEA ABAJO YAW
    cv::line( frame, cv::Point( 0, CENTRO_Y_YAW - 1 * AMPLIAR_Y2 ), cv::Point( frame.size().width, CENTRO_Y_YAW - 1 * AMPLIAR_Y2 ), CV_RGB( 0, 0, 0 ), 1, cv::LINE_8, 0 );

    for( int i = 0; i < int( altitude.size() - 1 ); i++ )
    {
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_YAW + std::get<0>( altitude[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_YAW + std::get<0>( altitude[i + 1] ) * AMPLIAR_Y ), CV_RGB( 200, 0, 0 ), 1, cv::LINE_8, 0 );
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_YAW + std::get<1>( altitude[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_YAW + std::get<1>( altitude[i + 1] ) * AMPLIAR_Y ), CV_RGB( 0, 180, 0 ), 1, cv::LINE_8, 0 );
        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_YAW + std::get<2>( altitude[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_YAW + std::get<2>( altitude[i + 1] ) * AMPLIAR_Y ), CV_RGB( 0, 0, 200 ), 1, cv::LINE_8, 0 );

        cv::line( frame, cv::Point( i * AMPLIAR_X, CENTRO_Y_YAW + std::get<3>( altitude[i] ) * AMPLIAR_Y ), cv::Point( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_YAW + std::get<3>( altitude[i + 1] ) * AMPLIAR_Y ), CV_RGB( 200, 200, 0 ), 2, cv::LINE_8, 0 );
    }

    cv::putText( frame, "proportional", cv::Point( 10, 10 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB( 255, 0, 0 ) );
    cv::putText( frame, "integrate", cv::Point( 10, 30 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB( 0, 255, 0 ) );
    cv::putText( frame, "derivate", cv::Point( 10, 50 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB( 0, 0, 255 ) );
    cv::putText( frame, "total_pitch", cv::Point( 10, 70 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB( 200, 200, 0 ) );
    cv::putText( frame, "ROLL", cv::Point( 10, CENTRO_Y_ROLL - AMPLIAR_Y2 + 20 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB( 200, 200, 0 ) );
    cv::putText( frame, "YAW", cv::Point( 10, CENTRO_Y_YAW - AMPLIAR_Y2 + 20 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB( 200, 200, 0 ) );

    cv::line( frame, cv::Point( frame.size().width - 100, 55 ), cv::Point( frame.size().width - 10, 55 ), CV_RGB( 0, 0, 0 ), 2, 8, 0 );

    cv::line( frame, cv::Point( frame.size().width - 55, 10 ), cv::Point( frame.size().width - 55, 100 ), CV_RGB( 0, 0, 0 ), 2, 8, 0 );

    cv::circle( frame, cv::Point( frame.size().width - 55 + std::get<4>( roll.back() ) * 5, 55 ), 4, CV_RGB( 255, 0, 255 ), -1, 8, 0 );

    cv::circle( frame, cv::Point( frame.size().width - 55, 55 - std::get<4>( pitch.back() ) * 5 ), 4, CV_RGB( 255, 0, 0 ), -1, 8, 0 );

}


void update_vector_pid_values( boost::shared_ptr<MessageServer> server, VectorPIDValues& pitch, VectorPIDValues& roll, VectorPIDValues& yaw, VectorPIDValues& altitude )
{
    if( pitch.size() >= 10 )
        pitch.erase( pitch.begin() );

    if( roll.size() >= 10 )
        roll.erase( roll.begin() );

    if( yaw.size() >= 10 )
        yaw.erase( yaw.begin() );

    if( altitude.size() >= 10 )
        altitude.erase( altitude.begin() );

    float pKp = std::stof( server->get( "robot/pitch/kp", "0" ) );
    float pKi = std::stof( server->get( "robot/pitch/ki", "0" ) );
    float pKd = std::stof( server->get( "robot/pitch/kd", "0" ) );
    float pSet = std::stof( server->get( "robot/pitch/set", "0" ) );
    float pValue = std::stof( server->get( "robot/pitch/value", "0" ) );

    pitch.push_back( std::make_tuple( pKp, pKi, pKd, pSet, pValue ) );

    float rKp = std::stof( server->get( "robot/roll/kp", "0" ) );
    float rKi = std::stof( server->get( "robot/roll/ki", "0" ) );
    float rKd = std::stof( server->get( "robot/roll/kd", "0" ) );
    float rSet = std::stof( server->get( "robot/roll/set", "0" ) );
    float rValue = std::stof( server->get( "robot/roll/value", "0" ) );

    roll.push_back( std::make_tuple( rKp, rKi, rKp, rSet, rValue ) );

    float yKp = std::stof( server->get( "robot/yaw/kp", "0" ) );
    float yKi = std::stof( server->get( "robot/yaw/ki", "0" ) );
    float yKd = std::stof( server->get( "robot/yaw/kd", "0" ) );
    float ySet = std::stof( server->get( "robot/yaw/set", "0" ) );
    float yValue = std::stof( server->get( "robot/yaw/value", "0" ) );

    yaw.push_back( std::make_tuple( yKp, yKi, yKd, ySet, yValue ) );

    float aKp = std::stof( server->get( "robot/altitude/kp", "0" ) );
    float aKi = std::stof( server->get( "robot/altitude/ki", "0" ) );
    float aKd = std::stof( server->get( "robot/altitude/kd", "0" ) );
    float aSet = std::stof( server->get( "robot/altitude/set", "0" ) );
    float aValue = std::stof( server->get( "robot/altitude/value", "0" ) );

    altitude.push_back( std::make_tuple( aKp, aKi, aKd, aSet, aValue ) );
    
}
