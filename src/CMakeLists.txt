cmake_minimum_required(VERSION 3.5)

project(AutomtiveSafetySuite LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## OpenCV Stuff
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
## end OpenCV Stuff

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Multimedia MultimediaWidgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Multimedia MultimediaWidgets REQUIRED)

SET(QT_USE_QTMULTIMEDIA TRUE)
SET(QT_USE_QTMULTIMEDIAWIDGETS TRUE)

# Grab eyetrack source files
file(GLOB EYETRACK_SRC ${PROJECT_SOURCE_DIR}/EyeTracking/*.cpp ${PROJECT_SOURCE_DIR}/EyeTracking/*.h)
# Eyetrack lib I'm using
file(GLOB EYETRACK_LIBS
    3rdparty/eyeLike/src/constants.h
    3rdparty/eyeLike/src/findEyeCenter.cpp
    3rdparty/eyeLike/src/findEyeCenter.h
    3rdparty/eyeLike/src/findEyeCorner.cpp
    3rdparty/eyeLike/src/findEyeCorner.h
    3rdparty/eyeLike/src/helpers.cpp
    3rdparty/eyeLike/src/helpers.h
    )

# Eyetracking resource file
configure_file(${PROJECT_SOURCE_DIR}/EyeTracking/haarcascade_frontalface_alt.xml res/haarcascade_frontalface_alt.xml)

# Grab GUI sources
file(GLOB GUI_SRC ${PROJECT_SOURCE_DIR}/GUI/*)

# Grab shared sources
file(GLOB SHARED_SRC ${PROJECT_SOURCE_DIR}/shared/*.cpp ${PROJECT_SOURCE_DIR}/shared/*.h)

# Grab Pedtracking source files
file(GLOB PEDTRACK_SRC ${PROJECT_SOURCE_DIR}/PedTracking/*cpp ${PROJECT_SOURCE_DIR}/PedTracking/*.h)

# Grab SignTracking source files
file(GLOB SIGNTRACK_SRC ${PROJECT_SOURCE_DIR}/SignTracking/*.cc ${PROJECT_SOURCE_DIR}/SignTracking/*.h)

set(PROJECT_SOURCES
        main.cpp
        ${GUI_SRC}
        ${SHARED_SRC}
        ${EYETRACK_SRC}
        ${EYETRACK_LIBS}
        ${PEDTRACK_SRC}
        ${SIGNTRACK_SRC}
)

set(CMAKE_AUTOUIC_SEARCH_PATHS GUI)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AutomtiveSafetySuite
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(AutomtiveSafetySuite SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(AutomtiveSafetySuite
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(AutomtiveSafetySuite PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::MultimediaWidgets  ${OpenCV_LIBS} )
