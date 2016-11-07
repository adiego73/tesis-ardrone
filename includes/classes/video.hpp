#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "structures.hpp"

namespace tesis
{

class Video
{

public:
	Point trackColor ( Color color );
	Point trackColor ( Color color, Color color2 );
	Point trackColor2 ( Color color, Color color2 );
	cv::Mat getFrame();
	cv::Mat getMorphology();
	cv::Size getFrameSize();
	float getFrameWidth();
	float getFrameHeight();
	void capture();

	Video ( int camera );
	~Video();

private:
	cv::Point getMoment ( cv::Mat frame );
	cv::VideoCapture cap;
	cv::Mat frame;
	cv::Mat morphology;
	cv::Size img_size;
	int radio;
	cv::Point lastFind;
	cv::Point lastLastFind;
	int fps;
	double dIntrinsic_matrix[3][3];
	double dDistCoeffs[5];
	cv::Mat intrinsic_matrix;
	cv::Mat distCoeffs;
	cv::Mat map1,map2;
	cv::Mat newCamMatrix;
	
	long time;
	std::vector <long>times;
	
};



};
#endif // VIDEO_HPP
