#include "core/threads.hpp"

using namespace tesis;

void gui_thread( boost::shared_ptr<MessageServer> messageServer, boost::shared_ptr<VideoData> videoProxy)
{
    bool quit;
    cv::Mat frame;
    std::string window_name = "Video Camera";
    
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    
    frame = videoProxy->readFrame();
    cv::imshow(window_name, frame);
    
    while (!quit && !frame.empty())
    {
        frame = videoProxy->readFrame();
        
        cv::imshow(window_name, frame);
        
        std::string finish_msg = messageServer->get( "gui/finish", "false" );
        std::istringstream( finish_msg ) >> std::boolalpha >> quit;
        
        std::cout << messageServer->get("camera/elapsed_time") <<  std::endl;
        
        
        cv::waitKey(1);
        
    }

}
