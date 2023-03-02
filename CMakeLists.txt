cmake_minimum_required(VERSION 3.23)
project(ffmpeg_play)

include_directories(D:/Environment/ffmpeg/include)
link_directories(D:/Environment/ffmpeg/lib)
include_directories()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(SOURCES main.cpp)
# SET(CMAKE_BUILD_TYPE “DEBUG”)

find_package(Qt5 COMPONENTS
        Core
        Widgets
        Gui
        Multimedia
        REQUIRED)

add_executable(ffmpeg_play main.cpp
                        ./ui/ffpemg_player.h
                        ./Headers/netinfo_get.h
                        ./Headers/mainwindow.h
                        ./code/mainwindow.cpp
                        ./code/netinfo_get.cpp
                        ./ui/ffmpeg.qrc
                        ./Headers/lyrices.h
                        ./code/lyrics.cpp)


target_link_libraries(ffmpeg_play
        Qt::Core
        Qt::Widgets
        Qt::Gui
        Qt::Multimedia
        avfilter
        avformat
        avcodec
        avutil
        swresample
        swscale
        )

find_package(OpenCV)
# pkg_check_modules(OpenCV OpenCV4)
link_directories(${OpenCV_LIBRARY_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(ffmpeg_play ${OpenCV_LIBRARIES})
