#include "classes/video.hpp"

#include <chrono>


using namespace std;

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
	//this->cap.set ( CV_CAP_PROP_FPS, 25 );
	std::cout << "FPS" << this->fps<< std::endl;
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
	/*
	
	cv::initUndistortRectifyMap(intrinsic_matrix, distCoeffs, cv::Mat(),intrinsic_matrix, cv::Size(this->img_size.width, this->img_size.height ), CV_32FC1, map1, map2);
*/
	
	/*
	cv::ocl::setUseOpenCL(false);
	bool has = cv::ocl::haveOpenCL();
	if(has)
		std::cout << "hay opencl" << std::endl;
	else
		std::cout << "no hay opencl" << std::endl;
	
	cv::ocl::Context context;
	if (!context.create(cv::ocl::Device::TYPE_ALL))
	{
		std::cout << "Failed creating the context..." << std::endl;
		//return;
	}

	std::cout << context.ndevices() << " GPU devices are detected." << std::endl; //This bit provides an overview of the OpenCL devices you have in your computer
	for (int i = 0; i < context.ndevices(); i++)
	{
		cv::ocl::Device device = cv::ocl::Device(context.device(i));
		std::cout << "name:              " << device.name() << std::endl;
		std::cout << "available:         " << device.available() << std::endl;
		std::cout << "imageSupport:      " << device.imageSupport() << std::endl;
		std::cout << "OpenCL_C_Version:  " << device.OpenCL_C_Version() << std::endl;
		std::cout << std::endl;
	}
	
	//cv::ocl::Device();
	cv::ocl::Device(context.device(0)); //Here is where you change which GPU to use (e.g. 0 or 1)*/
	/*
	cv::ocl::Device  device = cv::ocl::Device();//context.device(i);
	std::cout << "name:              " << device.name() << std::endl;
	std::cout << "available:         " << device.available() << std::endl;
	std::cout << "imageSupport:      " << device.imageSupport() << std::endl;
	std::cout << "OpenCL_C_Version:  " << device.OpenCL_C_Version() << std::endl;
	std::cout << std::endl;
	*/
}


Video::~Video()
{
	this->cap.release();
}

void Video::capture()
{
	auto start = std::chrono::high_resolution_clock::now();
	
	
	cv::Mat orig;
	cv::Mat orig2;
	this->cap.read ( orig);
	
	if ( !orig.empty() )
		//cv::remap(orig, this->frame, map1, map2,cv::INTER_CUBIC);
	
			cv::undistort ( orig, this->frame, intrinsic_matrix, distCoeffs );

	
	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> ( end - start );

	times.push_back(elapsed_time.count());
	long total = 0;
	for ( long time :  times) {
		total += time;
	}
	if(times.size() > 10)
		times.erase(times.begin());
	
	
		
	//orig2.copyTo(this->frame);
//	std::cout << " Tiempo capture: " <<  total / times.size() << std::endl;//6 - 7tiempo actual con ventana abierta

	

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
	
	auto start = std::chrono::high_resolution_clock::now();
	
	Point pos;
	cv::Mat frame;
	
	frame = this->getFrame();//.copyTo(frame);

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
//	cv::inRange ( img_2, thresholdMin, thresholdMax , frame_th2 );

//	cv::bitwise_or ( frame_th, frame_th2, frame_th );
	
	cv::morphologyEx ( frame_th, frame_th2, CV_MOP_OPEN, morphKernel );
	cv::morphologyEx ( frame_th2, frame_th2, CV_MOP_CLOSE, morphKernel );
	cv::GaussianBlur(frame_th2, frame_th2, cv::Size ( 5, 5 ), 0, 0);
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
	
/*	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> ( end - start );

	times.push_back(elapsed_time.count());
	long total = 0;
	for ( long time :  times) {
		total += time;
	}
	if(times.size() > 10)
		times.erase(times.begin());
	
	std::cout << " Tiempo: " <<  total / times.size() << std::endl;//6 - 7tiempo actual con ventana abierta
*/
	return pos;
}

Point Video::trackColor ( Color color, Color color2 )
{
	
	auto start = std::chrono::high_resolution_clock::now();
	
	Point pos;
	cv::Mat frame;
	
	frame = this->getFrame();//.copyTo(frame);

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

	//color1
	cv::Scalar thresholdMin;
	cv::Scalar thresholdMax;

	thresholdMin.val[0] = color.Hue.min;
	thresholdMin.val[1] = color.Saturation.min;
	thresholdMin.val[2] = color.Value.min;

	thresholdMax.val[0] = color.Hue.max;
	thresholdMax.val[1] = color.Saturation.max;
	thresholdMax.val[2] = color.Value.max;

	//color2
	cv::Scalar thresholdMin2;
	cv::Scalar thresholdMax2;
	thresholdMin2.val[0] = color2.Hue.min;
	thresholdMin2.val[1] = color2.Saturation.min;
	thresholdMin2.val[2] = color2.Value.min;

	thresholdMax2.val[0] = color2.Hue.max;
	thresholdMax2.val[1] = color2.Saturation.max;
	thresholdMax2.val[2] = color2.Value.max;
	
	cv::cvtColor ( frame, img_2, CV_BGR2HSV );
	cv::inRange ( img_2, thresholdMin, thresholdMax , frame_th );
	cv::inRange ( img_2, thresholdMin2, thresholdMax2 , frame_th2 );

	cv::bitwise_or ( frame_th, frame_th2, frame_th );
	
	cv::morphologyEx ( frame_th, frame_th2, CV_MOP_OPEN, morphKernel );
	cv::morphologyEx ( frame_th2, frame_th2, CV_MOP_CLOSE, morphKernel );
	cv::GaussianBlur(frame_th2, frame_th2, cv::Size ( 5, 5 ), 0, 0);
	//morphology = frame_th2;
	
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
	/*
	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> ( end - start );

	times.push_back(elapsed_time.count());
	long total = 0;
	for ( long time :  times) {
		total += time;
	}
	if(times.size() > 10)
		times.erase(times.begin());
	
	std::cout << " Tiempo: " <<  total / times.size() << std::endl;//6 - 7tiempo actual con ventana abierta
*/
	return pos;
}

Point Video::trackColor2 ( Color color, Color color2 )
{
	auto start = std::chrono::high_resolution_clock::now();
	
	
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
	

	auto end = std::chrono::high_resolution_clock::now();

	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds> ( end - start );

	std::cout << "Tiempo2: " <<  elapsed_time.count() << std::endl;
	
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

/*
void fcn_generateIdxImageForUndistort(cv::Mat& idxImg_dest, cv::Mat K_forDistort, cv::Mat distortionCoeffs)
{
	int imgHeight = 480;
    int imgWidth = 640;
	// create an artificial image
    cv::Mat idxImg_src=cv::Mat(imgHeight,imgWidth,CV_8UC4);
    for (int row=0; row<imgHeight; row++)
    {
        for (int col=0; col<imgWidth; col++)
        {
            idxImg_src.at<cv::Vec4b>(row[0]=row%256;
            idxImg_src.at<cv::Vec4b>(row[1]=int(row/256);
            idxImg_src.at<cv::Vec4b>(row[2]=col%256;
            idxImg_src.at<cv::Vec4b>(row[3]=int(col/256);
            // retrieve the row index = idxImg_src.at<Vec4b>(row[0]+256*idxImg_src.at<Vec4b>(row[1]
        }
    }
    // undistort the 4-channel image
    idxImg_dest=cv::Mat(imgHeight,imgWidth,CV_8UC4);
    undistort(idxImg_src, idxImg_dest, K_forDistort, distortionCoeffs);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // next need to modify the idxImg_dest a little bit, otherwise, the final image will have three curves.
    int originalRowIdx0, originalColIdx0;
    int originalRowIdx, originalColIdx;
    // if the originalRowIdx or originalColIdx change too much, then assign this pixel with the previous's pixel's value
    // first row then col
    for (int row=0; row<imgHeight; row++)
    {
        for (int col=0; col<imgWidth; col++)
        {
            originalRowIdx=idxImg_dest.at<cv::Vec4b>(row,col56*idxImg_dest.at<Vec4b>(row[1];
            originalColIdx =idxImg_dest.at<cv::Vec4b>(row,col56*idxImg_dest.at<Vec4b>(row[3];
            if (row==0 || col==0)
            {
                originalRowIdx0=originalRowIdx;
                originalColIdx0=originalColIdx;
            }
            else
            {
                if (abs(originalRowIdx-originalRowIdx0)>6)
                {
                    idxImg_dest.at<cv::Vec4b>(row,coliidxImg_dest.at<Vec4b>(row-1,col                    idxImg_dest.at<Vec4b>(row,coliidxImg_dest.at<Vec4b>(row-1,col                }
                if (abs(originalColIdx-originalColIdx0)>6)
                {
                    idxImg_dest.at<cv::Vec4b>(row,coliidxImg_dest.at<Vec4b>(row,col-1                    idxImg_dest.at<Vec4b>(row,coliidxImg_dest.at<Vec4b>(row,col-1                }
                originalRowIdx0=originalRowIdx;
                originalColIdx0=originalColIdx;
            }
        }
    }
    // first col then row
    for (int col=0; col<imgWidth; col++)
    {
        for (int row=0; row<imgHeight; row++)
        {
            originalRowIdx=idxImg_dest.at<Vec4b>(row,col56*idxImg_dest.at<Vec4b>(row[1];
            originalColIdx =idxImg_dest.at<Vec4b>(row,col56*idxImg_dest.at<Vec4b>(row[3];
            if (row==0 || col==0)
            {
                originalRowIdx0=originalRowIdx;
                originalColIdx0=originalColIdx;
            }
            else
            {
                if (abs(originalRowIdx-originalRowIdx0)>6)
                {
                    idxImg_dest.at<Vec4b>(row,coliidxImg_dest.at<Vec4b>(row-1,col                    idxImg_dest.at<Vec4b>(row,coliidxImg_dest.at<Vec4b>(row-1,col                }
                if (abs(originalColIdx-originalColIdx0)>6)
                {
                    idxImg_dest.at<Vec4b>(row,coliidxImg_dest.at<Vec4b>(row,col-1                    idxImg_dest.at<Vec4b>(row,coliidxImg_dest.at<Vec4b>(row,col-1                }
                originalRowIdx0=originalRowIdx;
                originalColIdx0=originalColIdx;
            }
        }
    }
}

void fcn_MyUndistortArrraryVersion(cv::Mat img_src, cv::Mat& img_dest, cv::Mat idxImg_dest)
{
    if (img_dest.empty())
    {
        printf("fcn_MyUndistort: you input an empty img_dest!! It should be initialized!!");
        return;
    }
    int originalRowIdx, originalColIdx;
    int rowNum=img_src.rows;
    int colNum=img_src.cols;
    // for idxImg_dest
    uchar* pIdxImg=idxImg_dest.data;
    int stepIdxImg=idxImg_dest.step;
    int chIdxImg=idxImg_dest.channels();
    // for img_dest
    uchar* pImgDest=img_dest.data;
    int stepImgDest=img_dest.step;
    int chImgDest=img_dest.channels();
    // for img_src
    uchar* pImgSrc=img_src.data;
    int stepImgSrc=img_src.step;
    int chImgSrc=img_src.channels();
    for (int row=0; row<rowNum; row++)
    {
        for (int col=0; col<colNum; col++)
        {
            // determine the pixel coordinate in the original image
            originalRowIdx=pIdxImg[row*stepIdxImg+col*chIdxImg+0]+256*pIdxImg[row*stepIdxImg+col*chIdxImg+1];
            originalColIdx =pIdxImg[row*stepIdxImg+col*chIdxImg+2]+256*pIdxImg[row*stepIdxImg+col*chIdxImg+3];
            if (originalColIdx==colNum)
            {
                originalColIdx--;
            }
            if (originalRowIdx==row)
            {
                originalRowIdx--;
            }
            if (originalColIdx>=0 && originalColIdx<colNum && originalRowIdx>=0 && originalRowIdx<rowNum)
            {
                pImgDest[row*stepImgDest+col*chImgDest+0]=pImgSrc[originalRowIdx*stepImgSrc+originalColIdx*chImgSrc+0];
                pImgDest[row*stepImgDest+col*chImgDest+1]=pImgSrc[originalRowIdx*stepImgSrc+originalColIdx*chImgSrc+1];
                pImgDest[row*stepImgDest+col*chImgDest+2]=pImgSrc[originalRowIdx*stepImgSrc+originalColIdx*chImgSrc+2];
            }
            else
            {
                //printf("========================\n originalRowIdx=%d, originalColIdx=%d \n", originalRowIdx, originalColIdx);
                // do nothing, the image pixel RBG value is [0,0,0]
            }
        }
    }
}*/

