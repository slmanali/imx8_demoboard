#include "/home/x_user/my_camera_project/Audio.h"
#include <cassert>
#include <iostream>

void testAudioControl() {
    AudioControl audioControl;
    std::cout << "Starting AudioControl tests...\n";

    // Test Headphone Volume
    std::cout << "Testing Headphone Volume...\n";
    audioControl.setVolumeLevel(50);
    int headphoneVol = audioControl.getVolumeLevel();
    assert(headphoneVol == 50 && "Headphone volume mismatch");
    audioControl.setVolumeLevel(0); // Test min
    assert(audioControl.getVolumeLevel() == 0 && "Headphone min volume failed");
    audioControl.setVolumeLevel(63); // Test max
    assert(audioControl.getVolumeLevel() == 63 && "Headphone max volume failed");

    // Test Line Output Volume
    std::cout << "Testing Line Output Volume...\n";
    audioControl.setLineOutputVolume(40);
    int lineVol = audioControl.getLineOutputVolume();
    assert(lineVol == 40 && "Line Output volume mismatch");
    audioControl.setLineOutputVolume(0);
    assert(audioControl.getLineOutputVolume() == 0 && "Line Output min volume failed");
    audioControl.setLineOutputVolume(63);
    assert(audioControl.getLineOutputVolume() == 63 && "Line Output max volume failed");

    // Test Capture Input Volume
    std::cout << "Testing Capture Input Volume...\n";
    audioControl.setCaptureInputVolume(20);
    int captureVol = audioControl.getCaptureInputVolume();
    assert(captureVol == 20 && "Capture Input volume mismatch");
    audioControl.setCaptureInputVolume(0);
    assert(audioControl.getCaptureInputVolume() == 0 && "Capture Input min volume failed");
    audioControl.setCaptureInputVolume(31);
    assert(audioControl.getCaptureInputVolume() == 31 && "Capture Input max volume failed");

    // Test Capture Input Type
    std::cout << "Testing Capture Input Type...\n";
    audioControl.setCaptureInputType("ADC");
    std::string inputType = audioControl.getCaptureInputType();
    assert(inputType == "ADC" && "Capture Input Type ADC mismatch");
    audioControl.setCaptureInputType("DMIC");
    inputType = audioControl.getCaptureInputType();
    assert(inputType == "DMIC" && "Capture Input Type DMIC mismatch");

    // Test Digital Playback Volume
    std::cout << "Testing Digital Playback Volume...\n";
    audioControl.setDigitalPlaybackVolume(80);
    int playbackVol = audioControl.getDigitalPlaybackVolume();
    assert(playbackVol == 80 && "Digital Playback volume mismatch");
    audioControl.setDigitalPlaybackVolume(0);
    assert(audioControl.getDigitalPlaybackVolume() == 0 && "Digital Playback min volume failed");
    audioControl.setDigitalPlaybackVolume(96);
    assert(audioControl.getDigitalPlaybackVolume() == 96 && "Digital Playback max volume failed");

    // Test Digital Capture Volume
    std::cout << "Testing Digital Capture Volume...\n";
    audioControl.setDigitalCaptureVolume(100);
    int digitalCaptureVol = audioControl.getDigitalCaptureVolume();
    assert(digitalCaptureVol == 100 && "Digital Capture volume mismatch");
    audioControl.setDigitalCaptureVolume(0);
    assert(audioControl.getDigitalCaptureVolume() == 0 && "Digital Capture min volume failed");
    audioControl.setDigitalCaptureVolume(119);
    assert(audioControl.getDigitalCaptureVolume() == 119 && "Digital Capture max volume failed");

    // Test Digital Playback Boost Volume
    std::cout << "Testing Digital Playback Boost Volume...\n";
    audioControl.setDigitalPlaybackBoostVolume(2);
    int boostVol = audioControl.getDigitalPlaybackBoostVolume();
    assert(boostVol == 2 && "Digital Playback Boost volume mismatch");
    audioControl.setDigitalPlaybackBoostVolume(0);
    assert(audioControl.getDigitalPlaybackBoostVolume() == 0 && "Digital Playback Boost min volume failed");
    audioControl.setDigitalPlaybackBoostVolume(3);
    assert(audioControl.getDigitalPlaybackBoostVolume() == 3 && "Digital Playback Boost max volume failed");

    // Test Digital Sidetone Volume
    std::cout << "Testing Digital Sidetone Volume...\n";
    audioControl.setDigitalSidetoneVolume(10);
    int sidetoneVol = audioControl.getDigitalSidetoneVolume();
    assert(sidetoneVol == 10 && "Digital Sidetone volume mismatch");
    audioControl.setDigitalSidetoneVolume(0);
    assert(audioControl.getDigitalSidetoneVolume() == 0 && "Digital Sidetone min volume failed");
    audioControl.setDigitalSidetoneVolume(15);
    assert(audioControl.getDigitalSidetoneVolume() == 15 && "Digital Sidetone max volume failed");

    std::cout << "All AudioControl tests passed!\n";
}

int main() {
    try {
        testAudioControl();
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}