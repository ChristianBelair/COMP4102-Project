########################################################
# CMake file for our COMP4102 project                  #
# run cmake . in this directory to generate a makefile #
# then run make to build our stuff                     #
########################################################
cmake_minimum_required(VERSION 3.5)

project(AutomotiveSafetySuite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable(EyeTrackTest EyeTracking/EyeTrackingTest.cpp)
target_link_libraries( EyeTrackTest ${OpenCV_LIBS} )
