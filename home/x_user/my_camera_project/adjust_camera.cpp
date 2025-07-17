#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <cstring>

bool set_control(int fd, __u32 id, int32_t value) {
    struct v4l2_control control = {};
    control.id = id;
    control.value = value;

    if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) {
        std::cerr << "Failed to set control: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

int main() {
    const char* device = "/dev/v4l-subdev2";  // OV5640 sensor
    int fd = open(device, O_RDWR);
    
    if (fd == -1) {
        std::cerr << "Failed to open device: " << strerror(errno) << std::endl;
        return -1;
    }

    // Adjust camera settings
    // set_control(fd, V4L2_CID_CONTRAST, 128);          // Set contrast to 128
    set_control(fd, V4L2_CID_SATURATION, 64);        // Set saturation to 200
    // set_control(fd, V4L2_CID_WHITE_BALANCE_TEMPERATURE_AUTO, 0); // Disable auto white balance
    set_control(fd, V4L2_CID_RED_BALANCE, 2000);      // Set red balance
    set_control(fd, V4L2_CID_BLUE_BALANCE, 1800);     // Set blue balance
    set_control(fd, V4L2_CID_HUE, 359);     // Set blue balance
    set_control(fd, V4L2_CID_EXPOSURE, 740);       // Disable auto exposure (manual mode)
    // set_control(fd, V4L2_CID_EXPOSURE_ABSOLUTE, 600); // Set exposure to 600
    // set_control(fd, V4L2_CID_HFLIP, 1);               // Horizontal flip
    // set_control(fd, V4L2_CID_VFLIP, 1);               // Vertical flip

    close(fd);
    std::cout << "Camera settings adjusted successfully!" << std::endl;
    return 0;
}
