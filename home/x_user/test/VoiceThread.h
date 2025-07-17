#ifndef VOICETHREAD_H
#define VOICETHREAD_H

#include <iostream>
#include <atomic>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <vosk_api.h>
#include <jsoncpp/json/json.h>
#include <mutex>
#include <condition_variable>
#include <map>
#include "Configuration.h"
#include "Logger.h"
#include "Timer.h"  // Include the Timer class

class VoiceThread {
public:
    VoiceThread(std::string _vosk_model, std::string pipeline_description) 
        : stop(false), vosk_model(_vosk_model), 
        pipeline_description(pipeline_description) {
        try {
            vosk_gpu_init(); 
            // Initialize GStreamer
            gst_init(nullptr, nullptr);
            LOG_INFO("VoiceThread Constructor");     
            // Initialize Vosk Model
            model = vosk_model_new(vosk_model.c_str());
            if (!model) throw std::runtime_error("Failed to load Vosk model");
            // Define command grammar (JSON array of valid commands)
            const std::string grammar = R"([
                "закрыть", "первый", "второй", "третий",
                "увеличить", "уменьшить", "дальше", "назад",
                "вверх", "опустить", "вправо", "влево",
                "задачи", "документ", "завершить",
                "один", "два", "три", "четыре"
            ])";

            rec = vosk_recognizer_new_grm(model, 16000.0, grammar.c_str());
            if (!rec) throw std::runtime_error("Failed to create Vosk recognizer");
            // Enable confidence scores and partial results
            vosk_recognizer_set_words(rec, 1);
            vosk_recognizer_set_partial_words(rec, 0);

            // Set up GStreamer pipelines
            pipeline = gst_parse_launch(pipeline_description.c_str(), nullptr);

            // Get appsink and connect to the callback
            appsink = gst_bin_get_by_name(GST_BIN(pipeline), "myappsink");
            g_object_set(appsink, "emit-signals", TRUE, NULL);
            g_signal_connect(appsink, "new-sample", G_CALLBACK(onNewSample), this);

        } catch (const std::exception& e) {
            LOG_ERROR("Something went wrong in VoiceThread Constructor: " + std::string(e.what()));    
        }
    }

    ~VoiceThread() {
        try {
            stop = true;
            LOG_INFO("VoiceThread destructor");  
            timer.stop();  // Stop the timer
            if (pipeline) {
                gst_element_set_state(pipeline, GST_STATE_NULL);
                gst_object_unref(pipeline);
            }
            vosk_recognizer_free(rec);
            vosk_model_free(model);
        } catch (const std::exception& e) {
            LOG_ERROR("Something went wrong in VoiceThread Destructor: " + std::string(e.what()));   
        }
    }

    void start() {
        try {
            stop = false;
            gst_element_set_state(pipeline, GST_STATE_PLAYING);
            // Start the timer to process audio every 4 seconds
            timer.start(4000, 0, [this]() {
                if (!stop) {
                    processAudioChunk();
                }
            });
        } catch (const std::exception& e) {
            LOG_ERROR("Something went wrong while start VoiceThread: " + std::string(e.what()));   
        }
    }

    void stopThread() {
        stop = true;
        timer.stop();  // Stop the 
        gst_element_set_state(pipeline, GST_STATE_NULL);
    }

    void setCommandRecognizeCallback(std::function<void(const std::string &)> callback) {
        commandRecognizeCallback = callback;
    }

    void setTalkSignCallback(std::function<void(int)> callback) {
        talkSignCallback = callback;
    }

private:
    std::atomic<bool> stop;    
    std::string vosk_model;
    std::string pipeline_description;
    Timer timer;  // Use Timer instead of std::thread
    std::function<void(const std::string &)> commandRecognizeCallback;
    std::function<void(int)> talkSignCallback;

    GstElement *pipeline;
    GstElement *appsink;
    VoskModel *model;
    VoskRecognizer *rec;
    std::vector<char> audioBuffer;
    std::map<std::string, std::string> commandMap = {
        {"ЗАКРЫТЬ", "ЗАКРЫТЬ"},
        {"ПЕРВЫЙ", "ПЕРВЫЙ"},
        {"ВТОРОЙ", "ВТОРОЙ"},
        {"ТРЕТИЙ", "ТРЕТИЙ"},
        {"УВЕЛИЧИТЬ", "УВЕЛИЧИТЬ"},
        {"УМЕНЬШИТЬ", "УМЕНЬШИТЬ"},
        {"ДАЛЬШЕ", "ДАЛЬШЕ"},
        {"НАЗАД", "НАЗАД"},
        {"ВВЕРХ", "ВВЕРХ"},
        {"ОПУСТИТЬ", "ОПУСТИТЬ"},
        {"ВПРАВО", "ВПРАВО"},
        {"ВЛЕВО", "ВЛЕВО"},
        {"ЗАДАЧИ", "ЗАДАЧИ"},
        {"ДОКУМЕНТ", "ДОКУМЕНТ"},
        {"ЗАВЕРШИТЬ", "ЗАВЕРШИТЬ"},
        {"ОДИН", "ОДИН"},
        {"ДВА", "ДВА"},
        {"ТРИ", "ТРИ"},
        {"ЧЕТЫРЕ", "ЧЕТЫРЕ"}};

    
        static GstFlowReturn onNewSample(GstElement *sink, gpointer user_data) {
        VoiceThread *self = static_cast<VoiceThread *>(user_data);
        return self->processNewSample(sink);
    }

    GstFlowReturn processNewSample(GstElement *sink) {
        GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
        if (!sample) return GST_FLOW_ERROR;

        GstBuffer *buffer = gst_sample_get_buffer(sample);
        GstMapInfo map;
        gst_buffer_map(buffer, &map, GST_MAP_READ);

        audioBuffer.insert(audioBuffer.end(), map.data, map.data + map.size);

        gst_buffer_unmap(buffer, &map);
        gst_sample_unref(sample);

        return GST_FLOW_OK;
    }

    void processAudioChunk() {
        try {
            // Process in chunks of 4 seconds
            int fourSecondSize = 16000 * 2 * 4; // 4 seconds of audio
            if (audioBuffer.size() >= static_cast<size_t>(fourSecondSize)) {
                std::vector<char> chunk(audioBuffer.begin(), audioBuffer.begin() + fourSecondSize);
                audioBuffer.erase(audioBuffer.begin(), audioBuffer.begin() + fourSecondSize);

                // Push audio to playback pipeline
                GstBuffer *playbackBuffer = gst_buffer_new_allocate(nullptr, chunk.size(), nullptr);
                gst_buffer_fill(playbackBuffer, 0, chunk.data(), chunk.size());
                gst_app_src_push_buffer(GST_APP_SRC(appsrc), playbackBuffer);

                if (talkSignCallback) {
                    talkSignCallback(0);
                }

                // Perform Vosk recognition
                if (vosk_recognizer_accept_waveform(rec, chunk.data(), chunk.size())) {
                    const char *resultJson = vosk_recognizer_result(rec);
                    Json::Reader reader;
                    Json::Value root;
                    if (reader.parse(resultJson, root)) {
                        std::string recognizedText = root["text"].asString();
                        if (!recognizedText.empty()) {
                            LOG_INFO("Recognized text: " + recognizedText);  
                            matchCommand(recognizedText);                        
                        }
                    }
                }
            }

            if (talkSignCallback) {
                talkSignCallback(1);
            }
        } catch (const std::exception& e) {
            LOG_ERROR("Something went wrong while processAudioChunk VoiceThread: " + std::string(e.what()));   
        }
    }

    // Helper function to calculate Levenshtein distance
    int levenshteinDistance(const std::string &s1, const std::string &s2) {
        size_t len1 = s1.size();
        size_t len2 = s2.size();
        std::vector<std::vector<int>> dp(len1 + 1, std::vector<int>(len2 + 1));

        for (size_t i = 0; i <= len1; ++i) {
            for (size_t j = 0; j <= len2; ++j) {
                if (i == 0)
                    dp[i][j] = j; // Deletion cost
                else if (j == 0)
                    dp[i][j] = i; // Insertion cost
                else if (s1[i - 1] == s2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1]; // Characters match
                else
                    dp[i][j] = 1 + std::min({dp[i - 1][j],    // Deletion
                                            dp[i][j - 1],    // Insertion
                                            dp[i - 1][j - 1] // Substitution
                    });
            }
        }
        return dp[len1][len2];
    }

    // Helper function to calculate similarity (1 - normalized Levenshtein distance)
    double calculateSimilarity(const std::string &s1, const std::string &s2) {
        int maxLength = std::max(s1.size(), s2.size());
        if (maxLength == 0) return 1.0; // Both strings are empty, consider them identical
        return 1.0 - static_cast<double>(levenshteinDistance(s1, s2)) / maxLength;
    }

    // Function to match recognized text against a command map
    void matchCommand(const std::string &recognizedText) {
        std::string bestMatch;
        double highestSimilarity = 0.0;

        // Split recognized text into words (using space as the delimiter)
        std::istringstream iss(recognizedText);
        std::vector<std::string> words{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

        // Iterate through each word and find the closest command in the map
        for (const std::string &word : words) {
            for (const auto &[command, response] : commandMap) {
                double similarity = calculateSimilarity(word, command);
                if (similarity > highestSimilarity) {
                    highestSimilarity = similarity;
                    bestMatch = response;
                }
            }
        }

        if (!bestMatch.empty()) {
            LOG_INFO("Recognized text: " + recognizedText);  
            LOG_INFO("Recognized text: " + bestMatch);  
            if (commandRecognizeCallback) {
                commandRecognizeCallback(recognizedText);
            }
        }
    }
};

#endif // VOICETHREAD_H

