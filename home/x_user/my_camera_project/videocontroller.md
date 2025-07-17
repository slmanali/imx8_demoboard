# Videocontroller Class Documentation

The `Videocontroller` class is designed to manage video playback using OpenCV for video frame handling and GStreamer for audio playback. It offers functionalities such as starting/stopping playback, updating the video path, controlling volume, and seeking through the video. 

## Header Guards
```cpp
#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H
```
This prevents multiple inclusions of the same header file, which can lead to compilation errors.

## Includes
```cpp
#include <opencv2/opencv.hpp>
#include <gst/gst.h>
#include "Logger.h"
#include "Timer.h"
```
- `<opencv2/opencv.hpp>`: Includes the OpenCV library for video handling.
- `<gst/gst.h>`: Includes GStreamer for audio playback functionalities.
- `Logger.h`: A custom logging utility for logging information or errors.
- `Timer.h`: A utility timer class for handling timed events.

## Class Definition
```cpp
class Videocontroller {
```
### Public Methods

#### Constructor
```cpp
Videocontroller(const std::string& _video_path)
```
Creates an instance of the `Videocontroller`. Initializes member variables and logs the constructor call.

- **Parameters**:
  - `_video_path`: A string that specifies the location of the video file.

#### Destructor
```cpp
~Videocontroller()
```
Cleans up resources when an instance of `Videocontroller` is destroyed. Stops video playback and releases the allocated video capture resource.

#### `update_video_path`
```cpp
void update_video_path(const std::string& _video_path)
```
Updates the `video_path` of the video file to be played.

- **Parameters**:
  - `_video_path`: New video file path.

#### `init`
```cpp
int init()
```
Initializes the video controller by opening a video file with OpenCV and setting up the GStreamer pipeline for audio playback.

- **Returns**: An integer indicating success (0) or failure (-1).
- **Exceptions**: Catches any standard exceptions and logs errors.

#### `startPlaying`
```cpp
void startPlaying()
```
Starts audio and video playback. Sets the pipeline state to playing and initializes the frame callback.

#### `setFrameCallback`
```cpp
void setFrameCallback(std::function<void(cv::Mat)> callback)
```
Sets a callback function that will be invoked with the current frame when it's available.

- **Parameters**:
  - `callback`: A function that takes a `cv::Mat` (frame) as an argument.

#### `stopPlaying`
```cpp
void stopPlaying()
```
Stops playback, sets the pipeline state to NULL, and resets the frame capture position.

#### `releasecamera`
```cpp
void releasecamera()
```
Releases the video capture object and stops any ongoing processes.

#### `playPause`
```cpp
void playPause()
```
Toggles playback between playing and paused states.

#### `seekForward`
```cpp
void seekForward(int _value)
```
Seek the video forward by the specified number of milliseconds.

- **Parameters**:
  - `_value`: The number of milliseconds to skip forward.

#### `seekBackward`
```cpp
void seekBackward(int _value)
```
Seek the video backward by the specified number of milliseconds.

- **Parameters**:
  - `_value`: The number of milliseconds to skip backward.

#### `volumeChanged`
```cpp
void volumeChanged(int _volume)
```
Changes the volume of the audio playback.

- **Parameters**:
  - `_volume`: The new volume value (0-100).

#### `pauseTimer`
```cpp
void pauseTimer()
```
Pauses the timer to prevent frame reading during seek operations.

#### `resumeTimer`
```cpp
void resumeTimer()
```
Resumes the timer if playback is not stopped or paused.

#### `getStop`
```cpp
bool getStop()
```
Returns whether the playback is stopped.

#### `getPause`
```cpp
bool getPause()
```
Returns whether the playback is paused.

#### `getVolume`
```cpp
int getVolume()
```
Returns the current volume level.

### Private Members

- `std::string video_path`: Stores the path to the video file.
- `bool isStop`: Indicates whether the playback is stopped.
- `bool isPause`: Indicates whether the playback is paused.
- `int volume`: Stores the current volume level.
- `GstElement *pipeline`: Holds the GStreamer pipeline instance.
- `GstElement *volumeElement`: Holds the volume element of the GStreamer pipeline.
- `Timer timer`: An instance for managing a callback timer.
- `cv::VideoCapture cap`: The OpenCV video capture object.
- `std::function<void(cv::Mat)> Frame_callback`: The callback to process frames.
- `void PlayFrame()`: Plays the next frame from the video and invokes the frame callback. This method is private and only called by the timer.

## Thread Safety
This class is not inherently thread-safe. Care should be taken to manage concurrent access to methods particularly when modifying playback state and seeking.

### Example Usage
```cpp
Videocontroller controller("video.mp4");
if (controller.init() == 0) {
    controller.setFrameCallback([](cv::Mat frame) {
        // Process each frame
    });
    controller.startPlaying();
}
```

This class provides a structured and flexible way to manage video playback, combining both video and audio functionalities with a comprehensive set of controls for playback management.

---
