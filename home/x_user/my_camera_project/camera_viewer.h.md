# Camera Viewer Class Documentation

## Overview

The `CameraViewer` class is a part of a larger application that utilizes a graphical user interface (GUI) for interacting with camera functionality, video streaming, audio management, and various other features such as QR code processing and file display. This class is designed using the Qt framework and interacts with technologies like OpenCV for image processing, and GStreamer for streaming media. Additionally, it manages various states and modes, including low power modes and network configurations.

## Header File: `camera_viewer.h`

### Includes

The header includes multiple standard and external libraries:

- `opencv2/opencv.hpp`: OpenCV library for image processing.
- `Q*` classes: Qt framework classes for GUI construction and event management.
- `gst/gst.h`: GStreamer library for handling multimedia streaming.
- `zbar.h`: Library for decoding QR codes.
- Additional utility libraries for data handling, strings, threading, and encryption.

### Class Declaration

```cpp
class CameraViewer : public QWidget {
    Q_OBJECT
```

`CameraViewer` inherits from `QWidget`, making it part of Qt's widget-based interface.

### Public Methods

The following are public methods provided by the `CameraViewer` class:

- **Constructor:**
    ```cpp
    CameraViewer(QWidget *parent = nullptr);
    ```
    Constructs a `CameraViewer` object.

- **Tab Creation Methods:**
    - `createFirstTab()`
    - `createNavigationWidget()`
    - `createContentWidget()`
    - `createVideoTab()`
    - `createAudioControlTab()`
    
  These methods create respective tabs in the viewer.

- **UI Interaction Methods:**
    - `button_pressed()`
    - `setVisors(std::string _value)`
    - `send_audio_settings()`
    - `AudioReset()`

- **Data Handling Methods:**
    - `readwifijson()`
    - `loadTasks(const std::string &filename)`
    - `showFilesList(const std::string &folder_path, const std::string &suffix)`
  
- **File Handling Methods:**
    - `LoadPDF(const std::string &filepath)`
    - `LoadMP4(const std::string &filepath)`
    - `loadTXT(const std::string &filePath)`

- **QR Code Processing:**
    - `start_qrcode()`
    - `stop_qrcode()`
    - `processQRCode(cv::Mat _frame)`

- **State and Mode Management:**
    - `working_mode()`
    - `FSM(nlohmann::json _data, std::string _event)`
    - `remotestart()`
    - `remoteend()`
    - `Enter_Low_Power_Mode()`
    - `Exit_Low_Power_Mode()`

- **Audio Stream Management:**
    - `start_audio_channel()`
    - `close_audio_channel()`

- **Display Methods:**
    - `display_tasks()`
    - `drawTaskList()`
  
- **Utility Methods:**
    - `toUpperCase(const std::string& input)`
    - `getCurrentDateTime()`
    - `base64_decode_openssl(const std::string &encoded)`
    - `aes_decrypt_ecb(const std::string &cipherText, const std::string &key)`
  
- **User Interaction Handling:**
    - `handle_command_recognize(std::string _command)`
    - `changeLanguage(std::string _lang)`

### Private Slots

Private slots are signals used for event handling in the class:

- `Enter_Low_Power_Mode()`
- `Exit_Low_Power_Mode()`
- `report_and_reset_clicks()`
- `finish_helping()`
- `checkwifi()`
- `handleIMUClassification(const QString& label)`

### Private Member Variables

This section details private member variables that manage the state, data, and user interface components.

- **Graphical Components:**
    - `QGraphicsScene *videoScene, *videoScene1;`, `QGraphicsView *videoView, *videoView1;`, etc., to handle video display.
  
- **UI Labels and Controls:**
    - Multiple `QLabel` and `QListWidget` objects to show information and feedback to the user.

- **Threads and Audio Management:**
    - `std::unique_ptr<speechThread> voiceThread;`, `AudioControl A_control;`, etc., to manage voice processing and audio streams.

- **Configuration and Management Classes:**
    - `Configuration config;`, `LanguageManager lang;`, `WiFiManager network;`, and others to manage various settings and stateful operations.

- **Data Structures:**
    - `std::vector<std::string> tasks;`, `std::map<std::string, std::vector<std::map<std::string, std::string>>> procedure;` for organizing and managing tasks and procedures.

### Destructor

```cpp
~CameraViewer();
```

Proper cleanup of allocated resources when the `CameraViewer` instance is destroyed.

### Notes

- The `CameraViewer` class is integral for managing both the visual output from camera feeds as well as interactive elements for user input.
- It encompasses a substantial number of features related to multimedia processing, along with various user controls for video and audio management.
- The use of modern C++ practices such as smart pointers (`std::unique_ptr`) helps manage resources efficiently and mitigate memory leaks.

## Conclusion

The `CameraViewer` class is a feature-rich component designed for the management of multimedia streams and user interactions within a GUI context. It leverages various library integrations to offer functionality across video processing, audio control, data management, and UI rendering. This documentation provides an overview of its structure and capabilities, which can be beneficial for developers looking to understand or extend the functionality of the class.

---
