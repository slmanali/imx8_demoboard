QT += core gui multimedia widgets

CONFIG += c++17
TARGET = video_player
TEMPLATE = app

SOURCES += main.cpp \
           VideoPlayer.cpp

HEADERS += VideoPlayer.h

# OpenCV
INCLUDEPATH += /usr/include/opencv4
LIBS += `pkg-config --libs opencv4`

# GStreamer
CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0 gstreamer-video-1.0 gstreamer-audio-1.0
