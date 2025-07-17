# Audio.h - Detailed Documentation

## Overview
The `Audio.h` header file defines three classes: `AudioStreamer`, `AudioPlayer`, and `AudioControl`. These classes leverage the GStreamer library to manage audio streaming and control audio settings on a system running the ALSA sound driver. 

1. **AudioStreamer**: Manages the outgoing audio stream using a GStreamer pipeline.
2. **AudioPlayer**: Handles incoming audio streams, also through a GStreamer pipeline.
3. **AudioControl**: Provides functionality to control various audio settings on the system, such as volume and input/output types.

This file requires the GStreamer library and expects to be compiled with G++.

### Compilation
You can compile the code using the command:
```sh
g++ -o main main.cpp `pkg-config --cflags --libs gstreamer-1.0`
```

## Class Descriptions

### 1. Class: `AudioStreamer`
This class is responsible for creating and managing an audio output stream using a GStreamer pipeline.

#### Public Methods
- **`AudioStreamer(std::string _audio_outcoming_pipeline)`**
  - Constructor that initializes the audio pipeline string.
  
- **`void init()`**
  - Initializes GStreamer, creates the pipeline, and sets up signal handling for end-of-stream and error messages.

- **`void run()`**
  - Sets the GStreamer pipeline state to `PLAYING`, starting the audio output.

- **`void quit()`**
  - Stops the audio playback by setting the pipeline state to `NULL` and cleaning up.

- **`void update_pipline(std::string _audio_outcoming_pipeline)`**
  - Updates the audio output pipeline string.

- **`~AudioStreamer()`**
  - Destructor to clean up the GStreamer pipeline and bus resources.

#### Private Members
- **GstElement *pipeline**: Holds the GStreamer pipeline element.
- **GstBus *bus**: Used to receive messages from the pipeline.
- **std::string audio_outcoming_pipeline**: The GStreamer pipeline string.
- **bool _init**: Tracks if the object has been initialized.

#### Private Static Methods
- **`static void on_eos(GstBus *bus, GstMessage *msg, gpointer user_data)`**
  - Callback invoked when the end of the stream is reached; seeks back to the start of the audio.

- **`static void on_error(GstBus *bus, GstMessage *msg, gpointer user_data)`**
  - Callback to handle error messages from the GStreamer pipeline.

---

### 2. Class: `AudioPlayer`
This class is designed to manage audio playback through an incoming GStreamer pipeline.

#### Public Methods
- **`AudioPlayer(std::string _audio_incoming_pipeline)`**
  - Constructor that initializes the incoming audio pipeline string.

- **`void init()`**
  - Initializes the GStreamer library, creates the audio pipeline, and sets up signal handling similar to `AudioStreamer`.

- **`void run()`**
  - Sets the GStreamer pipeline state to `PLAYING` to begin audio playback.

- **`void quit()`**
  - Stops the audio playback by setting the pipeline state to `NULL`.

- **`void update_pipline(std::string _audio_incoming_pipeline)`**
  - Updates the audio input pipeline string.

- **`~AudioPlayer()`**
  - Destructor to clean up the GStreamer resources.

#### Private Members
- **GstElement *pipeline**: Holds the GStreamer pipeline element for playback.
- **GstBus *bus**: Used to handle messages from the pipeline.
- **std::string audio_incoming_pipeline**: The GStreamer pipeline string.
- **bool _init**: Indicates if the player has been initialized.

#### Private Static Methods
- **`static void on_eos(GstBus *bus, GstMessage *msg, gpointer user_data)`**
  - Callback for the end of the stream, performs similar actions to `AudioStreamer`.

- **`static void on_error(GstBus *bus, GstMessage *msg, gpointer user_data)`**
  - Handles error messages from the audio pipeline.

---

### 3. Class: `AudioControl`
This class provides methods to control various aspects of audio playback and capture on the ALSA sound system.

#### Public Methods
- **`void setVolumeLevel(int volume)`**
  - Sets the headphone volume level using the `amixer` command.

- **`int getVolumeLevel()`**
  - Retrieves the current headphone volume level.

- **`std::string getCaptureInputType()`**
  - Gets the current capture input type from the system.

- **`void setCaptureInputType(const std::string &capture_input_type)`**
  - Sets the specified capture input type.

- **`void setCaptureInputVolume(int volume)`**
  - Sets the volume for the capture input.

- **`int getCaptureInputVolume()`**
  - Retrieves the current volume level for the capture input.

- **`void setDigitalPlaybackVolume(int volume)`**
  - Sets the digital playback volume.

- **`int getDigitalPlaybackVolume()`**
  - Gets the current digital playback volume.

- **`void setDigitalCaptureVolume(int volume)`**
  - Sets the digital capture volume.

- **`int getDigitalCaptureVolume()`**
  - Retrieves the current digital capture volume.

- **`void setLineOutputVolume(int volume)`**
  - Sets the line output volume level.

- **`int getLineOutputVolume()`**
  - Retrieves the current line output volume level.

- **`void setDigitalPlaybackBoostVolume(int volume)`**
  - Sets the digital playback boost volume.

- **`int getDigitalPlaybackBoostVolume()`**
  - Retrieves the digital playback boost volume.

- **`void setDigitalSidetoneVolume(int volume)`**
  - Sets the digital sidetone volume.

- **`int getDigitalSidetoneVolume()`**
  - Retrieves the current digital sidetone volume.

#### Private Members
- **std::string executeCommand(const std::string &command)**: Executes a shell command and returns the output.

### Error Handling
The implementation includes robust error handling using exceptions. Errors encountered during initialization, state changes, or command executions are logged, and exceptions are thrown to signal failures to the caller.

### Logging
The code utilizes a `Logger` class (not defined in this file) to log various events, warnings, and errors, aiding in debugging and system monitoring.

## Summary
The `Audio.h` file encapsulates functionalities related to audio streaming, playback, and control, utilizing the GStreamer library and system commands to manage audio on platforms that use the ALSA subsystem. This organized structure facilitates easy integration and extension for audio-related applications.

---