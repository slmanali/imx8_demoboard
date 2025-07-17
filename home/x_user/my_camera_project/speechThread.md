# SpeechThread Class Documentation

The `speechThread` class provides a multi-threaded interface for real-time speech recognition using the Vosk API and GStreamer framework. It captures audio from a specified pipeline, processes the audio to recognize speech, and can invoke a callback function for recognized commands.

## Include Guard

```cpp
#ifndef SPEECHThREAD_H
#define SPEECHThREAD_H
// ...
#endif // SPEECHThREAD_H
```
This prevents multiple inclusions of the header file during compilation.

## Includes

The following libraries are included within the header file:
- `<iostream>`: For standard input/output stream.
- `<atomic>`: For atomic operations on the boolean variable `stop`.
- `<functional>`: To use `std::function` for callback handling.
- `<chrono>`: To manage time, specifically for timeout functionality.
- `<thread>`: For multi-threading capabilities.
- `<gst/gst.h>` and `<gst/app/gstappsink.h>`: GStreamer framework headers for audio processing.
- `"vosk_api.h"`: Vosk speech recognition API.
- `"Logger.h"`: Custom logger implementation for logging messages.
- `<jsoncpp/json/json.h>`: JSON handling library used for parsing recognized speech results.

## Class Definition

```cpp
class speechThread {
public:
    // Constructor and Destructor
    speechThread(std::string model_path, std::string grammar_json, std::string pipeline_description, int timeout_seconds = 3);
    ~speechThread(); 

    // Public Methods
    void setCommandCallback(std::function<void(const std::string&)> callback);
    void start();
    void stopThread();
    bool getstatus();

private:
    // Private Members
    std::atomic<bool> stop;
    std::string model_path, grammar_json, pipeline_description;
    int timeout_seconds;
    std::chrono::time_point<std::chrono::system_clock> last_audio_time;
    std::thread timeout_thread;

    GstElement *pipeline, *appsink;
    VoskModel *model;
    VoskRecognizer *rec;
    std::function<void(const std::string&)> command_callback;
    std::mutex cleanup_mutex;

    // Private Methods
    void initialize_vosk();
    void initialize_gstreamer();
    static GstFlowReturn on_new_sample(GstElement* sink, gpointer user_data);
    GstFlowReturn process_sample(GstElement* sink);
    void timeout_checker();
    void process_final_result();
    void force_final_result();
    void handle_result(const char* result);
    void cleanup();
};
```

### Public Methods

1. **Constructor**: 
    - **Parameters**:
        - `model_path`: Path to the Vosk model directory.
        - `grammar_json`: Path to the JSON grammar file for the recognizer.
        - `pipeline_description`: GStreamer pipeline description for capturing audio.
        - `timeout_seconds`: Optional timeout threshold in seconds (default is 3 seconds).
    - Initializes Vosk and GStreamer components.

2. **Destructor**:
    - Cleans up resources and stops any running threads.

3. **setCommandCallback**:
    - **Parameters**: `callback`: A function that takes a `std::string` parameter to handle recognized commands.
    - Sets the command callback function to be called with recognized text.

4. **start**:
    - Starts the speech recognition thread and GStreamer pipeline, setting its state to `GST_STATE_PLAYING`.

5. **stopThread**:
    - Stops the speech recognition thread and GStreamer pipeline, joining the timeout checker thread to ensure safe shutdown.

6. **getstatus**:
    - Returns the current status of the thread (`true` if stopped, otherwise `false`).

### Private Members

- `std::atomic<bool> stop`: Indicates if the speech thread is running or has been stopped.
- `std::string model_path, grammar_json, pipeline_description`: File paths necessary for initializing Vosk and GStreamer components.
- `int timeout_seconds`: Time duration after which a final recognition result will be forced if no audio is detected.
- `std::chrono::time_point<std::chrono::system_clock> last_audio_time`: Records the last time audio input was received.
- `std::thread timeout_thread`: Separate thread for monitoring audio timeouts.
- `GstElement *pipeline, *appsink`: GStreamer pipeline and appsink elements for audio processing.
- `VoskModel *model`: Pointer to the Vosk model object.
- `VoskRecognizer *rec`: Pointer to the Vosk recognizer object for speech recognition.
- `std::function<void(const std::string&)> command_callback`: Callback function for recognized speech.
- `std::mutex cleanup_mutex`: Mutex for thread-safe cleanup operations.

### Private Methods

1. **initialize_vosk**:
    - Initializes Vosk with the specified model and grammar, setting up the recognizer.

2. **initialize_gstreamer**:
    - Initializes GStreamer, parsing the provided pipeline description and setting up signal handlers.

3. **on_new_sample**:
    - Static callback function that is called when a new audio sample is available; delegates processing to `process_sample`.

4. **process_sample**:
    - **Parameters**: `GstElement* sink`: Pointer to the GStreamer element that received the sample.
    - Processes the audio sample, recognizes speech, and calls the final result handler when appropriate.

5. **timeout_checker**:
    - Runs in a separate thread, checking if a specified timeout has elapsed and acting accordingly by forcing a final recognition result.

6. **process_final_result**:
    - Handles the processing of the final recognition result from Vosk.

7. **force_final_result**:
    - Forces and processes the final result if the timeout has been reached.

8. **handle_result**:
    - Processes the JSON result received from the Vosk recognizer, validates response structures, and invokes the command callback when applicable.

9. **cleanup**:
    - Releases resources associated with the recognizer, model, and GStreamer pipeline ensuring thread safety through locking.

## Logging

The class uses a logging mechanism (presumably defined in `Logger.h`) to log information and errors at various points throughout the execution, helping to track the internal state and errors for debugging purposes.

## Exception Handling

All methods that may throw exceptions are wrapped in try-catch blocks. Errors are logged via the `LOG_ERROR` function, which provides useful information in case of failures in initializing or processing.

## Usage Example

Here is a brief example of how you might typically use the `speechThread` class:

```cpp
#include "speechThread.h"

// Callback to handle recognized speech commands
void commandHandler(const std::string& command) {
    std::cout << "Recognized command: " << command << std::endl;
}

int main() {
    // Create an instance of speechThread
    speechThread st("path/to/model", "path/to/grammar.json", "pipeline_description");

    // Set the command callback
    st.setCommandCallback(commandHandler);

    // Start the speech recognition
    st.start();

    // Main application loop or other functionality

    // Stop the speech recognition
    st.stopThread();

    return 0;
}
```

The example demonstrates how to initialize the `speechThread`, set up a callback for recognized commands, start the speech recognition, and gracefully shut down when done.

---
