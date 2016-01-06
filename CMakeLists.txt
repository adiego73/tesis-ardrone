project(tesis-ardrone CXX)

cmake_minimum_required(VERSION 2.8.4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_subdirectory(src)

set(USER_LIB_PATH "/usr/local/lib")
set(BOOST_LIB_PATH "/opt/boost_1_57_0/lib")
set(CVDRONE_LIB_PATH "/opt/cvdrone/lib")
set(FFMPEG_LIB_PATH "/opt/ffmpeg/lib")

include_directories(./includes/ 
                    /usr/local/include 
                    #/opt/ffmpeg/include
                    /opt/cvdrone/include 
                    /opt/boost_1_57_0/include 
                    /usr/local/include/opencv 
                    /usr/local/include/opencv2 )
                    
add_executable(tesis-ardrone main.cpp 

                             src/core/camera_thread.cpp
                             src/core/robot_thread.cpp
                             src/core/gui_thread.cpp
                            
                             src/classes/pid.cpp 
                             src/classes/pid_yaw.cpp
                             src/classes/video.cpp 
                             src/classes/environment.cpp 
                             src/classes/configuration_manager.cpp
                             )                           

# pthread
find_library(PTHREAD_LIB NAMES pthread)

# opencv
find_library(OPENCV_CORE_LIB NAMES opencv_core PATHS ${USER_LIB_PATH})
find_library(OPENCV_HIGHGUI_LIB NAMES opencv_highgui PATHS ${USER_LIB_PATH})
find_library(OPENCV_IMGPROC_LIB NAMES opencv_imgproc PATHS ${USER_LIB_PATH})
find_library(OPENCV_VIDEO_LIB NAMES opencv_video PATHS ${USER_LIB_PATH})


# boost
find_library(BOOST_THREAD_LIB NAMES boost_thread PATHS ${BOOST_LIB_PATH})
find_library(BOOST_SYSTEM_LIB NAMES boost_system PATHS ${BOOST_LIB_PATH})

# cvdrone
find_library(CVDRONE_LIB NAMES cvdrone PATHS ${CVDRONE_LIB_PATH})

# fmpeg libs
find_library(AVDEVICE_LIB NAMES avdevice PATHS ${FFMPEG_LIB_PATH})
find_library(AVFILTER_LIB NAMES avfilter PATHS ${FFMPEG_LIB_PATH})
find_library(SWSCALE_LIB NAMES swscale PATHS ${FFMPEG_LIB_PATH})
find_library(AVFORMAT_LIB NAMES avformat PATHS ${FFMPEG_LIB_PATH})
find_library(AVCODEC_LIB NAMES avcodec PATHS ${FFMPEG_LIB_PATH})
find_library(AVUTIL_LIB NAMES avutil PATHS ${FFMPEG_LIB_PATH})

target_link_libraries(tesis-ardrone ${PTHREAD_LIB} 
                                    ${OPENCV_CORE_LIB} 
                                    ${OPENCV_HIGHGUI_LIB} 
                                    ${OPENCV_IMGPROC_LIB} 
                                    ${OPENCV_VIDEO_LIB}
                                    ${BOOST_THREAD_LIB} 
                                    ${BOOST_SYSTEM_LIB} 
                                    ${CVDRONE_LIB} 
                                    ${AVDEVICE_LIB} 
                                    ${AVUTIL_LIB} 
                                    ${AVFILTER_LIB} 
                                    ${SWSCALE_LIB} 
                                    ${AVFORMAT_LIB} 
                                    ${AVCODEC_LIB} 
                                    ${PTHREAD_LIB}
                                    )                                   