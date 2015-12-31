project(tesis-ardrone)

cmake_minimum_required(VERSION 2.8.4)

set(USER_LIB_PATH /usr/local/lib)
set(BOOST_LIB_PATH /opt/boost_1_57_0/lib)
set(CVDRONE_LIB_PATH /opt/cvdrone/lib)

add_executable(tesis-ardrone src/video.cpp src/environment.cpp main.cpp)

install(TARGETS tesis-ardrone RUNTIME DESTINATION bin)

include_directories(includes/ 
                    includes/core/
                    /usr/local/include 
                    /opt/cvdrone/include 
                    /opt/boost_1_57_0/include 
                    /usr/local/include/opencv 
                    /usr/local/include/opencv2)

add_subdirectory(src)
add_subdirectory(includes)

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

target_link_libraries(tesis-ardrone ${PTHREAD_LIB} 
                                    ${OPENCV_CORE_LIB} 
                                    ${OPENCV_HIGHGUI_LIB} 
                                    ${OPENCV_IMGPROC_LIB} 
                                    ${OPENCV_VIDEO_LIB}
                                    ${BOOST_THREAD_LIB} 
                                    ${BOOST_SYSTEM_LIB} 
                                    ${CVDRONE_LIB})
