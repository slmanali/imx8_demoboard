# Code Documentation: `vosk_gstreamer_example.cpp`

This document provides a comprehensive overview of the `vosk_gstreamer_example.cpp` file. The code is designed to work with the Vosk speech recognition API and GStreamer to create a voice recognition application. 

## Overview

The program initializes a GStreamer pipeline that captures audio input from a microphone. It processes the audio input using the Vosk speech recognition model to recognize voice commands. Recognized commands are displayed in uppercase output in the console. The application handles signals for graceful shutdown.

### Environment Requirements
- **C++ Compiler**: Support for C++11 or later
- **Libraries**: 
  - Vosk API
  - GStreamer
  - JSON for C++
- **Compiler Flags**: 
  ```bash
  g++ -o voice_app vosk_gstreamer_example.cpp $(pkg-config --cflags --libs gstreamer-1.0 gstreamer-app-1.0) -lpthread -ljsoncpp -lvosk
  ```

### Included Headers
- `<iostream>`: For input and output streams.
- `<csignal>`: For handling signals.
- `<thread>`: For threading capabilities.
- `<chrono>`: For timing utilities.
- `<codecvt>` & `<locale>`: For string conversion between UTF-8 and wide strings.
- `"speechThread.h"`: Custom header that likely defines the `speechThread` class, which handles speech recognition operations.

## Global Variables
- `std::unique_ptr<speechThread> voiceThread;`: A smart pointer to manage the lifecycle of the speechThread instance used for voice recognition.

## Functions

### 1. `toUpperCase`
```cpp
std::string toUpperCase(const std::string& input);
```
**Description**: Converts a given string to uppercase using wide character conversion.

- **Parameters**:
  - `input`: The string to be converted to uppercase.
  
- **Returns**: A new string that is the uppercase version of the input.

### 2. `signalHandler`
```cpp
void signalHandler(int signum);
```
**Description**: Handles interrupt signals (e.g., SIGINT) to stop the voice recognition thread gracefully.

- **Parameters**:
  - `signum`: The signal number received.
  
### 3. `onCommandRecognized`
```cpp
void onCommandRecognized(const std::string& command);
```
**Description**: Callback function that is called when a voice command is recognized.

- **Parameters**:
  - `command`: The string command recognized from the audio input.

### 4. `onTalkSignal`
```cpp
void onTalkSignal(int status);
```
**Description**: Callback function to indicate the status of voice activity.

- **Parameters**:
  - `status`: An integer indicating the status (0 for listening, 1 for processing voice input).

### 5. `main`
```cpp
int main();
```
**Description**: The entry point of the program. It initializes the Vosk model and GStreamer pipeline and starts the recognition process.

- **Try-Catch Block**:
  - Initializes the voice recognition model using a specified grammar.
  - Sets up the GStreamer pipeline for audio capture and playback.
  - Starts the `voiceThread` for processing audio input and recognizing commands.

- **Infinite Loop**: The main loop keeps the program running while the voice recognition is active. It checks every 30 seconds to keep the thread alive.

## Error Handling
The application handles initialization errors for the Vosk model and GStreamer pipeline. If any errors occur, a message is printed to the standard error output, and the program exits with a failure status.

## GStreamer Pipeline Description
The audio pipeline is defined as follows:
```cpp
const std::string pipeline_description = 
    "pulsesrc device=alsa_input.platform-sound-wm8904.stereo-fallback ! audioconvert ! audioresample ! "
    "audio/x-raw,format=S16LE,rate=16000,channels=1 ! tee name=splitter "
    "splitter. ! queue ! appsink name=myappsink "
    "splitter. ! queue ! audioconvert ! audioresample ! "
    "audio/x-raw,format=S16LE,rate=44100,channels=2 ! pulsesink device=alsa_output.platform-sound-wm8904.stereo-fallback";
```
This pipeline captures audio from the specified ALSA device, converts and resamples the audio, splits the signal for speech recognition and playback.

## Conclusion
The `vosk_gstreamer_example.cpp` file demonstrates how to integrate the Vosk speech recognition API with GStreamer to create an interactive voice command system. This documentation can serve as a guide for understanding the code structure and functionality for future development or modifications.

---