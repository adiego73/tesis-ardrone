#include "core/threads.hpp"
#include "util/keys.hpp"
#include <string>
#include <sys/time.h>

using namespace tesis;
using namespace std;
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

#ifndef CF_WHITE
#define CF_WHITE CV_RGB(255,255,255)
#endif
#ifndef CF_BLACK
#define CF_BLACK CV_RGB(0,0,0)
#endif
#ifndef CF_RED
#define CF_RED CV_RGB(255,0,0)
#endif
#ifndef CF_BLUE
#define CF_BLUE CV_RGB(0, 0, 255)
#endif
#ifndef CF_GREEN
#define CF_GREEN CV_RGB(0, 255, 0)
#endif
#ifndef CF_YELLOW
#define CF_YELLOW CV_RGB(255, 255, 0)
#endif
#ifndef CF_PURPLE
#define CF_PURPLE CV_RGB(255, 0, 255)
#endif

// indice 0: kp,
// indice 1: ki
// indice 2: kd
// indice 3: set
// indice 4: actual value
typedef std::vector<std::tuple<float, float, float, float, float>> VectorPIDValues;
typedef Point Velocity;

void update_graphics_frame ( cv::Mat& frame, VectorPIDValues pitch, VectorPIDValues roll, VectorPIDValues yaw, VectorPIDValues altitude );
void update_robot_debug_frame ( cv::Mat& frame, VectorPIDValues roll, VectorPIDValues pitch, VectorPIDValues yaw, VectorPIDValues altitude, boost::shared_ptr<Environment> env, Point position, Velocity velocity, boost::shared_ptr<MessageServer> messageServer );
void draw_robot ( cv::Mat& frame, boost::shared_ptr<Environment> env, VectorPIDValues roll, VectorPIDValues pitch, VectorPIDValues yaw, Point position, Velocity velocity );
void update_vector_pid_values ( boost::shared_ptr<MessageServer> server, VectorPIDValues& pitch, VectorPIDValues& roll, VectorPIDValues& yaw, VectorPIDValues& altitude, Point& position, Velocity& velocity );
void write_robot_info ( cv::Mat& frame, VectorPIDValues roll, VectorPIDValues pitch, VectorPIDValues yaw, VectorPIDValues altitude, boost::shared_ptr<Environment> env, Point position, Velocity velocity, boost::shared_ptr<MessageServer> messageServer );
void grabar ( cv::Mat&  robot, cv::Mat& graphics, cv::Mat& morphology, cv::VideoWriter& vw1, cv::VideoWriter& vw2, std::string path, std::string file_name );

void gui_thread ( boost::shared_ptr<MessageServer> messageServer, boost::shared_ptr<Environment> env, boost::shared_ptr<VideoData> videoProxy )
{
	bool quit;
	bool show_graphics = false;
	bool bGrabar = false;
	cv::Mat frame;
	cv::Mat morphology;
	std::string morphology_name = "Morphology";
	std::string window_name = "Video Camera";
	std::string graphics_window_name = "PID Graphics - Debug";
	std::string robot_debug_window_name = "Robot and Safe spots - Debug";

	VectorPIDValues pitch_values;
	VectorPIDValues roll_values;
	VectorPIDValues yaw_values;
	VectorPIDValues altitude_values;
	Point robot_position;
	Velocity robot_velocity;

	//	Writers para guardar videos
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	strftime ( buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo );
	string file_name ( buffer );

	cv::VideoWriter outputVideo;
	cv::VideoWriter outputVideo2;
	cv::VideoWriter outputVideo3;
	/*******************/

	cv::namedWindow ( window_name, cv::WINDOW_AUTOSIZE );
	//cv::namedWindow ( morphology_name, cv::WINDOW_AUTOSIZE );
	frame = videoProxy->readFrame();
	morphology = videoProxy->readMorphology();


	messageServer->announce ( "gui/action/takeoff" );
	messageServer->announce ( "gui/action/land" );
	messageServer->announce ( "gui/action/autocontrol" );
	messageServer->announce ( "gui/action/emergency" );
	messageServer->announce ( "gui/finish" );
	messageServer->announce ( "gui/go_next_destination" );
	messageServer->announce ( "gui/grabar" );
	messageServer->announce ( "robot/onground" );

	while ( !quit ) {
		frame = videoProxy->readFrame();
		//morphology = videoProxy->readMorphology();

		cv::imshow ( window_name, frame );
		//cv::imshow ( morphology_name, morphology );

		if ( show_graphics && !frame.empty() ) {

			update_vector_pid_values ( messageServer, pitch_values, roll_values , yaw_values, altitude_values, robot_position, robot_velocity );

			cv::Mat graphics_frame ( cv::Size ( 640, 630 ), CV_8UC3, cv::Scalar ( 255, 255, 255 ) );
			cv::Mat robot_debug_frame ( cv::Size ( 640, 480 ), CV_8UC3, cv::Scalar ( 230, 230, 230 ) );

			update_graphics_frame ( graphics_frame, pitch_values, roll_values, yaw_values, altitude_values );
			update_robot_debug_frame ( robot_debug_frame, roll_values, pitch_values, yaw_values, altitude_values, env, robot_position, robot_velocity, messageServer );

			cv::imshow ( robot_debug_window_name, robot_debug_frame );
			cv::imshow ( graphics_window_name, graphics_frame );

			if ( bGrabar )
				grabar ( robot_debug_frame, graphics_frame, morphology, outputVideo, outputVideo2, env->getVideosPath(), file_name );
		}

		int key_pressed = cv::waitKey ( 1 );

		switch ( key_pressed ) {
		case key::SPACE: {
			bool takeoff = messageServer->getBool ( "gui/action/takeoff", false );
			
			std::cout << "takeoff: " << std::noboolalpha << takeoff << std::endl;

			if ( takeoff ) {
				messageServer->publish ( "gui/action/takeoff", "false" );
				messageServer->publish ( "gui/action/land", "true" );
			} else {
				messageServer->publish ( "gui/action/takeoff", "true" );
				messageServer->publish ( "gui/action/land", "false" );
			}

			break;
		}

		case key::S: {
			bool autocontrol = messageServer->get ( "gui/action/autocontrol", "false" ).find ( "false" ) != std::string::npos;
			std::cout << "autocontrol: " << std::noboolalpha << autocontrol << std::endl;
			messageServer->publish ( "gui/action/autocontrol", autocontrol ? "true" : "false" );
			break;
		}

		case key::Q:
		case key::ESC: {
			messageServer->publish ( "gui/finish", "true" );
			quit = true;
			break;
		}

		case key::N: {
			messageServer->publish ( "gui/go_next_destination", "true" );
			break;
		}

		case key::L: {
			messageServer->publish ( "gui/action/land", "true" );
			break;
		}


		case key::T: {
			std::vector<std::string> topics = messageServer->topics();

			for ( std::string topic : topics ) {
				std::cout << topic << std::endl;
			}

			break;
		}

		case key::D: {
			show_graphics = show_graphics ? false : true;

			if ( !show_graphics ) {
				cv::destroyWindow ( robot_debug_window_name );
				cv::destroyWindow ( graphics_window_name );
			}
			break;
		}

		case key::R: {
			bGrabar = bGrabar ? false : true;
			messageServer->publish ( "gui/grabar", bGrabar ? "true" : "false" );
			break;
		}
		}
		quit = messageServer->getBool( "gui/finish", false);
	}

	cv::destroyAllWindows();
}

void update_graphics_frame ( cv::Mat& frame, VectorPIDValues pitch, VectorPIDValues roll, VectorPIDValues yaw, VectorPIDValues altitude )
{
	//********************  PARA EL PITCH  ************
	//LINEA CENTRO PITCH
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_PITCH ), cv::Point ( frame.size().width, CENTRO_Y_PITCH ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );
	//LINEA ARRIBA PITCH
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_PITCH + 1 * AMPLIAR_Y2 ), cv::Point ( frame.size().width, CENTRO_Y_PITCH + 1 * AMPLIAR_Y2 ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );
	//LINEA ABAJO PITCH
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_PITCH - 1 * AMPLIAR_Y2 ), cv::Point ( frame.size().width, CENTRO_Y_PITCH - 1 * AMPLIAR_Y2 ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );

	//PUNTOS
	for ( int i = 0; i < int ( pitch.size() - 1 ); i++ ) {
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<0> ( pitch[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<0> ( pitch[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 200, 0, 0 ), 1, 8, 0 );
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<1> ( pitch[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<1> ( pitch[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 0, 180, 0 ), 1, 8, 0 );
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<2> ( pitch[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<2> ( pitch[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 0, 0, 200 ), 1, 8, 0 );

		//amarillo
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<3> ( pitch[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<3> ( pitch[i + 1] ) * AMPLIAR_Y ),  CV_RGB ( 200, 200, 0 ), 2, 8, 0 );
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_PITCH + std::get<4> ( pitch[i] ) * AMPLIAR_Y / 30 ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_PITCH + std::get<4> ( pitch[i + 1] ) * AMPLIAR_Y / 30 ), CV_RGB ( 0, 255, 255 ), 2, 8, 0 );
	}

	//********************  PARA EL ROLL  ************
	//LINEA CENTRO ROLL
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_ROLL ), cv::Point ( frame.size().width, CENTRO_Y_ROLL ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );
	//LINEA ARRIBA ROLL
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_ROLL + 1 * AMPLIAR_Y2 ), cv::Point ( frame.size().width, CENTRO_Y_ROLL + 1 * AMPLIAR_Y2 ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );
	//LINEA ABAJO ROLL
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_ROLL - 1 * AMPLIAR_Y2 ), cv::Point ( frame.size().width, CENTRO_Y_ROLL - 1 * AMPLIAR_Y2 ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );

	for ( int i = 0; i < int ( roll.size() - 1 ); i++ ) {
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<0> ( roll[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<0> ( roll[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 180, 0, 0 ), 1, 8, 0 );
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<1> ( roll[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<1> ( roll[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 0, 180, 0 ), 1, 8, 0 );
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<2> ( roll[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<2> ( roll[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 0, 0, 200 ), 1, 8, 0 );

		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<3> ( roll[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<3> ( roll[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 200, 200, 0 ), 2, 8, 0 );
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_ROLL + std::get<4> ( roll[i] ) * AMPLIAR_Y / 30 ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_ROLL + std::get<4> ( roll[i + 1] ) * AMPLIAR_Y / 30 ), CV_RGB ( 0, 255, 255 ), 2, 8, 0 );

	}

	//********************  PARA LA ALTITUDE  ************
	//LINEA CENTRO
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_YAW ), cv::Point ( frame.size().width, CENTRO_Y_YAW ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );
	//LINEA ARRIBA
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_YAW + 1 * AMPLIAR_Y2 ), cv::Point ( frame.size().width, CENTRO_Y_YAW + 1 * AMPLIAR_Y2 ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );
	//LINEA ABAJO
	cv::line ( frame, cv::Point ( 0, CENTRO_Y_YAW - 1 * AMPLIAR_Y2 ), cv::Point ( frame.size().width, CENTRO_Y_YAW - 1 * AMPLIAR_Y2 ), CV_RGB ( 0, 0, 0 ), 1, 8, 0 );

	for ( int i = 0; i < int ( altitude.size() - 1 ); i++ ) {
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_YAW + std::get<0> ( altitude[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_YAW + std::get<0> ( altitude[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 200, 0, 0 ), 1, 8, 0 );
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_YAW + std::get<1> ( altitude[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_YAW + std::get<1> ( altitude[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 0, 180, 0 ), 1, 8, 0 );
		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_YAW + std::get<2> ( altitude[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_YAW + std::get<2> ( altitude[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 0, 0, 200 ), 1, 8, 0 );

		cv::line ( frame, cv::Point ( i * AMPLIAR_X, CENTRO_Y_YAW + std::get<3> ( altitude[i] ) * AMPLIAR_Y ), cv::Point ( ( i + 1 ) * AMPLIAR_X, CENTRO_Y_YAW + std::get<3> ( altitude[i + 1] ) * AMPLIAR_Y ), CV_RGB ( 200, 200, 0 ), 2, 8, 0 );
	}

	cv::putText ( frame, "proportional", cv::Point ( 10, 10 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB ( 255, 0, 0 ) );
	cv::putText ( frame, "integrate", cv::Point ( 10, 30 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB ( 0, 255, 0 ) );
	cv::putText ( frame, "derivate", cv::Point ( 10, 50 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB ( 0, 0, 255 ) );
	cv::putText ( frame, "total_pitch", cv::Point ( 10, 70 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB ( 200, 200, 0 ) );
	cv::putText ( frame, "ROLL", cv::Point ( 10, CENTRO_Y_ROLL - AMPLIAR_Y2 + 20 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB ( 200, 200, 0 ) );
	cv::putText ( frame, "ALTITUDE", cv::Point ( 10, CENTRO_Y_YAW - AMPLIAR_Y2 + 20 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB ( 200, 200, 0 ) );

	cv::line ( frame, cv::Point ( frame.size().width - 100, 55 ), cv::Point ( frame.size().width - 10, 55 ), CV_RGB ( 0, 0, 0 ), 2, 8, 0 );

	cv::line ( frame, cv::Point ( frame.size().width - 55, 10 ), cv::Point ( frame.size().width - 55, 100 ), CV_RGB ( 0, 0, 0 ), 2, 8, 0 );

	cv::circle ( frame, cv::Point ( frame.size().width - 55 + std::get<4> ( roll.back() ) * 5, 55 ), 4, CV_RGB ( 255, 0, 255 ), -1, 8, 0 );

	cv::circle ( frame, cv::Point ( frame.size().width - 55, 55 - std::get<4> ( pitch.back() ) * 5 ), 4, CV_RGB ( 255, 0, 0 ), -1, 8, 0 );

}

void update_robot_debug_frame ( cv::Mat& frame, VectorPIDValues roll, VectorPIDValues pitch, VectorPIDValues yaw, VectorPIDValues altitude, boost::shared_ptr<Environment> env, Point position, Velocity velocity, boost::shared_ptr<MessageServer> messageServer )
{
	float ALT_CAMERA = env->getConfigurationCameraHeight();

	auto draw_cross = [&frame] ( const Point point, const int thickness ) {
		cv::line ( frame, cv::Point ( point.x, point.y - 5 ), cv::Point ( point.x, point.y + 5 ), CV_RGB ( 0, 0, 0 ), thickness, 16, 0 );
		cv::line ( frame, cv::Point ( point.x - 5, point.y ), cv::Point ( point.x + 5, point.y ), CV_RGB ( 0, 0, 0 ), thickness, 16, 0 );
	};

	// ------------------------------
	// RECTANGLE
	int width = ( frame.cols - ( frame.cols * ( ALT_CAMERA - std::get<4> ( altitude.back() ) ) / ALT_CAMERA ) ) / 2;
	int height = ( frame.rows - ( frame.rows * ( ALT_CAMERA - std::get<4> ( altitude.back() ) ) / ALT_CAMERA ) ) / 2;

	cv::line ( frame, cv::Point ( width, height ), cv::Point ( frame.cols - width, height ), CV_RGB ( 0, 0, 0 ), 2, 8, 0 );
	cv::line ( frame, cv::Point ( width, height ), cv::Point ( width, frame.rows - height ), CV_RGB ( 0, 0, 0 ), 2, 8, 0 );
	cv::line ( frame, cv::Point ( frame.cols - width, height ), cv::Point ( frame.cols - width, frame.rows - height ), CV_RGB ( 0, 0, 0 ), 2, 8, 0 );
	cv::line ( frame, cv::Point ( width, frame.rows - height ), cv::Point ( frame.cols - width, frame.rows - height ), CV_RGB ( 0, 0, 0 ), 2, 8, 0 );
	// ------------------------------

	// ------------------------------
	cv::Size size;
	size.width = frame.cols;
	size.height = frame.rows;

	// DESTINATIONS
	for ( SafeSpot dest : env->getSpots() ) {
		Point p = Util::rpoint_to_ipoint ( dest.pos, size, env->getConfigurationSpaceSize() );

		draw_cross ( p, 1 );
	}

	// ------------------------------

	// ------------------------------
	// NEXT DESTINATION
	int id = messageServer->getInt ( "rutina/destination/id", 0 );
	
	SafeSpot* spot = env->getSpot ( id );
	if ( spot != NULL ) {
		Point p = Util::rpoint_to_ipoint ( spot->pos, size, env->getConfigurationSpaceSize() );

		draw_cross ( p, 2 );
	}

	// ROBOT
	Point r_position = env->getRobotPostionNormalized ( std::get <4> ( altitude.back() ) );
	draw_robot ( frame, env, roll, pitch, yaw, position, velocity );

	write_robot_info ( frame, roll, pitch, yaw, altitude, env, position, velocity, messageServer );

	// ------------------------------
}
void write_robot_info ( cv::Mat& frame, VectorPIDValues roll, VectorPIDValues pitch, VectorPIDValues yaw, VectorPIDValues altitude, boost::shared_ptr<Environment> env, Point position, Velocity velocity, boost::shared_ptr<MessageServer> messageServer )
{

// informacion en la pantalla.

	string line1 = cv::format ( "SET-> Pitch: %.2f, Roll: %.2f, Yaw: %.2f, Z: %.2f",
	                            std::get<3> ( pitch.back() ), std::get<3> ( roll.back() ),
	                            std::get<3> ( yaw.back() ), std::get<3> ( altitude.back() ) );

	string line2 = cv::format (
	                   "GET--> Pitch: %.3f , Roll: %.3f, Yaw: %.3f, Altitude: %.0f cm",
	                   std::get<4> ( pitch.back() ), std::get<4> ( roll.back() ),
	                   std::get<4> ( yaw.back() ), std::get<4> ( altitude.back() ) * 100 );
	string line3 = cv::format (
	                   "Vel-X: %.3f, Vel-Y: %.3f, Vel-Z: %.3f cm/s Bateria: %d%%, ",
	                   velocity.x, velocity.y, velocity.z, messageServer->getInt ( "robot/battery", 0 ) );
	/*string line4 = cv::format("Battery: %d %%, State: %s",
			threadAttr->data.copterValues.battery, threadAttr->data.copterValues.ctrl_state_sz.c_str());
	*/

	

	string line4 = cv::format ( "Destino: X: %.1f, Y: %.1f, Z: %.0fcm Dist: %d, Dx: %d, Dy: %d, Ang: %.2f, SPAng: %.2f",
	                            messageServer->getFloat ( "rutina/destination/x", 0 ),
								messageServer->getFloat ( "rutina/destination/y", 0 ),
								messageServer->getFloat ( "rutina/destination/z", 0 ) / 10.0,
	                            messageServer->getLong ( "robot/destino/dist/total", 0 ),
	                            messageServer->getLong ( "robot/destino/dist/x", 0 ),
	                            messageServer->getLong ( "robot/destino/dist/y", 0 ),
	                            messageServer->getFloat ( "robot/destino/angulo", 0. ),
	                            messageServer->getFloat ( "robot/destino/setpointyaw", 0. ) );

	string line5 = cv::format ( "POSICION--> X: %.2f, Y: %.2f, Tierra: %s Estado: %d AutoControl: %s",
	                            position.x, position.y, ( messageServer->getInt ( "robot/onground", 1 ) == 1 ) ? "Land" : "TakeOff",
	                            messageServer->getLong ( "robot/state", 0 ),
	                            messageServer->get ( "gui/action/autocontrol", "false" ).c_str() );

	string line6 = cv::format ( "%s  %s", 
	                            messageServer->getBool ( "gui/grabar", false ) ? "REC" : "",
								messageServer->getBool ( "robot/estabilizado", false ) ? "Estab." : "NO Estab."
  							);

	//string line6 = cv::format("tiempo chekpoint: %d", msChangeDestination);

	cv::putText ( frame, line1, cv::Point ( 10, 10 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CF_RED );
	cv::putText ( frame, line2, cv::Point ( 10, 30 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CF_BLUE );
	cv::putText ( frame, line3, cv::Point ( 10, 50 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CF_BLUE );
	cv::putText ( frame, line4, cv::Point ( 10, 70 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CF_BLACK );
	cv::putText ( frame, line5, cv::Point ( 10, 90 ), CV_FONT_HERSHEY_SIMPLEX, 0.4, CF_BLACK );
	cv::putText( frame, line6, cv::Point(10, 110), CV_FONT_HERSHEY_SIMPLEX, 0.4, CF_RED);
	//cv::putText( frame, line6, cv::Point(10, 110), CV_FONT_HERSHEY_SIMPLEX, 0.4, BLACK );


}

void update_vector_pid_values ( boost::shared_ptr<MessageServer> server, VectorPIDValues& pitch, VectorPIDValues& roll, VectorPIDValues& yaw, VectorPIDValues& altitude, Point& position, Velocity& velocity )
{
	if ( pitch.size() >= 200 )
		pitch.erase ( pitch.begin() );

	if ( roll.size() >= 200 )
		roll.erase ( roll.begin() );

	if ( yaw.size() >= 200 )
		yaw.erase ( yaw.begin() );

	if ( altitude.size() >= 200 )
		altitude.erase ( altitude.begin() );

	float pKp = server->getFloat ( "robot/pitch/kp", 0 );
	float pKi = server->getFloat ( "robot/pitch/ki", 0 );
	float pKd = server->getFloat ( "robot/pitch/kd", 0 );
	float pSet = server->getFloat ( "robot/pitch/set", 0 );
	float pValue = server->getFloat ( "robot/pitch/value", 0 );

	pitch.push_back ( std::make_tuple ( pKp, pKi, pKd, pSet, pValue ) );

	float rKp = server->getFloat ( "robot/roll/kp", 0 );
	float rKi = server->getFloat ( "robot/roll/ki", 0 );
	float rKd = server->getFloat ( "robot/roll/kd", 0 );
	float rSet = server->getFloat ( "robot/roll/set", 0 );
	float rValue = server->getFloat ( "robot/roll/value", 0 );

	roll.push_back ( std::make_tuple ( rKp, rKi, rKp, rSet, rValue ) );

	float yKp = server->getFloat ( "robot/yaw/kp", 0 );
	float yKi = server->getFloat ( "robot/yaw/ki", 0 );
	float yKd = server->getFloat ( "robot/yaw/kd", 0 );
	float ySet = server->getFloat ( "robot/yaw/set", 0 );
	float yValue = server->getFloat ( "robot/yaw/value", 0 );

	yaw.push_back ( std::make_tuple ( yKp, yKi, yKd, ySet, yValue ) );

	float aKp = server->getFloat ( "robot/altitude/kp", 0 ) ;
	float aKi = server->getFloat ( "robot/altitude/ki", 0 ) ;
	float aKd = server->getFloat ( "robot/altitude/kd", 0 ) ;
	float aSet = server->getFloat ( "robot/altitude/set", 0 );
	float aValue = server->getFloat ( "robot/altitude/value", 0 );

	altitude.push_back ( std::make_tuple ( aKp, aKi, aKd, aSet, aValue ) );

	velocity.x = server->getFloat ( "robot/velocity/x", 0 );
	velocity.y = server->getFloat ( "robot/velocity/y", 0 );
	velocity.z = server->getFloat ( "robot/velocity/y", 0 );
	position.x = server->getFloat ( "camera/robot_position/x", -1 );
	position.y = server->getFloat ( "camera/robot_position/y", -1 );

}

//  this function should be improved.
void draw_robot ( cv::Mat& frame, boost::shared_ptr<Environment> env, VectorPIDValues roll, VectorPIDValues pitch, VectorPIDValues yaw, Point position, Velocity velocity )
{

	auto to_cv_point = [] ( const Point p ) {
		return cv::Point ( p.x, p.y );
	};

	float yaw_value = std::get<4> ( yaw.back() );
	float pitch_set = std::get<3> ( pitch.back() );
	float roll_set = std::get<3> ( roll.back() );

	cv::Size size;
	size.width = frame.cols;
	size.height = frame.rows;

	Point posPx = Util::rpoint_to_ipoint ( position, size, env->getConfigurationSpaceSize() );

	Point pt;
	Point pt2;

	float ajusteSet = 200;
	float ajusteGet = 5;

	// ************ seteo los sets ************
	//pitch set
	//desde
	pt2.y = 0;
	pt2.x = 4;
	pt2 = Util::rotate ( pt2, yaw_value );

	pt2.x += posPx.x;
	pt2.y += posPx.y;

	//hasta
	pt.y = pitch_set * ajusteSet * -1;
	pt.x = 0;

	pt = Util::rotate ( pt, yaw_value );

	pt.y = posPx.y + pt.y;
	pt.x = posPx.x + 2 + pt.x;

	cv::line ( frame, to_cv_point ( pt2 ), to_cv_point ( pt ), CV_RGB ( 255, 0, 0 ), 1, 8, 0 );

	//roll set
	//desde
	pt2.y = 4;
	pt2.x = 0;

	pt2 = Util::rotate ( pt2, yaw_value );

	pt2.x += posPx.x;
	pt2.y += posPx.y;

	//hasta
	pt.x = roll_set * ajusteSet;
	pt.y = 0;

	pt = Util::rotate ( pt, yaw_value );

	pt.x = posPx.x + pt.x;
	pt.y = posPx.y + 2 + pt.y;

	cv::line ( frame, to_cv_point ( pt2 ), to_cv_point ( pt ), CV_RGB ( 255, 0, 0 ), 1, 8, 0 );

	//********** seteo las inclinaciones *********
	//pitch value
	//desde
	pt2.y = 0;
	pt2.x = -4;

	pt2 = Util::rotate ( pt2, yaw_value );

	pt2.x += posPx.x;
	pt2.y += posPx.y;

	//hasta
	pt.y = velocity.y * ajusteGet / 10 * -1;                //pitch
	pt.x = 0;

	pt = Util::rotate ( pt, yaw_value );

	pt.y = pt2.y + pt.y;
	pt.x = pt2.x + pt.x;

	cv::line ( frame, to_cv_point ( pt2 ), to_cv_point ( pt ), CV_RGB ( 0, 0, 255 ), 1, 8, 0 );

	//roll value
	//desde
	pt2.y = -4;
	pt2.x = 0;

	pt2 = Util::rotate ( pt2, yaw_value );

	pt2.x += posPx.x;
	pt2.y += posPx.y;

	//hasta
	pt.x = velocity.x * ajusteGet / 10;                     //roll
	pt.y = 0;

	pt = Util::rotate ( pt, yaw_value );

	pt.x = pt2.x + pt.x;
	pt.y = pt2.y + pt.y;

	cv::line ( frame, to_cv_point ( pt2 ), to_cv_point ( pt ), CV_RGB ( 0, 0, 255 ), 1, 8, 0 );

	//dibujo el robot
	//linea 1
	//punto1
	pt.x = -7;
	pt.y = 0;

	pt = Util::rotate ( pt, yaw_value );

	pt.x = posPx.x + pt.x;
	pt.y = posPx.y + pt.y;

	//punto 2
	pt2.x = 7;
	pt2.y = 0;

//     std::cout << yaw_value << std::endl;

	pt2 = Util::rotate ( pt2, yaw_value );

	pt2.x = posPx.x + pt2.x;
	pt2.y = posPx.y + pt2.y;
	cv::line ( frame, to_cv_point ( pt2 ), to_cv_point ( pt ), CV_RGB ( 0, 0, 0 ), 2, 8, 0 );

	//linea 2
	//punto1
	pt.x = 0;
	pt.y = -14;

	pt = Util::rotate ( pt, yaw_value );

	pt.x = posPx.x + pt.x;
	pt.y = posPx.y + pt.y;

	//punto 2
	pt2.x = 0;
	pt2.y = 7;

	pt2 = Util::rotate ( pt2, yaw_value ) ;

	pt2.x = posPx.x + pt2.x;
	pt2.y = posPx.y + pt2.y;
	cv::line ( frame, to_cv_point ( pt2 ), to_cv_point ( pt ), CV_RGB ( 0, 0, 0 ), 2, 8, 0 );

}

void grabar ( cv::Mat&  robot, cv::Mat& graphics, cv::Mat& morphology, cv::VideoWriter& vw1, cv::VideoWriter& vw2, std::string path, std::string file_name )
{
	//vw1 es robot + graphics
	cv::Size sizeGrande = cv::Size ( robot.size().width + graphics.size().width, ( robot.size().height > graphics.size().height ) ? robot.size().height : graphics.size().height );

	if ( !vw1.isOpened() ) {
		string source = path;
		source.append ( file_name );
		source.append ( "_1" );
		source.append ( ".avi" );
		// Open the output
		vw1.open ( source, CV_FOURCC ( 'X', 'V', 'I', 'D' ), 12,
		           sizeGrande, true );

		if ( !vw1.isOpened() ) {
			std::printf ( "Could not open the output video for write: %s", source.c_str() );
		}
	}

	cv::Mat grande ( sizeGrande, CV_8UC3,
	                 cv::Scalar ( 255, 255, 255 ) );

	robot.copyTo ( grande.colRange ( 0, robot.size().width ).rowRange ( 0, robot.size().height ) );

	graphics.copyTo ( grande.colRange ( robot.size().width, robot.size().width + graphics.size().width ).rowRange ( 0,
	                  graphics.size().height ) );
	vw1.write ( grande );


	//vw2 es morphology
	if ( !vw2.isOpened() ) {
		string source = path;
		source.append ( file_name );
		source.append ( "_2" );
		source.append ( ".avi" );
		// Open the output
		vw2.open ( source, CV_FOURCC ( 'X', 'V', 'I', 'D' ), 12,
		           morphology.size(), true );

		if ( !vw2.isOpened() ) {
			std::printf ( "Could not open the output video for write: %s", source.c_str() );
		}
	}

	vw2.write ( morphology );

}
