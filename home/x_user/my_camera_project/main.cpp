// #include <QApplication>
// #include <QSurfaceFormat>
// #include <cstdlib>  // For setenv
// #include "app.h"

// int main(int argc, char *argv[]) {

//     // Set the environment variable for Qt's QPA platform
//     setenv("QT_QPA_PLATFORM", "wayland", 1);  // 1 means to overwrite if the variable is already set
//     // setenv("QT_OPENGL", "es", 1);  // 1 means to overwrite if the variable is already set
//     QSurfaceFormat format;
//     format.setRenderableType(QSurfaceFormat::OpenGLES);
//     QSurfaceFormat::setDefaultFormat(format);
//     QApplication app(argc, argv);
//     LOG_INFO("start Application");
//     App w;
//     w.showFullScreen();

//     return app.exec();
// }

// main helmet start
#include <QApplication>
#include <csignal>
// #undef Status  // Undefine X11's Status macro to avoid conflict with OpenCV
#include "camera_viewer.h"
#include <QSurfaceFormat>
#include <cstdlib>  // For setenvy
#include <gst/gst.h>
void setEnvIfDifferent(const char* var, const char* desired_value) {
    const char* current_value = getenv(var);
    if (!current_value || strcmp(current_value, desired_value) != 0) {
        setenv(var, desired_value, 1);
        std::cout << "Set " << var << " to " << desired_value << std::endl;  // Optional debug output
    } else {
        std::cout << var << " is already set to " << desired_value << std::endl;  // Optional debug output
    }
}

volatile std::sig_atomic_t gSignalStatus = 0;

void signal_handler(int signal) {
  gSignalStatus = signal;
  qApp->quit();  // Terminate the Qt event loop
}

int main(int argc, char *argv[]) {

    try {
        // std::signal(SIGINT, signal_handler);  // Handle Ctrl+C
        gst_init(&argc, &argv);
        freopen("/home/x_user/my_camera_project/Outputs.log", "a", stdout);
        freopen("/home/x_user/my_camera_project/Errors.log", "a", stderr);
        // Set the environment variable for Qt's QPA platform
        setEnvIfDifferent("QT_LOGGING_RULES", "*.debug=false;qt.qpa.*=false");
        setEnvIfDifferent("GST_PLUGIN_PATH", "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/");
        // setEnvIfDifferent("LD_LIBRARY_PATH", "/usr/lib/aarch64-linux-gnu/");
        setEnvIfDifferent("XDG_RUNTIME_DIR", "/run/user/0");
        setEnvIfDifferent("WAYLAND_DISPLAY", "wayland-1");
        setEnvIfDifferent("QT_QPA_PLATFORM", "wayland");
        // setEnvIfDifferent("MESA_LOADER_DRIVER_OVERRIDE", "vivante");
        // setEnvIfDifferent("EGL_PLATFORM", "wayland");
        setEnvIfDifferent("DISPLAY", ":0");
        setEnvIfDifferent("GST_DEBUG", "3");
        setEnvIfDifferent("GST_PLUGIN_FEATURE_RANK", "vaapih264dec:256");
        // setEnvIfDifferent("GST_GL_PLATFORM", "egl");   
        // setEnvIfDifferent("LANG", "ru_RU.UTF-8");   
        // setEnvIfDifferent("LC_ALL", "ru_RU.UTF-8");   
        // setenv("QT_QPA_PLATFORM", "xcb", 1); 
        // setenv("QT_QPA_PLATFORM", "wayland", 1);  // 1 means to overwrite if the variable is already set
        // setenv("WAYLAND_DISPLAY", "/run/wayland-0", 1);
        // setenv("XDG_RUNTIME_DIR", "/run/user/0", 1);
        // setenv("QT_XCB_GL_INTEGRATION", "xcb_egl", 1);  // Force EGL
        // setenv("GST_DEBUG", "3", 1);  // 1 means to overwrite if the variable is already set
        QSurfaceFormat format;
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setProfile(QSurfaceFormat::CoreProfile);
        QSurfaceFormat::setDefaultFormat(format);
        QApplication app(argc, argv);   
        LOG_INFO("=========================================================================================================================");
        LOG_INFO("Smart Helmet Client application logic constructor");
        LOG_INFO("========================================================================================================================="); 

        CameraViewer viewer;
        viewer.showFullScreen();

        return app.exec();
    } catch (const std::exception& e) {
        LOG_ERROR("APPLICATION NOT RUNNING: " + std::string(e.what()));
        return -1;
    }
}

// main helmet end

//g++ -o audio_app main.cpp Audio.h     $(pkg-config --cflags --libs gstreamer-1.0)     -lpulse     -std=c++11
// #include <iostream>
// #include "Audio.h" // Ensure this includes the full code you provided, including AudioPlayer and AudioStreamer classes
// #include <cstdlib>  // for setenv

// // Define the config struct (or class) with your incoming and outgoing audio pipeline URLs
// struct Config {
//     std::string audio_incoming_pipeline = "udpsrc port=5002 caps=\"application/x-rtp,clock-rate=8000\" ! rtpjitterbuffer drop-on-latency=True latency=100 ! rtpspeexdepay ! queue ! speexdec enh=false ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,rate=44100,channels=2 ! pulsesink device=alsa_output.platform-sound-wm8904.stereo-fallback";
//     std::string audio_outcoming_pipeline = "pulsesrc device=alsa_input.platform-sound-wm8904.stereo-fallback ! volume volume=5.0 ! opusenc complexity=0 frame-size=60 bandwidth=narrowband bitrate=32000 ! rtpopuspay ! udpsink host=192.168.1.145 port=6000";
// };

// int main() {
//     setenv("GST_PLUGIN_PATH", "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/", 1);
//     setenv("GST_DEBUG", "3", 1); 
//     Config config;
//     // AudioControl A_control;
//     // A_control.setVolumeLevel(90);
//     // A_control.setCaptureInputType("DMIC");
//     // A_control.setCaptureInputVolume(12);
//     // A_control.setCaptureInputType("ADC");
//     // A_control.setCaptureInputVolume(72);
//     // Create AudioPlayer and AudioStreamer instances
//     AudioPlayer A_player(" ");
//     AudioStreamer A_streamer(" ");

//     try {
//         // Initialize and run AudioPlayer
//         A_player.update_pipline(config.audio_incoming_pipeline);
//         A_player.init();
//         A_player.run();

//         A_streamer.update_pipline(config.audio_outcoming_pipeline);
//         // Initialize and run AudioStreamer
//         A_streamer.init();
//         A_streamer.run();

//         // Keep the program running to allow GStreamer pipelines to play
//         std::cout << "Audio streaming and playback started. Press Enter to stop..." << std::endl;
//         std::cin.get(); // Wait for user input to exit

//         // Stop the pipelines
//         A_player.quit();
//         // A_streamer.quit();
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }

//     return 0;
// }


// #include "WiFiManager.h"
// #include <iostream>
// #include <thread>
// #include <chrono>

// int main() {
//     try {
//         // Initialize HTTPSession with API URL and SSL certificate path
//         Configuration config("configuration_ap.json");
//         Json::Value wifi;
//         Json::CharReaderBuilder readerBuilder;
//         std::string errors;
//         LOG_INFO("Reading WIFI file " + config.wifi_file);
//         std::string uri;
//         std::ifstream file(config.wifi_file, std::ifstream::binary);
//         if (!file) {
//             LOG_ERROR("Error: Could not open file.");
//             return 1;
//         }
//         bool parsingSuccessful = Json::parseFromStream(readerBuilder, file, &wifi, &errors);
//         if (!parsingSuccessful) {
//             LOG_ERROR("Error parsing JSON: " + errors);
//             return 1;
//         }
//         if (wifi.isArray() && wifi.size() > 0 && wifi[0].isMember("uri")) {
//             uri = wifi[0]["uri"].asString();
//             LOG_INFO("URI: " + uri);
//         } else {
//             LOG_ERROR("Error: JSON format is not as expected." );
//             return 1;
//         }
//         LOG_INFO("FINISH WIFI file");
//         HTTPSession session("https://"+uri, config.ssl_cert_path, config.api_key);
//         LOG_INFO("FINISH HTTPSession");

//         WiFiManager _network(config.wireless_interface, session, config);
//         LOG_INFO("FINISH WiFiManager");
        
        
//         // Authenticate with the server
//         LOG_INFO("_network.init()...");
//         _network.init();
//         _network.setConnections(wifi);
//         LOG_INFO("_network.setConnections(wifi) successful.");
//         _network.run();
//         LOG_INFO("_network.run() successful.");
//         // g++ main.cpp WiFiManager.h HTTPSession.h Configuration.h Logger.h -o wifi_manager_app  -lcurl -ljsoncpp -std=c++11 -lpthread
//         // // Get the VPN certificate
//         // std::cout << "Getting VPN certificate..." << std::endl;
//         // session.get_certificate("data/configuration.ovpn");
//         // std::cout << "VPN certificate obtained successfully." << std::endl;
        
//         // // Record the IP address
//         // std::cout << "Recording IP address..." << std::endl;
//         // session.record_ip("192.168.1.100");
//         // std::cout << "IP address recorded successfully." << std::endl;
        
//         // // Check if HTTP is available
//         // std::cout << "Checking HTTP availability..." << std::endl;
//         // if (session.is_http_available("example.com")) {
//         //     std::cout << "HTTP is available." << std::endl;
//         // } else {
//         //     std::cout << "HTTP is not available." << std::endl;
//         // }

//         // // Check if VPN is available
//         // std::cout << "Checking VPN availability..." << std::endl;
//         // if (session.is_vpn_available()) {
//         //     std::cout << "VPN is available." << std::endl;
//         // } else {
//         //     std::cout << "VPN is not available." << std::endl;
//         // }
//     } catch (const std::exception &e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }

//     return 0;
// }


// #include <iostream>
// #include <fstream>
// #include <string>
// #include <cstdio>
// #include <memory>
// #include <regex>
// #include <stdexcept>

// bool Vpntask(const std::string& file_path) {
//     std::string fullCommand = "sudo -n /usr/sbin/openvpn --config " + file_path;
//     std::cout << "Executing command: " << fullCommand << std::endl;

//     FILE* pipe = popen(fullCommand.c_str(), "r");
//     if (!pipe) {
//         std::cerr << "Failed to open pipe for command: " << fullCommand << std::endl;
//         return false;
//     }

//     std::string client_address;
//     char buffer[256];
//     std::regex ip_regex("/sbin/ip addr add dev tun0 ([^/]+)/24");

//     while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//         std::string line(buffer);
//          std::cout << line << std::endl;
//         if (line.find("Initialization Sequence Completed") != std::string::npos) {
//             std::cout << "[VPN] Initialization Sequence Completed" << std::endl;
//             std::cout << "config.client_address: " << client_address << std::endl;
//             std::cout << "h_authenticated" << std::endl;
//             pclose(pipe);
//             return true;
//         } 
//         else if (line.find("/sbin/ip addr add dev tun0") != std::string::npos) {
//             std::smatch match;
//             if (std::regex_search(line, match, ip_regex) && match.size() > 1) {
//                 client_address = match.str(1);  // Capture the IP address
//                 std::cout << "[VPN] New address: " << client_address << std::endl;
//             }
//         }
//     }

//     pclose(pipe);
//     return false;
// }

// void readOutputFile(const std::string& outputFilePath) {
//     std::ifstream inputFile(outputFilePath);
//     if (!inputFile.is_open()) {
//         std::cerr << "Failed to open file: " << outputFilePath << std::endl;
//         return;
//     }

//     std::string line;
//     while (std::getline(inputFile, line)) {
//         std::cout << line << std::endl;
//     }

//     inputFile.close();
// }

// int main() {
//     std::string configFilePath = "data/configuration.ovpn";
//     std::string outputFilePath = "output.txt";
    
//     if (Vpntask(configFilePath)) {
//         std::cout << "Output saved to " << outputFilePath << std::endl;
//     } else {
//         std::cerr << "Failed to save output." << std::endl;
//     }
    
//     return 0;
// }

// #include <opencv2/opencv.hpp>
// #include <iostream>

// int main() {
//     // Input video pipeline
//     std::string input_pipeline = "v4l2src device=/dev/video3 ! video/x-raw, format=YUY2, width=640, height=480 ! videoconvert ! appsink sync=false max-buffers=1 drop=true";
//     cv::VideoCapture cap;

//     // Output video pipeline
//     std::string host = "192.168.1.124"; // Replace with receiver's IP
//     std::string port = "5000"; // Replace with desired port
//     std::string output_pipeline = "appsrc ! videoconvert ! videoscale ! vpuenc_h264 bitrate=5000 ! rtph264pay config-interval=3 mtu=1400 ! udpsink host=" + host + " port=" + port;
//     cv::VideoWriter scap;

//     // Open input pipeline
//     if (!cap.open(input_pipeline, cv::CAP_GSTREAMER)) {
//         std::cerr << "Error: Could not open input pipeline." << std::endl;
//         return -1;
//     }

//     // Open output pipeline
//     int fps = 25;
//     cv::Size output_size(640, 480);
//     if (!scap.open(output_pipeline, cv::CAP_GSTREAMER, fps, output_size, true)) {
//         std::cerr << "Error: Could not open output pipeline." << std::endl;
//         return -1;
//     }

//     // Frame processing loop
//     cv::Mat frame, resized_frame;
//     std::cout << "Streaming video. Press Ctrl+C to stop." << std::endl;
//     while (true) {
//         // Capture frame
//         if (!cap.read(frame)) {
//             std::cerr << "Error: Could not read frame from input pipeline." << std::endl;
//             break;
//         }

//         // Resize frame
//         cv::resize(frame, resized_frame, output_size, 0, 0, cv::INTER_NEAREST);

//         // Write frame to output pipeline
//         scap.write(resized_frame);
//     }

//     // Release resources
//     cap.release();
//     scap.release();

//     std::cout << "Streaming stopped." << std::endl;
//     return 0;
// }

// #include "gpio.h" // Include your GPIOThread class
// #include <iostream>
// #include <csignal> // For handling termination signals
// #include <atomic>

// // Signal handler to gracefully exit the program
// std::atomic<bool> running(true);
// void signalHandler(int signum) {
//     std::cout << "\nInterrupt signal (" << signum << ") received. Stopping the program...\n";
//     running = false;
// }

// void buttonPressed() {
//     std::cout << "Button pressed event detected!" << std::endl;
// }

// int main() {
//     // Register signal handler
//     signal(SIGINT, signalHandler);

//     try {
//         // Create a GPIO thread to monitor gpiochip2, line 25
//         GPIOThread gpio_thread("gpiochip2", 25);

//         // Set the button pressed callback
//         gpio_thread.set_button_pressed_callback(buttonPressed);

//         std::cout << "GPIO monitoring started. Press CTRL+C to exit." << std::endl;

//         // Start the GPIO thread
//         gpio_thread.start();

//         // Keep the main thread alive until the user interrupts
//         while (running) {
//             std::this_thread::sleep_for(std::chrono::seconds(1));
//         }

//         // Stop the GPIO thread
//         gpio_thread.stop();
//         std::cout << "GPIO monitoring stopped." << std::endl;

//     } catch (const std::exception &e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return EXIT_FAILURE;
//     }

//     return EXIT_SUCCESS;
// }

// #include <iostream>
// #include <string>
// #include <openssl/aes.h>
// #include <openssl/evp.h>
// #include <openssl/bio.h>
// #include <openssl/buffer.h>
// #include <cstring> // Include this for memset()

// // Function to decode base64
// std::string base64_decode_openssl(const std::string &encoded) {
//     BIO *bio, *b64;
//     char *decoded = new char[encoded.size()];
//     memset(decoded, 0, encoded.size());
    
//     b64 = BIO_new(BIO_f_base64());
//     bio = BIO_new_mem_buf(encoded.c_str(), encoded.size());
//     bio = BIO_push(b64, bio);

//     // Disable line breaks
//     BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

//     // Perform decoding
//     int decoded_length = BIO_read(bio, decoded, encoded.size());
//     BIO_free_all(bio);

//     std::string result(decoded, decoded_length);
//     delete[] decoded;
//     return result;
// }

// // Function to decrypt AES-128-ECB using EVP API (recommended for OpenSSL 3.0)
// std::string aes_decrypt_ecb(const std::string &cipherText, const std::string &key) {
//     EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
//     if (!ctx) {
//         throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
//     }

//     unsigned char decrypted[1024] = {0}; // Adjust size based on expected output
//     int len = 0, plaintext_len = 0;

//     // Initialize decryption
//     if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, 
//         reinterpret_cast<const unsigned char *>(key.c_str()), NULL)) {
//         EVP_CIPHER_CTX_free(ctx);
//         throw std::runtime_error("Failed to initialize AES decryption");
//     }

//     // Disable padding
//     EVP_CIPHER_CTX_set_padding(ctx, 0);

//     // Perform decryption
//     if (!EVP_DecryptUpdate(ctx, decrypted, &len, 
//         reinterpret_cast<const unsigned char *>(cipherText.c_str()), cipherText.size())) {
//         EVP_CIPHER_CTX_free(ctx);
//         throw std::runtime_error("Failed to decrypt");
//     }
//     plaintext_len = len;

//     if (!EVP_DecryptFinal_ex(ctx, decrypted + len, &len)) {
//         EVP_CIPHER_CTX_free(ctx);
//         throw std::runtime_error("Failed to finalize decryption");
//     }
//     plaintext_len += len;

//     EVP_CIPHER_CTX_free(ctx);

//     return std::string(reinterpret_cast<const char *>(decrypted), plaintext_len);
// }

// // Main function
// int main() {
//     // Base64-encoded string
//     std::string base64_payload = "aB3BSMM7HVd/uAQSQygQRLYi0nz8lT8srLL7nPfjRnm2A5P/qwLLa4j1EPFSeGov0ugTp8Xy7kXJnA7EBkQgOw==";
//     // AES key
//     std::string aes_key = "mZq4t7w!z%C*F-Ja";

//     try {
//         // Step 1: Base64 decode
//         std::string decoded_payload = base64_decode_openssl(base64_payload);
//         std::cout << "Base64 Decoded: " << decoded_payload << std::endl;

//         // Step 2: AES-128-ECB decrypt
//         std::string decrypted_message = aes_decrypt_ecb(decoded_payload, aes_key);
//         std::cout << "Decrypted Message: " << decrypted_message << std::endl;

//     } catch (const std::exception &e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }

//     return 0;
// }
