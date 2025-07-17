# CameraReader Class Documentation

## Overview
The `CameraReader` class is designed to interface with camera devices, capturing video frames and optionally streaming them. It leverages OpenCV for image capturing and processing and GStreamer for handling various multimedia streams. The class provides a threaded mechanism for continuous frame capturing and includes functionality for snapshots and remote frame reading.

## Dependencies
The class depends on several external libraries:
- OpenCV for video capturing and processing.
- GStreamer for multimedia handling.
- Boost for a lock-free queue used for frame streaming.
- Qt framework (for timing and logging).

## Include Guards
```cpp
#ifndef CAMERAREADER_H
#define CAMERAREADER_H
```
This prevents multiple inclusions of the header file.

## Class Members

### Public Members
- **Constructor:** 
  ```cpp
  Camerareader(const std::string& _camera_pipeline)
  ```
  Initializes the camera pipeline and logs the construction of the class instance.

- **Destructor:**
  Releases resources by stopping the streaming thread and releasing the video capture object.

- **Init Function:**
  ```cpp
  int init();
  ```
  Opens the camera pipeline using OpenCV. Returns `0` on success or `-1` on failure.

- **Camera Pipeline Update:**
  ```cpp
  void update_camera_pipeline(const std::string& _camera_pipeline);
  ```
  Updates the camera pipeline string at runtime and logs the update.

- **Start and Stop Capturing:** 
  ```cpp
  void startCapturing(int _period);
  void stopCapturing();
  ```
  Starts and stops the frame capturing process at the specified interval.

- **Release Camera:**
  ```cpp
  void releasecamera();
  ```
  Releases the camera resource explicitly.

- **Stream Control:**
  ```cpp
  int startstream(std::string _stream_pipeline, int _fps, int _width, int _height);
  void stopstream();
  ```
  Starts and stops the video streaming process.

- **Remote Control:**
  ```cpp
  int startremote(std::string _remote_pipeline);
  void stopremote();
  ```
  Controls the initiation and termination of a remote video feed.

- **Frame Callback:**
  ```cpp
  void setFrameCallback(std::function<void(cv::Mat)> callback);
  ```
  Sets a callback function to process frames after they are captured.

- **Snapshot Function:**
  ```cpp
  bool takeSnapshotGst(const std::string& pipeline_desc);
  ```
  Captures a single snapshot based on a GStreamer pipeline description.

### Private Members
- **Member Variables:**
  - `std::string camera_pipeline;` - The GStreamer pipeline for camera access.
  - `Timer timer;` - Timer object for capturing frames at specified intervals.
  - `cv::VideoCapture cap;` - OpenCV object for video capturing from the camera.
  - `cv::VideoWriter scap;` - OpenCV object for writing streamed video to a file.
  - `cv::VideoCapture rcap;` - OpenCV object for capturing remote video stream.
  - `cv::Mat frame;` - Matrix object to hold a captured frame.
  - `std::function<void(cv::Mat)> Frame_callback;` - User-defined function for processing frames.
  - `bool stream, remote;` - Boolean flags indicating whether streaming or remote capture is active.

- **Frame Capture Mechanism:**
  ```cpp
  void CaptureFrame();
  ```
  Captures a frame from the camera and processes it (including color conversion if necessary). Also handles updating the frame stream if enabled.

- **Thread for Streaming:**
  - `std::thread streamThread;` - Thread for handling the streaming of frames.
  - `void startStreamingThread();` - Starts the dedicated thread for streaming.
  - `void stopStreamingThread();` - Stops the streaming thread when required.

- **Remote Frame Capture:**
  ```cpp
  void CaptureRemoteFrame();
  ```
  Reads and processes a frame from the remote video source.

### Error Handling
The class employs extensive error handling with logging, using the `LOG_ERROR` macro to document issues when opening streams or pipelines.

### Notes
- Memory management is crucial in this class, particularly with the custom queue for frames, where frames are dynamically allocated and must be properly deleted after use to avoid memory leaks.
- The class is designed with thread safety in mind, particularly with the usage of a lock-free queue to manage frame streaming without significant overhead.

### Example Usage
To use the `CameraReader`, instantiate the object with a camera pipeline string, initialize it, and start capturing frames like so:
```cpp
Camerareader cameraReader("v4l2src device=/dev/video0 ! videoconvert ! appsink");
if (cameraReader.init() == 0) {
    cameraReader.startCapturing(100); // Capture every 100 ms
}

// Set a callback to process each frame
cameraReader.setFrameCallback([](cv::Mat frame) {
    // Process frame...
});
```

Ensure to stop capturing and release the camera before destroying the object to clean up resources properly.

---
