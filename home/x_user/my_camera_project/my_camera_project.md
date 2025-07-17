# Project Documentation: my_camera_project

This document serves as a comprehensive guide to the `my_camera_project`, detailing its configuration files, build dependencies, and important components. This project is primarily focused on utilizing camera functionalities, multimedia handling, and various software integrations, including OpenCV, GStreamer, and networking capabilities.

## Project Structure

The project is organized into several source files and headers, with the primary entry point being `main.cpp`. The structure consists of various components, each serving specific functionalities required by the camera project.

### Key Components

- **SOURCES**: Contains the implementation files (*.cpp).
  - `main.cpp`: The main application entry point.
  - `camera_viewer.cpp`: Handles the camera viewing functionalities.

- **HEADERS**: Contains the header files (*.h) that define interfaces and functionalities for the project components.
  - `camera_viewer.h`: Declares functions and classes related to camera viewing.
  - `Configuration.h`: Manages application configurations.
  - `Logger.h`: Handles logging features for debugging and information tracking.
  - `gpio.h`: Directly interfaces with GPIO (General Purpose Input/Output) for hardware control.
  - `Audio.h`: Manages audio functionalities within the application.
  - `HTTPSession.h`: Handles HTTP sessions, networking, and communication protocols.
  - `power_management.h`: Contains mechanisms for power management, including sleep and wake functionalities.
  - `speechThread.h`: Supports speech recognition and processing in a separate thread.
  - `camerareader.h`: Facilitates camera data reading and processing.
  - `PDFCreator.h`: Manages PDF creation functionalities.
  - `videocontroller.h`: Manages video functionalities and controls.
  - `LanguageManager.h`: Handles multilingual support and language settings.
  - `FloatingMessage.h`: Displays transient messages in the user interface.
  - `imu_classifier_thread.h`: Supports classification processes from IMU data in a separate thread.

## Build Configuration

The project is built using Qt's qmake system, and the configuration file `my_camera_project.pro` contains the necessary settings for building the application.

### Qt Module Setup

The project uses several Qt modules, which are specified in the `.pro` file:

```pro
QT += core gui widgets concurrent multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
```

### C++ Standard

The project is configured to utilize C++17 standard features:

```pro
CONFIG += c++17
```

### Debug Configuration

The project is set to build in debug mode, enabling detailed debugging information.

```pro
CONFIG += debug
```

### Target

The target name for building the application is specified as `my_camera_project`:

```pro
TARGET = my_camera_project
```

### Source Files

The source files included in the build are specified as follows:

```pro
SOURCES += main.cpp \
           camera_viewer.cpp
```

### Header Files

The associated header files are included through:

```pro
HEADERS += camera_viewer.h \
           Configuration.h \
           Logger.h \
           gpio.h \
           Audio.h \
           HTTPSession.h \
           power_management.h \
           speechThread.h \
           camerareader.h \
           PDFCreator.h \
           videocontroller.h \
           LanguageManager.h \
           FloatingMessage.h \
           imu_classifier_thread.h
```

### Library Dependencies

The project utilizes several external libraries, and the paths to their include directories are specified as follows:

```pro
# Include OpenCV and GStreamer
INCLUDEPATH += /usr/include/opencv4
INCLUDEPATH += /usr/include/gstreamer-1.0
INCLUDEPATH += /usr/lib/aarch64-linux-gnu/gstreamer-1.0
INCLUDEPATH += /usr/include/jsoncpp
INCLUDEPATH += /usr/include/poppler/qt5
INCLUDEPATH += /usr/include/boost
INCLUDEPATH += /home/x_user/my_camera_project/onnxruntime/include

# Include GLib-2.0 (required for GStreamer)
INCLUDEPATH += /usr/include/glib-2.0
INCLUDEPATH += /usr/lib/aarch64-linux-gnu/glib-2.0/include
```

Moreover, libraries necessary for linking are specified using `LIBS`:

```pro
LIBS += `pkg-config --cflags --libs gstreamer-1.0 gstreamer-app-1.0 glib-2.0` \
        -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -lcurl \
        -ljsoncpp -lgpiod -lpoppler-qt5 -lssl -lcrypto -lzbar -lX11 \
        -lzip -lpthread -L/home/x_user/my_camera_project -lvosk \
        -lgstreamer-1.0 -lgstapp-1.0 -lgstvideo-1.0 -lhpdf -lpng -lz \
        -lQt5GLib-2.0 -lonnxruntime
```

### Compiler and Linker Flags

The project contains compiler warnings flags and sanitizer settings to enhance debugging and prevent warnings:

```pro
QMAKE_CXXFLAGS += -Wall -Wextra -Wno-deprecated-declarations -Wno-psabi -fsanitize=address -g
QMAKE_LFLAGS += -fsanitize=address

# Ensure GStreamer plugins are found
QMAKE_LFLAGS += -Wl,-rpath=/usr/lib/aarch64-linux-gnu/gstreamer-1.0
```

### Package Management

Several packages (e.g., GStreamer) are integrated using `PKGCONFIG` to facilitate library management:

```pro
PKGCONFIG += gstreamer-1.0 gstreamer-app-1.0 gstreamer-video-1.0
```

## Additional Notes

- Commented sections within the `.pro` file (e.g., for alternative compiler flags) suggest possible areas for optimization or testing. Uncommenting such lines can be used for specific configurations during the development phase.
  
- Performance optimizations for release builds may be added later by modifying `QMAKE_CXXFLAGS_RELEASE`.

This documentation should guide developers in understanding the configuration and dependencies of the `my_camera_project` while assisting in further development and debugging processes.

---
