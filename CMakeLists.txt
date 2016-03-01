cmake_minimum_required(VERSION 2.8.4)

project(tesis-ardrone CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "/opt/boost_1_57_0/include")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/opt/boost_1_57_0/lib")
    
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

add_subdirectory(src)

set(USER_LIB_PATH "/usr/local/lib")

include_directories(./includes/ 
                    /usr/local/include 
                    ${Boost_INCLUDE_DIRS} 
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

# opencv
find_package( OpenCV REQUIRED )


# boost
find_package(Boost 1.57.0 REQUIRED COMPONENTS system thread chrono)

target_link_libraries(tesis-ardrone ${CMAKE_DL_LIBS} 
                                    ${OpenCV_LIBS}
                                    ${Boost_LIBRARIES}
                                    )         

install(TARGETS tesis-ardrone DESTINATION bin/)
install(FILES config/environment.json DESTINATION bin/config/)
                                   
