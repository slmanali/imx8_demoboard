# Documentation for `run_app.sh`

## Overview

The `run_app.sh` script is a Bash script designed for setting up the environment and managing log files for a camera project that utilizes the Qt framework and GStreamer. The script provides functionality to create backups of log files when they exceed a specified size limit and subsequently launches the main application.

## Prerequisites

- Bash shell support
- The script is intended for a Linux environment, specifically for systems with Wayland and EGLFS configurations.
- GStreamer installed in the specified path (/usr/lib/aarch64-linux-gnu/gstreamer-1.0/).

## Usage

To execute this script, run the following command in a terminal:

```bash
bash run_app.sh
```

## Environment Configuration

The script includes several commented-out environment configurations that users can enable depending on their specific setup:

1. **DISPLAY Management**: 
    - Optionally unset the `DISPLAY` variable to avoid using X11.
    - Optionally unset the `QT_PLUGIN_PATH` if necessary.

2. **EGLFS Setup**: 
    - Various variables related to EGLFS platform configuration for graphics rendering using direct framebuffer.
    - `XDG_RUNTIME_DIR`: Represents the runtime directory for user services.
    - `QT_QPA_PLATFORM`: Specifies the platform to be used (Wayland in this case).
    - `WAYLAND_DISPLAY`: Defines the display socket for Wayland.

3. **KMS Integration**: 
    - Users can specify a particular KMS (Kernel Mode Setting) integration for i.MX8 by uncommenting one of the provided options.

4. **GStreamer Plugin Path**: 
    - The GStreamer plugin path can be set if additional plugins are needed for the application.

## Backing Up Log Files

### Variables

- **MAX_SIZE_MB**: Defines the maximum allowed log file size in megabytes (default is 10 MB).
- **BACKUP_DIR**: The directory where log file backups will be stored (default is `/home/x_user/my_camera_project/old_logs`).
- **FILES**: An array of log file paths to be checked and backed up.

### Logic

1. A backup directory is created if it does not already exist.
2. The script iterates over each file in the `FILES` array:
    - Checks if the file exists.
    - Retrieves the file size using `stat`.
    - Compares the file size to the threshold defined by `MAX_SIZE_MB`.
    - If the file size exceeds the threshold:
        - A timestamped backup filename is created.
        - The original file is backed up to the defined backup directory.
        - The original file is truncated to reduce its size to zero bytes.
        - A message is printed to the console indicating the successful backup.

## Launching the Application

After the backup process, the script changes the working directory to the project's directory (`/home/x_user/my_camera_project`) and sets the `GST_PLUGIN_PATH` for GStreamer. Finally, it executes the main application:

```bash
./my_camera_project
```

## Summary

The `run_app.sh` script efficiently manages backup operations for log files and prepares the environment for running a Qt-based camera application. Users can customize environment variables based on their needs, while also ensuring that log files do not exceed the designated size. Proper usage of this script helps maintain system stability and log integrity for the camera project.

---
