#include "VideoPlayer.h"
#include <QApplication>
#include <cstdlib>  // For setenvy

void setEnvIfDifferent(const char* var, const char* desired_value) {
    const char* current_value = getenv(var);
    if (!current_value || strcmp(current_value, desired_value) != 0) {
        setenv(var, desired_value, 1);
        std::cout << "Set " << var << " to " << desired_value << std::endl;  // Optional debug output
    } else {
        std::cout << var << " is already set to " << desired_value << std::endl;  // Optional debug output
    }
}

int main(int argc, char *argv[])
{
    setEnvIfDifferent("GST_PLUGIN_PATH", "/usr/lib/aarch64-linux-gnu/gstreamer-1.0/");
    // setEnvIfDifferent("LD_LIBRARY_PATH", "/usr/lib/aarch64-linux-gnu/");
    setEnvIfDifferent("XDG_RUNTIME_DIR", "/run/user/0");
    setEnvIfDifferent("WAYLAND_DISPLAY", "wayland-1");
    setEnvIfDifferent("QT_QPA_PLATFORM", "wayland");
    // setEnvIfDifferent("MESA_LOADER_DRIVER_OVERRIDE", "vivante");
    // setEnvIfDifferent("EGL_PLATFORM", "wayland");
    setEnvIfDifferent("DISPLAY", ":0");
    // setEnvIfDifferent("GST_DEBUG", "3");
    // setEnvIfDifferent("GST_PLUGIN_FEATURE_RANK", "vaapih264dec:256");
    QApplication app(argc, argv);

    if (argc < 2) {
        std::cout << "Usage: ./video_player <video-file-path>"<< std::endl; 
        return -1;
    }

    QString videoPath = argv[1];
    VideoPlayer player(videoPath);
    player.show();

    return app.exec();
}
