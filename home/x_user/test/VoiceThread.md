# VoiceThread Class Documentation

## Overview

The `VoiceThread` class is designed to handle voice recognition and audio processing using Vosk and GStreamer. It initializes the Vosk model for speech recognition, sets up a GStreamer pipeline for audio processing, and processes audio samples in real-time to recognize spoken commands. The class utilizes modern C++ features such as atomic variables for thread safety, function callbacks for event handling, and leverages a custom `Timer` class to manage audio processing intervals.

## Include Guards 

```cpp
#ifndef VOICETHREAD_H
#define VOICETHREAD_H
```
This section prevents multiple inclusions of `VoiceThread.h` during compilation.

## Dependencies

This class requires several libraries, indicated by the `#include` directives:

- `<iostream>`: For standard input-output stream operations.
- `<atomic>`: To ensure safe access to shared variables across threads.
- `<functional>`: For using function callbacks.
- `<vector>`, `<map>`, `<string>`, `<sstream>`, `<algorithm>`: Standard containers and utilities.
- `<gst/gst.h>` and `<gst/app/gstappsink.h>`: For GStreamer functionalities related to multimedia processing.
- `<vosk_api.h>`: For interfacing with the Vosk speech recognition API.
- `<jsoncpp/json/json.h>`: For parsing JSON responses.
- `<mutex>` and `<condition_variable>`: For synchronization in multithreaded environments (although they appear unused in the provided code).
- `"Configuration.h"`, `"Logger.h"`, `"Timer.h"`: Custom headers likely providing additional functionality such as configuration management, logging, and timing utilities.

## Class Definition

```cpp
class VoiceThread {
```
The `VoiceThread` class manages the lifecycle of the voice recognition process and encapsulates the necessary operations.

### Public Member Functions

- **Constructor**
  ```cpp
  VoiceThread(std::string _vosk_model, std::string pipeline_description);
  ```
  Initializes a `VoiceThread` object, setting up the Vosk model, GStreamer pipeline, and the command recognizer. Throws exceptions if initialization fails.

- **Destructor**
  ```cpp
  ~VoiceThread();
  ```
  Cleans up resources by stopping the pipeline and freeing Vosk model and recognizer instances.

- **start()**
  ```cpp
  void start();
  ```
  Starts the audio processing by setting the GStreamer pipeline to playing state and begins timed audio processing.

- **stopThread()**
  ```cpp
  void stopThread();
  ```
  Stops the voice processing thread and sets the pipeline to an idle state.

- **setCommandRecognizeCallback(std::function<void(const std::string &)> callback)**
  ```cpp
  void setCommandRecognizeCallback(std::function<void(const std::string &)> callback);
  ```
  Sets a callback function to be invoked when a command is recognized.

- **setTalkSignCallback(std::function<void(int)> callback)**
  ```cpp
  void setTalkSignCallback(std::function<void(int)> callback);
  ```
  Sets a callback function to signal the processing status during audio handling.

### Private Member Variables

- **Atomic Flags**
  ```cpp
  std::atomic<bool> stop;
  ```
  A flag that indicates whether the thread should stop processing audio.

- **Model and Pipeline Details**
  ```cpp
  std::string vosk_model;
  std::string pipeline_description;
  GstElement *pipeline;
  GstElement *appsink;
  VoskModel *model;
  VoskRecognizer *rec;
  std::vector<char> audioBuffer;
  ```
  Variables to store the Vosk model path, GStreamer pipeline description, references to the GStreamer pipeline and app sink, Vosk model, recognizer, and a buffer for audio data.

### Command Mapping

```cpp
std::map<std::string, std::string> commandMap;
```
This map defines the recognized commands and their responses. It serves the purpose of matching recognized speech to predefined commands, supporting operations such as "close", "increase", "back", etc.

### Private Member Functions

- **static GstFlowReturn onNewSample(GstElement *sink, gpointer user_data)**
  ```cpp
  static GstFlowReturn onNewSample(GstElement *sink, gpointer user_data);
  ```
  A static callback function that processes new audio samples arriving at the GStreamer app sink. It passes control to the member function `processNewSample()`.

- **GstFlowReturn processNewSample(GstElement *sink)**
  ```cpp
  GstFlowReturn processNewSample(GstElement *sink);
  ```
  Pulls a new sample from the GStreamer app sink and stores the audio data into `audioBuffer`.

- **void processAudioChunk()**
  ```cpp
  void processAudioChunk();
  ```
  Processes the buffered audio data in chunks, performing voice recognition and handling recognized commands.

- **int levenshteinDistance(const std::string &s1, const std::string &s2)**
  ```cpp
  int levenshteinDistance(const std::string &s1, const std::string &s2);
  ```
  Computes the Levenshtein distance between two strings, useful for measuring the similarity of recognized words to command words.

- **double calculateSimilarity(const std::string &s1, const std::string &s2)**
  ```cpp
  double calculateSimilarity(const std::string &s1, const std::string &s2);
  ```
  Calculates a similarity score between two strings based on their Levenshtein distance.

- **void matchCommand(const std::string &recognizedText)**
  ```cpp
  void matchCommand(const std::string &recognizedText);
  ```
  Attempts to match recognized speech to the defined commands in `commandMap` by determining the best matches based on similarity scores.

## Exception Handling

Throughout the class methods, exceptions are caught and logged using the `LOG_ERROR` function to help trace issues during initialization, processing, and destruction.

## Conclusion

The `VoiceThread` class is a comprehensive solution for handling audio input and command recognition in a multithreaded environment using Vosk and GStreamer. It is particularly useful for applications that require real-time voice commands for interaction.

---