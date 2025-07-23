#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <chrono>

#define LOG_INFO(msg) std::cout << "[INFO] " << msg << std::endl
#define LOG_ERROR(msg) std::cerr << "[ERROR] " << msg << std::endl

const std::string wireless_interface = "wlan0";

bool connect(const std::string& ssid, const std::string& password) {
    try {
        // Generate WPA configuration
        std::stringstream cmd;
        cmd << "sudo wpa_passphrase \"" << ssid << "\" \"" << password << "\" > /etc/wpa_supplicant/wpa_supplicant.conf";
        int ret = system(cmd.str().c_str());
        if (ret != 0) {
            LOG_ERROR("Failed to generate WPA configuration");
            return false;
        }

        // Kill existing WPA Supplicant processes
        system("pkill -f wpa_supplicant");
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Start WPA Supplicant with configuration file
        cmd.str("");
        cmd << "sudo wpa_supplicant -B -i " << wireless_interface << " -c /etc/wpa_supplicant/wpa_supplicant.conf";
        ret = system(cmd.str().c_str());
        if (ret != 0) {
            LOG_ERROR("Failed to start WPA supplicant");
            return false;
        }

        // Obtain IP address via DHCP
        system(("sudo dhclient -r " + wireless_interface).c_str());
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ret = system(("sudo dhclient " + wireless_interface).c_str());
        if (ret != 0) {
            LOG_ERROR("Failed to obtain IP address");
            return false;
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));

        LOG_INFO("Connected successfully to SSID: " + ssid);
        return true;
    } catch (const std::exception& e) {
        LOG_ERROR("Exception: " << e.what());
        return false;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: sudo " << argv[0] << " <SSID> <PASSWORD>\n";
        return 1;
    }

    std::string ssid = argv[1];
    std::string password = argv[2];

    LOG_INFO("Attempting to connect to SSID: " + ssid);

    bool success = connect(ssid, password);

    if (success) {
        LOG_INFO("Wi-Fi connection established.");
        // Optionally test connectivity
        system("ping -c 3 google.com");
    } else {
        LOG_ERROR("Wi-Fi connection failed.");
    }

    return success ? 0 : 1;
}
