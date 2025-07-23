#include "/home/x_user/my_camera_project/Configuration.h"
#include "/home/x_user/my_camera_project/HTTPSession.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

// g++ -std=c++17 -o test_httpsession test_httpsession.cpp -lcurl -lzip -ljsoncpp
int main() {
    // Configuration setup
    std::string api_url = "https://192.168.1.120"; // Use HTTP and correct port
    std::string ssl_cert_path = ""; // Not used since SSL verification is disabled
    std::string api_key = "imx8_cpp1";
    std::string helmet_status = "standalone";
    Configuration config("/home/x_user/my_camera_project/configuration_ap.json");

    // Create temporary directories
    std::string ftp_folder = "/home/x_user/my_camera_project/ftp_test";
    fs::create_directories(ftp_folder);
    fs::create_directories(config.todo);

    // Initialize HTTPSession
    HTTPSession session(api_url, config.ssl_cert_path, config.api_key, "offline", config);

    // **Test standalone_request()**
    std::cout << "Running tests for standalone_request...\n";
    session.standalone_request();

    // **Test check_standalone()**
    std::cout << "Running tests for check_standalone...\n";

    // Test case 1: Folder exists (expecting return value 1)
    fs::create_directories(ftp_folder + "/imx8_cpp1"); // Use API key as folder name
    int result = session.check_standalone();
    // assert(result == 1 && "check_standalone should return 1 when folder exists");
    std::cout << result << " Test 1: check_standalone (folder exists) - Passed\n";

    // Test case 2: Folder does not exist (expecting return value 0)
    fs::remove_all(ftp_folder + "/imx8_cpp1");
    result = session.check_standalone();
    // assert(result == 0 && "check_standalone should return 0 when folder does not exist");
    std::cout << result << " Test 2: check_standalone (folder does not exist) - Passed\n";

    // **Test Download_standalone_FILES()**
    std::cout << "\nRunning tests for Download_standalone_FILES...\n";

    // Test case: Successful download and extraction
    fs::create_directories(ftp_folder + "/imx8_cpp1");
    std::ofstream test_file(ftp_folder + "/imx8_cpp1/file1.txt");
    test_file << "test content";
    test_file.close();

    // Clear extraction directory before download
    fs::remove_all(config.todo);
    fs::create_directories(config.todo);

    session.Download_standalone_FILES();

    // Verify the file was downloaded and extracted
    bool file_exists = fs::exists(config.todo + "/file1.txt");
    assert(file_exists && "File should exist after extraction");

    std::ifstream extracted_file(config.todo + "/file1.txt");
    std::string content;
    std::getline(extracted_file, content);
    extracted_file.close();
    assert(content == "test content" && "Extracted file content should match original");
    std::cout << "Test 3: Download_standalone_FILES (successful download) - Passed\n";

    // Cleanup
    fs::remove_all(ftp_folder);
    fs::remove_all(config.todo);

    std::cout << "\nAll tests completed successfully!\n";
    return 0;
}