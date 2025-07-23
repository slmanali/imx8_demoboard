# IMX8 Demoboard

### UI Layout Preview

[View the interface layout](layout.PNG)

This repository contains reference files and documentation for setting up an i.MX8 based demo system. The focus is a Qt/C++ application running on a Debian-based image with additional services and configuration helpers.

## Repository layout

boot/                     Device tree binary for the i.MX8 board
etc/systemd/system/       Custom systemd services
home/x_user/my_camera_project/  Main Qt/C++ application
home/x_user/test/         Example source code
root/                     Startup scripts

### boot/

* **imx8mp-var-dart-dt8mcustomboard3.dtb** – compiled device tree used during boot. See [Device tree for IMX8.md](Device%20tree%20for%20IMX8.md) for details.

### etc/systemd/system/

* **helmet8cpp.service** – runs the helmet Qt application at boot.
* **nmap_scan.service** – performs a network scan after the interface is up.

### home/x_user/my_camera_project/

The main application sources. Important items include:

* `main.cpp` and several `*.h`/`*.cpp` files implementing camera capture, audio processing, and networking.
* `my_camera_project.pro` – Qt build configuration.
* `run_app.sh` – helper script that rotates logs and launches the compiled binary.
* `gps_init.sh` – script for GPS data initialization.
* `assets/` – CSS, images, and audio assets used by the UI.
* `data/` – example CSV logs and configuration files.
* `onnxruntime/` – prebuilt ONNX Runtime library files.
* `vosk-model-*` – speech recognition models.

### home/x_user/test/

Small experimental programs, including `vosk_gstreamer_example.cpp` and utilities used during development.

### root/

Contains **startup_nmap_scan.sh** which waits for Wi-Fi connectivity and repeatedly scans the network until another host responds.

### Additional documentation

* **imx8_cpp_kernel_6.md** – step-by-step instructions for preparing the development environment and installing required packages.
* **Device tree for IMX8.md** – notes on the device tree used with this board.
* **interface_layout.html** and **layout.PNG** – overview of the UI layout.
* Numerous `*.md` files inside `home/x_user/my_camera_project` describe specific modules and scripts.

## Building the application

1. Ensure Qt5 (or Qt6) and the listed dependencies from `imx8_cpp_kernel_6.md` are installed on the target device.
2. Navigate to `home/x_user/my_camera_project` and run:
   ```bash
   qmake
   make -j$(nproc)
   ```
3. Run the application with `./my_camera_project` or use `run_app.sh` which handles log rotation and environment variables.

## Services

After installation, enable the systemd units so the application and network scan start automatically:

```bash
sudo systemctl daemon-reload
sudo systemctl enable helmet8cpp.service
sudo systemctl enable nmap_scan.service
```

This repository provides a starting point for experimenting with the i.MX8 device and the helmet project. Consult the markdown files for more details on individual components and setup instructions.
