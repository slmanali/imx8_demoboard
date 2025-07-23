#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

#include "/home/x_user/my_camera_project/speechThread.h"
#include <codecvt>
#include <locale>
// Global pointer to handle signal interruption
std::unique_ptr<speechThread> voiceThread;

std::string toUpperCase(const std::string& input) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_str = converter.from_bytes(input);
    
    for (wchar_t& ch : wide_str) {
        ch = std::towupper(ch);
    }
    
    return converter.to_bytes(wide_str);
}
// Signal handler for graceful shutdown
void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received. Stopping VoiceThread...\n";
    if (voiceThread) {
        voiceThread->stopThread();
    }
    exit(signum);
}

// Callback function for recognized commands
void onCommandRecognized(const std::string& command) {
    std::cout << "Recognized Command: " << toUpperCase(command) << std::endl;
}

// Callback function for voice activity
void onTalkSignal(int status) {
    if (status == 0) {
        std::cout << "Listening..." << std::endl;
    } else if (status == 1) {
        std::cout << "Processing voice input..." << std::endl;
    }
}

int main() {
    try {
        // Define grammar for valid commands
        const std::string grammar = R"([
            "закрыть", "первый", "второй", "третий",
            "увеличить", "уменьшить", "дальше", "назад",
            "вверх", "опустить", "вправо", "влево",
            "задачи", "документ", "завершить",
            "один", "два", "три", "четыре"
        ])";

        const std::string pipeline_description = "pulsesrc device=alsa_input.platform-sound-wm8904.stereo-fallback ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,rate=16000,channels=1 ! tee name=splitter splitter. ! queue ! appsink name=myappsink splitter. ! queue ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,rate=44100,channels=2 ! pulsesink device=alsa_output.platform-sound-wm8904.stereo-fallback";
        signal(SIGINT, signalHandler);

        voiceThread = std::make_unique<speechThread>(
            "/home/x_user/my_camera_project/vosk-model-small-ru-0.22",
            grammar,  // Pass grammar to constructor
            pipeline_description,
            5  // Timeout in seconds
        );

        voiceThread->setCommandCallback(onCommandRecognized);
        voiceThread->start();

        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error initializing VoiceThread: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
// g++ -o voice_app vosk_gstreamer_example.cpp $(pkg-config --cflags --libs gstreamer-1.0 gstreamer-app-1.0) -lpthread -ljsoncpp -lvosk
// #include <iostream>
// #include <gst/gst.h>
// #include <gst/app/gstappsink.h>
// #include "/home/x_user/my_camera_project/vosk_api.h"
// #include <cstring>
// #include <jsoncpp/json/json.h>

// class SpeechRecognizer {
// public:
//     SpeechRecognizer(const std::string &model_path) {
//         gst_init(nullptr, nullptr); // Initialize GStreamer

//         // Initialize Vosk model and recognizer
//         model = vosk_model_new(model_path.c_str());
//         if (!model) {
//             throw std::runtime_error("Failed to load Vosk model");
//         }

//         recognizer = vosk_recognizer_new(model, 16000.0); // Set sample rate to 16kHz
//         if (!recognizer) {
//             throw std::runtime_error("Failed to initialize Vosk recognizer");
//         }

//         // GStreamer pipeline for microphone input
//         std::string pipeline_description =
//             "autoaudiosrc ! audioconvert ! audioresample ! "
//             "audio/x-raw,format=S16LE,rate=16000,channels=1 ! "
//             "tee name=splitter "
//             "splitter. ! queue ! appsink name=myappsink "  // For speech recognition
//             "splitter. ! queue ! audioconvert ! audioresample ! "
//             "audio/x-raw,format=S16LE,rate=44100,channels=2 ! pulsesink device=alsa_output.platform-sound-wm8904.stereo-fallback";  // For audio playback
            
//         pipeline = gst_parse_launch(pipeline_description.c_str(), nullptr);
//         if (!pipeline) {
//             throw std::runtime_error("Failed to create GStreamer pipeline");
//         }

//         appsink = gst_bin_get_by_name(GST_BIN(pipeline), "myappsink");
//         if (!appsink) {
//             throw std::runtime_error("Failed to get appsink element");
//         }
//         gst_app_sink_set_emit_signals(GST_APP_SINK(appsink), true);
//         g_signal_connect(appsink, "new-sample", G_CALLBACK(on_new_sample), this);
//     }

//     ~SpeechRecognizer() {
//         vosk_recognizer_free(recognizer);
//         vosk_model_free(model);
//         if (appsink) gst_object_unref(appsink);
//         if (pipeline) gst_object_unref(pipeline);
//     }

//     void start() {
//         // Start GStreamer pipeline
//         gst_element_set_state(pipeline, GST_STATE_PLAYING);

//         // Main loop for GStreamer
//         GMainLoop *main_loop = g_main_loop_new(nullptr, false);
//         g_main_loop_run(main_loop);
//     }

// private:
//     static GstFlowReturn on_new_sample(GstAppSink *appsink, gpointer user_data) {
//         auto *self = static_cast<SpeechRecognizer *>(user_data);
//         return self->process_new_sample(appsink);
//     }

//     GstFlowReturn process_new_sample(GstAppSink *appsink) {
//         GstSample *sample = gst_app_sink_pull_sample(appsink);
//         if (!sample) return GST_FLOW_ERROR;

//         GstBuffer *buffer = gst_sample_get_buffer(sample);
//         GstMapInfo map;
//         if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
//             // Pass the audio data to the Vosk recognizer
//             if (vosk_recognizer_accept_waveform(recognizer, reinterpret_cast<const char *>(map.data), map.size)) {
//                 const char *result = vosk_recognizer_result(recognizer);
//                 process_result(result);
//             } else {
//                 const char *partial_result = vosk_recognizer_partial_result(recognizer);
//                 std::cout << "Partial result: " << partial_result << std::endl;
//             }
//             gst_buffer_unmap(buffer, &map);
//         }

//         gst_sample_unref(sample);
//         return GST_FLOW_OK;
//     }

//     void process_result(const char *result) {
//         // Parse the JSON result
//         Json::Reader reader;
//         Json::Value root;
//         if (reader.parse(result, root)) {
//             std::string recognized_text = root["text"].asString();
//             std::cout << "Recognized text: " << recognized_text << std::endl;
//         } else {
//             std::cerr << "Failed to parse result JSON: " << result << std::endl;
//         }
//     }

//     VoskModel *model;
//     VoskRecognizer *recognizer;
//     GstElement *pipeline;
//     GstElement *appsink;
// };

// int main() {
//     try {
//         // Path to Vosk model directory
//         std::string model_path = "/home/x_user/my_camera_project/vosk-model-small-ru-0.22";

//         // Create and start the recognizer
//         SpeechRecognizer recognizer(model_path);
//         recognizer.start();
//     } catch (const std::exception &e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }
