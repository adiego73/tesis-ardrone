project(tesis-ardrone CXX)

cmake_minimum_required(VERSION 2.8.4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_subdirectory(src)

set(USER_LIB_PATH "/usr/local/lib")
set(BOOST_LIB_PATH "/opt/boost_1_57_0/lib")
set(FFMPEG_LIB_PATH "/opt/ffmpeg/lib")

include_directories(./includes/ 
                    /usr/local/include 
                    /opt/boost_1_57_0/include 
                    /usr/local/include/opencv 
                    /usr/local/include/opencv2 )
                    
add_executable(tesis-ardrone main.cpp 

                             src/core/message/message_server.cpp
                             src/core/data/videodata.cpp
                             src/core/camera_thread.cpp
                             src/core/robot_thread.cpp
                             src/core/gui_thread.cpp
                            
                             src/util/util.cpp
                             src/classes/video.cpp 
                             src/classes/environment.cpp 
                             src/classes/environment_configuration.cpp
                )

# pthread
#find_library(PTHREAD_LIB NAMES pthread)

# opencv
find_package( OpenCV REQUIRED )

# boost
find_library(BOOST_THREAD_LIB NAMES boost_thread PATHS ${BOOST_LIB_PATH})
find_library(BOOST_SYSTEM_LIB NAMES boost_system PATHS ${BOOST_LIB_PATH})

target_link_libraries(tesis-ardrone ${CMAKE_DL_LIBS} 
                                    ${OpenCV_LIBS}
                                    ${BOOST_THREAD_LIB} 
                                    ${BOOST_SYSTEM_LIB} 
                                    )                                   
