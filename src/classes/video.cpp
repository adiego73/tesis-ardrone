#include "classes/video.hpp"

using namespace tesis;

Video::Video ( int camera ) : cap ( camera )
{
	if ( !this->cap.isOpened() ) {
		std::ostringstream message;
		message << "The camera " << camera << " couldn't be opened";
		throw std::invalid_argument ( message.str() );
	}

	this->cap.set ( CV_CAP_PROP_FRAME_WIDTH, 640 );
	this->cap.set ( CV_CAP_PROP_FRAME_HEIGHT, 480 );
	this->img_size.height = this->cap.get ( CV_CAP_PROP_FRAME_HEIGHT );
	this->img_size.width = this->cap.get ( CV_CAP_PROP_FRAME_WIDTH );
	this->fps = this->cap.get ( CV_CAP_PROP_FPS );

	radio = 0;

	double data[3][3] = {{ 387.56134746, 0., 312.88026831},
		{0., 387.31586735, 223.29567174},
		{0., 0., 1.}
	};

	double data2[5] = { -0.20259054, 0.11998335, 0.00658746, 0.00440328, -0.12875139};

	memcpy ( dIntrinsic_matrix, data, sizeof ( double ) * 3 * 3 );
	memcpy ( dDistCoeffs, data2, sizeof ( double ) * 5 );
	intrinsic_matrix = cv::Mat ( 3, 3, CV_64F, &dIntrinsic_matrix );
	distCoeffs = cv::Mat ( 5, 1, CV_64F, &dDistCoeffs );
}


Video::~Video()
{
	this->cap.release();
}

void Video::capture()
{
	cv::Mat orig;
	this->cap.read ( orig );

	if ( !orig.empty() )
		cv::undistort ( orig, this->frame, intrinsic_matrix, distCoeffs );

}

cv::Mat Video::getFrame()
{
	if ( this->frame.empty() )
		this->capture();

	return this->frame;
}


cv::Mat Video::getMorphology()
{

	if ( this->morphology.empty() )
		this->morphology = cv::Mat ( this->getFrameSize(), CV_8UC1 );

	return this->morphology;
}

cv::Size Video::getFrameSize()
{
	return this->img_size;
}

float Video::getFrameHeight()
{
	return this->getFrameSize().height;
}

float Video::getFrameWidth()
{
	return this->getFrameSize().width;
}



Point Video::trackColor ( Color color )
{
	Point pos;
	cv::Mat frame = this->getFrame();

	if ( frame.empty() ) {
		Point p;
		p.x = -1;
		p.y = -1;
		return p;
	}

	cv::Size imageSize = this->getFrameSize();

	cv::Mat frame_th = cv::Mat ( imageSize, CV_8UC1 );
	cv::Mat frame_th2 = cv::Mat ( imageSize, CV_8UC1 );
	cv::Mat img_2 = cv::Mat ( imageSize, CV_8UC3 );

	cv::Mat morphKernel = cv::getStructuringElement ( cv::MORPH_RECT, cv::Size ( 5, 5 ), cv::Point ( 1, 1 ) );

	cv::Scalar thresholdMin;
	cv::Scalar thresholdMax;

	thresholdMin.val[0] = color.Hue.min;
	thresholdMin.val[1] = color.Saturation.min;
	thresholdMin.val[2] = color.Value.min;

	thresholdMax.val[0] = color.Hue.max;
	thresholdMax.val[1] = color.Saturation.max;
	thresholdMax.val[2] = color.Value.max;

	cv::cvtColor ( frame, img_2, CV_BGR2HSV );
	cv::inRange ( img_2, thresholdMin, thresholdMax , frame_th );

	cv::morphologyEx ( frame_th, frame_th2, CV_MOP_OPEN, morphKernel );

	cv::Moments moments_1;
	cv::Moments moments_2;

	double moment10;
	double moment01;
	double area, r;

	moments_1 = cv::moments ( frame_th2, 1 );

	moment10 = moments_1.m10;
	moment01 = moments_1.m01;
	area = moments_1.m00;

	if ( area > 3 ) {
		pos.x = moment10 / area;
		pos.y = moment01 / area;
	} else {
		pos.x = -1;
		pos.y = -1;
	}

	return pos;
}

Point Video::trackColor ( Color color, Color color2 )
{
	cv::Point pos;
	cv::Mat frame = this->getFrame();

	if ( frame.empty() ) {
		Point p;
		p.x = -1;
		p.y = -1;
		return p;
	}

	cv::Size imageSize = this->getFrameSize();

	cv::Mat frame_th = cv::Mat::zeros ( imageSize, CV_8UC1 );
	cv::Mat frame_th2 = cv::Mat ( imageSize, CV_8UC1 );
	cv::Mat img_2 = cv::Mat ( imageSize, CV_8UC3 );

	cv::Scalar thresholdMin;
	cv::Scalar thresholdMax;

	cv::Scalar thresholdMin2;
	cv::Scalar thresholdMax2;
	//color1
	thresholdMin.val[0] = color.Hue.min;
	thresholdMin.val[1] = color.Saturation.min;
	thresholdMin.val[2] = color.Value.min;

	thresholdMax.val[0] = color.Hue.max;
	thresholdMax.val[1] = color.Saturation.max;
	thresholdMax.val[2] = color.Value.max;
	//color2
	thresholdMin2.val[0] = color2.Hue.min;
	thresholdMin2.val[1] = color2.Saturation.min;
	thresholdMin2.val[2] = color2.Value.min;

	thresholdMax2.val[0] = color2.Hue.max;
	thresholdMax2.val[1] = color2.Saturation.max;
	thresholdMax2.val[2] = color2.Value.max;

	cv::cvtColor ( frame, img_2, CV_BGR2HSV );
	cv::inRange ( img_2, thresholdMin, thresholdMax , frame_th );


	cv::Rect rect;
	cv::Point p1;
	cv::Point p2;

	if ( lastFind.x != -1 && lastLastFind.x != -1 ) {
		cv::Point delta;
		delta = lastFind - lastLastFind;

		//p1.x
		if ( delta.x < 0 )
			p1.x = lastLastFind.x + radio;
		else if ( delta.x > 0 )
			p1.x = lastLastFind.x - radio;
		else
			p1.x = lastLastFind.x - radio * 2;

		//p1.y
		if ( delta.y < 0 )
			p1.y = lastLastFind.y + radio;
		else if ( delta.y > 0 )
			p1.y = lastLastFind.y - radio;
		else
			p1.y = lastLastFind.y - radio * 2;

		p1.x = cv::max ( p1.x, 0 );
		p1.y = cv::max ( p1.y, 0 );

		p1.x = cv::min ( p1.x, imageSize.width - 1 );
		p1.y = cv::min ( p1.y, imageSize.height - 1 );


		p2 = lastFind + delta;

		//p2.x
		if ( delta.x < 0 )
			p2.x -= radio * 3;
		else if ( delta.x > 0 )
			p2.x += radio * 3;
		else
			p2.x += radio * 2;

		//p2.y
		if ( delta.y < 0 )
			p2.y -= radio * 3;
		else if ( delta.y > 0 )
			p2.y += radio * 3;
		else
			p2.y += radio * 2;

		p2.x = cv::max ( p2.x, 0 );
		p2.y = cv::max ( p2.y, 0 );

		p2.x = cv::min ( p2.x, imageSize.width - 1 );
		p2.y = cv::min ( p2.y, imageSize.height - 1 );
	}

	else if ( lastFind.x != -1 ) {

		//p1.x
		p1.x = lastFind.x - radio * 2;

		//p1.y
		p1.y = lastFind.y - radio * 2;
		//p2.x
		p2.x = lastFind.x + radio * 2;

		//p2.y
		p2.y = lastFind.y + radio * 2;
	}

	if ( lastFind.x != -1 ) {
		p1.x = cv::max ( p1.x, 0 );
		p1.y = cv::max ( p1.y, 0 );

		p1.x = cv::min ( p1.x, imageSize.width - 1 );
		p1.y = cv::min ( p1.y, imageSize.height - 1 );

		p2.x = cv::max ( p2.x, 0 );
		p2.y = cv::max ( p2.y, 0 );

		p2.x = cv::min ( p2.x, imageSize.width - 1 );
		p2.y = cv::min ( p2.y, imageSize.height - 1 );

		//busco otra porcion de rojo
		cv::Rect rect = cv::Rect ( p1, p2 );

		//std::cout << p1.x<< "  "<< p1.y<< "  "<< p2.x << "  "<<p2.y <<std::endl;
		if ( rect.width != 0 && rect.height != 0 ) {
			cv::Mat subImg = img_2 ( rect );

			cv::Mat frame_th_small1 = cv::Mat ( subImg.size(), CV_8UC1 );

			cv::inRange ( subImg, thresholdMin2, thresholdMax2 , frame_th_small1 );

			cv::Mat frame_th_small2 = frame_th ( rect );

			cv::bitwise_or ( frame_th_small2, frame_th_small1, frame_th_small2 );

			frame_th_small2.copyTo ( frame_th.colRange ( rect.x, rect.x + rect.width ).rowRange ( rect.y, rect.y + rect.height ) );
		}
	}


	pos = getMoment ( frame_th );

	if ( pos.x == -1 ) {
		cv::inRange ( img_2, thresholdMin2, thresholdMax2 , frame_th2 );

		cv::bitwise_or ( frame_th, frame_th2, frame_th );

		pos = getMoment ( frame_th );
	}

	morphology = frame_th.clone();
	lastLastFind = lastFind;
	lastFind = pos;
	return { ( float ) pos.x, ( float ) pos.y, -1.0};
}

cv::Point Video::getMoment ( cv::Mat frame )
{
	cv::Point pos;
	cv::Mat dest = cv::Mat ( frame.size(), CV_8UC1 );

	cv::Mat morphKernel = cv::getStructuringElement ( cv::MORPH_ELLIPSE, cv::Size ( 2, 2 ), cv::Point ( 1, 1 ) );
	cv::morphologyEx ( frame, dest, CV_MOP_OPEN, morphKernel );

	morphKernel = cv::getStructuringElement ( cv::MORPH_ELLIPSE, cv::Size ( 5, 5 ), cv::Point ( 1, 1 ) );
	cv::dilate ( dest, dest, morphKernel, cv::Point ( 1, 1 ), 2 );

	morphKernel = cv::getStructuringElement ( cv::MORPH_ELLIPSE, cv::Size ( 5, 5 ), cv::Point ( 1, 1 ) );
	cv::morphologyEx ( dest, dest, CV_MOP_OPEN, morphKernel );

	cv::Moments moments_1;
	cv::Moments moments_2;

	double moment10;
	double moment01;
	double area;

	moments_1 = cv::moments ( dest, 1 );


	moment10 = moments_1.m10;
	moment01 = moments_1.m01;
	area = moments_1.m00;
	radio = area / ( M_PI * 2 );

	if ( area > 3 ) {
		pos.x = moment10 / area;
		pos.y = moment01 / area;
	} else {

		pos.x = -1;
		pos.y = -1;
	}

	return pos;
}

