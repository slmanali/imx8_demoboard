# Code Documentation for `adjust_camera.cpp`

## Overview

The `adjust_camera.cpp` program is a command-line utility that interfaces with a video device using the Video4Linux2 (v4l2) API. The purpose of this program is to set various camera parameters, such as contrast, saturation, white balance, exposure, and image orientation, for a camera specified by a device file. The camera used as an example in this code is the OV5640 sensor, represented by the device file `/dev/v4l-subdev2`.

## Includes and Libraries

```cpp
#include <iostream>                // For input/output stream, e.g., std::cout
#include <fcntl.h>                // For file control operations (open)
#include <unistd.h>               // For POSIX API, including close
#include <linux/videodev2.h>      // For v4l2 controls definitions
#include <sys/ioctl.h>           // For the ioctl function to control devices
#include <cstring>                // For using C-style strings and memory functions
```

1. **iostream**: Provides functionalities for input and output through streams.
2. **fcntl.h**: Contains file control options, including the `open` function.
3. **unistd.h**: Provides access to the POSIX operating system API including the `close` function.
4. **linux/videodev2.h**: Contains definitions of video device controls which are used to set camera properties.
5. **sys/ioctl.h**: Provides the `ioctl` function which is used for device-specific input/output operations.
6. **cstring**: Offers C-style string and memory manipulation functions.

## Function: `set_control`

```cpp
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
```

### Parameters
- **int fd**: The file descriptor for the video device, obtained through the `open` call.
- **__u32 id**: The control ID specifying which property to change (e.g., contrast, saturation).
- **int32_t value**: The desired value to set for the control identified by `id`.

### Returns
- **bool**: Returns `true` if the control was successfully set, `false` otherwise. In case of failure, an error message is printed to `std::cerr`.

### Description
This function takes a file descriptor for a video device, a control ID, and a value. It creates a `v4l2_control` structure and populates it with the provided ID and value. The `ioctl` system call is used to attempt to set the control on the device. If it fails, it outputs an error message indicating what went wrong.

## Program Entry Point: `main`

```cpp
int main() {
    const char* device = "/dev/v4l-subdev2";  // OV5640 sensor
    int fd = open(device, O_RDWR);
    
    if (fd == -1) {
        std::cerr << "Failed to open device: " << strerror(errno) << std::endl;
        return -1;
    }

    // Adjust camera settings
    set_control(fd, V4L2_CID_CONTRAST, 128);          // Set contrast to 128
    set_control(fd, V4L2_CID_SATURATION, 200);        // Set saturation to 200
    set_control(fd, V4L2_CID_WHITE_BALANCE_TEMPERATURE_AUTO, 0); // Disable auto white balance
    set_control(fd, V4L2_CID_RED_BALANCE, 2000);      // Set red balance
    set_control(fd, V4L2_CID_BLUE_BALANCE, 1800);     // Set blue balance
    set_control(fd, V4L2_CID_AUTO_EXPOSURE, 1);       // Disable auto exposure (manual mode)
    set_control(fd, V4L2_CID_EXPOSURE_ABSOLUTE, 600); // Set exposure to 600
    set_control(fd, V4L2_CID_HFLIP, 1);               // Horizontal flip
    set_control(fd, V4L2_CID_VFLIP, 1);               // Vertical flip

    close(fd);
    std::cout << "Camera settings adjusted successfully!" << std::endl;
    return 0;
}
```

### Description
1. The program specifies the video device to be opened, which in this case is the OV5640 sensor located at `/dev/v4l-subdev2`.
2. It attempts to open this device in read/write mode (`O_RDWR`). If the open fails, it outputs an error message and returns `-1` to indicate the failure.
3. If the device is opened successfully, it proceeds to adjust various camera settings using the `set_control` function. The settings include:
   - **Contrast**: Set to 128.
   - **Saturation**: Set to 200.
   - **White Balance**: Auto disabled (set to `0`).
   - **Red Balance**: Set to `2000`.
   - **Blue Balance**: Set to `1800`.
   - **Auto Exposure**: Disabled (set to `1` indicates manual mode).
   - **Exposure**: Set to `600` (in arbitrary units).
   - **Horizontal Flip**: Enabled (set to `1`).
   - **Vertical Flip**: Enabled (set to `1`).
4. The file descriptor is closed using `close(fd)`, and the program indicates that the camera settings were adjusted successfully.

### Notes
- The program assumes that the user has the necessary permissions to access the camera device.
- The control IDs (like `V4L2_CID_CONTRAST`) are from the v4l2 library and may vary based on the camera and driver capabilities.
- The hard-coded values for controls should be tested for each specific camera as they may lead to unexpected results if the values are outside allowed ranges.

## Conclusion
The `adjust_camera.cpp` program provides a functional demonstration of how to interface with a camera device using the Video4Linux2 API to adjust camera settings. It performs error checking for device access and control setting to ensure robust operation.

---