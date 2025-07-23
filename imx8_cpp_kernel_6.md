# C++ Library in Kernel - `6.1.36-imx8mp+ge6ac294d4629`

**OS: Debian GNU/Linux 12 (bookworm)**

---

## Getting Started

After burning the system into SD-card:

1. Run the testbench in external mode active.
2. Connect via serial-USB.
3. Burn the OS into eMMC using:
   ```bash
   install_debian.sh
   ```
4. Shut down the system and switch to internal mode in the testbench.
5. Connect to Wi-Fi using:
   ```bash
   sudo nmcli device wifi connect "TP-Link_71C0_5G" password "30963501"
   ```

> [**Note:**](#footnote1) In U-Boot:

```bash
bootcmd=gpio set 9; gpio set 131; gpio set 154; gpio set 155; gpio set 21; gpio clear 73; gpio clear 70; sleep 0.2; gpio set 73; gpio set 86; gpio set 88; gpio set 89; gpio set 120; run bsp_bootcmd
```

---

## Footnotes

`<a name="footnote1">`[1]`</a>`: Better to do it in the last step.

---

## Development Tools & Libraries Installation

### 1. Nano Editor

```bash
sudo apt-get update
sudo apt-get install v4l-utils nano
```

---

### 2. Qt Library

Best done via serial interface:

```bash
sudo apt install \
    qtbase5-dev \
    qttools5-dev-tools \
    qtdeclarative5-dev \
    qt5-qmake \
    libqt5widgets5 \
    libqt5gui5 \
    libqt5core5a \
    qtquickcontrols2-5-dev \
    qml-module-qtquick-controls2 \
    qml-module-qtquick-dialogs \
    qml-module-qtquick-layouts \
    qml-module-qtquick-window2 \
    qml-module-qtquick-xmllistmodel \
    qml-module-qtquick2
```

---

### 3. OpenVPN Library

Install OpenVPN 2.4.7 for Debian 10:
must be in the root directory
cd /root/

```bash
sudo apt-get -f install
sudo dpkg -i libssl1.1_1.1.1n-0+deb10u3_arm64.deb
sudo dpkg -i libpkcs11-helper1_1.25.1-1_arm64.deb
sudo dpkg -i openvpn_2.4.7-1+deb10u1_arm64.deb
```

---

### 4. JSON Library

```bash
sudo apt-get install libjsoncpp-dev
```

---

### 5. GPIO Library

```bash
sudo apt-get install gpiod libgpiod-dev
```

---

### 6. Libcurl

```bash
sudo apt-get install libcurl4-openssl-dev nlohmann-json3-dev
```

---

### 7. Network Tools

```bash
sudo apt install ipcalc nmap
```

---

### 8. PDF Libraries

For Qt5:

```bash
sudo apt install libpoppler-qt5-dev
```

For Qt6 (We are NOT using it- **NO NEED**):

```bash
sudo apt install libpoppler-qt6-dev
```

---

### 9. OpenCV Library

```bash
sudo apt-get install libopencv-dev
```

---

### 10. Upgrade GStreamer

Update `/etc/apt/sources.list` with:

```bash
nano /etc/apt/sources.list
```

then delete everything and add those

```bash
deb http://deb.debian.org/debian bookworm main contrib non-free non-free-firmware
deb-src http://deb.debian.org/debian bookworm main contrib non-free
deb http://security.debian.org/debian-security bookworm-security main contrib non-free non-free-firmware
deb-src http://security.debian.org/debian-security bookworm-security main contrib non-free
```

Then do this:

```bash
sudo apt-get update
sudo apt-get install libgstreamer-plugins-base1.0-0=1.22.0-3+deb12u4
sudo apt-get install gir1.2-gstreamer-1.0=1.22.0-2+deb12u1
sudo apt-get install libgstreamer1.0-0=1.22.0-2+deb12u1
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
```

For kernel 5 (We are NOT using it- **NO NEED**):

```bash
sudo apt-get install libgstreamer1.0-0=1.18.0-1
sudo apt-get install libgstreamer-plugins-base1.0-0=1.18.0-1
sudo apt-get install gir1.2-gstreamer-1.0=1.18.0-1
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
```

---

### 11. ZBar Library

```bash
sudo apt install libzbar-dev
```

---

### 12. Base64 Library

```bash
sudo apt install libb64-dev
```

---

### 13. VOSK for C++

IN General.

Download it from: [https://github.com/alphacep/vosk-api/releases/vosk-linux-aarch64-0.3.45.zip]()

But already installed the library (both the header file vosk_api.h and the library libvosk.so in the project directory)

```bash
sudo cp /home/x_user/my_camera_project/libvosk.so /usr/local/lib
sudo ldconfig
```

---

### 14. ZIP Library for PDF

```bash
sudo apt-get install libzip-dev
```

---

### 15. ONNX Runtime (ManDown Feature)

IN General

Download from: [https://github.com/microsoft/onnxruntime/releases/download/v1.18.0/onnxruntime-linux-aarch64-1.18.0.tgz]()

But already installed the library (both the header files  and the libraries in the folder onnxruntime in the project directory )

```bash
sudo cp /home/x_user/my_camera_project/onnxruntime/lib/libonnxruntime.so /usr/local/lib
sudo cp /home/x_user/my_camera_project/onnxruntime/lib/libonnxruntime.so.1.18.0 /usr/local/lib
sudo ldconfig
```

Test model (onnx_test) Test model + IMU (IMU_test)

```bash
cd /home/x_user/test
g++ onnx_test.cpp -o onnx_test -I/home/x_user/my_camera_project/onnxruntime/include -lonnxruntime
g++ -std=c++17 IMU_test.cpp -o imu_test -I/home/x_user/my_camera_project/onnxruntime/include -lonnxruntime -lm
```

#### Training Environment

This must done just in the main SOM.

```bash
sudo apt install git python3-pip cmake build-essential libprotobuf-dev protobuf-compiler libprotoc-dev
pip3 install joblib matplotlib --break-system-packages -i https://pypi.tuna.tsinghua.edu.cn/simple
pip3 install scikit-learn skl2onnx onnx onnxruntime numpy --break-system-packages -i https://pypi.tuna.tsinghua.edu.cn/simple
```

If pypi.org is slow/unreachable, use a reliable mirror (e.g., Tsinghua) add this -i https://pypi.tuna.tsinghua.edu.cn/simple
In the last step some error appears related to (torch2.0 and tvm) you can ignore it.
Use `Model_ManDown.py` to train. (Just in one)

```bash
cd /home/x_user/test
python3 Model_ManDown.py
```

---

### 16. Qt5GLib-2.0

```bash
sudo apt update
sudo apt install libgstreamer1.0-dev libglib2.0-dev qtbase5-dev
sudo apt-get install libqt5glib-2.0-0
sudo ln -s /usr/lib/aarch64-linux-gnu/libQt5GLib-2.0.so.1.2.0 /usr/lib/aarch64-linux-gnu/libQt5GLib-2.0.so
sudo ldconfig
```

---

### 17. GPS in C++

#### libserial

```bash
sudo apt-get install libserial-dev
```

#### libnmea

```bash
sudo apt install git build-essential cmake
cd /home/x_user/my_camera_project/
git clone https://github.com/jacketizer/libnmea.git
cd libnmea
mkdir build && cd build
cmake ..
make
sudo make install
sudo ldconfig
```

Test the nmea installation

```bash
cd /home/x_user/test
gcc yourfile.c -o yourff -I/usr/local/include -L/usr/local/lib -lnmea
./yourff
```

Must got this
libnmea test!
After this, you can delete this folder '/home/x_user/my_camera_project/libnmea'

```bash
rm -rf /home/x_user/my_camera_project/libnmea
```

#### Boost Dependencies

```bash
sudo apt install libboost-dev libboost-system-dev libboost-thread-dev
```

##### Compilation:

Those needed so must compile all of them. files must be in this (/home/x_user/my_camera_project)

```bash
g++ -std=c++17 -o epo_parser epo_parser.cpp -I/usr/local/include -L/usr/local/lib -lnmea
g++ -std=c++17 -pthread -o gps_config gps_config.cpp -lboost_system -lboost_thread
gcc -o epo_upload_demo epo_upload_demo.c (C version of epoloader)
g++ -std=c++17 -pthread gps_parser.cpp -o gps_parser -lboost_system -lboost_thread
```

---

### 18. U-Boot GPIO Settings

In U-Boot:

```bash
editenv bootcmd
bootcmd=gpio set 9; gpio set 131; gpio set 154; gpio set 155; gpio set 21; gpio clear 73; gpio clear 70; sleep 0.2; gpio set 73; gpio set 86; gpio set 88; gpio set 89; gpio set 120; run distro_bootcmd; run bsp_bootcmd
saveenv

editenv fdt_file
fdt_file=imx8mp-var-dart-dt8mcustomboard3.dtb
saveenv

boot
```

---

### 19. GPU (Vulkan)  (NO NEED just attempt to enable GPU)

```bash
sudo apt-get install vulkan-tools libvulkan-dev vulkan-validationlayers-dev
vulkaninfo
```

---

### 20. CPU Monitor

```bash
sudo apt install nmon
```

---

### 21. Bandwidth Monitor

```bash
sudo apt install bmon
```

---

### 22. CPU Performance Tuning

```bash
sudo apt-get install linux-cpupower
sudo cpupower frequency-set -g performance
cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

# To apply at boot
sudo nano /etc/udev/rules.d/99-cpufreq.rules
SUBSYSTEM=="cpu", ACTION=="add", KERNEL=="cpu[0-9]*", RUN+="/usr/bin/cpupower frequency-set -g performance"
SUBSYSTEM=="cpu", ACTION=="add", KERNEL=="cpu[0-9]*", RUN+="/usr/bin/cpupower frequency-set -f 1.60ghz"

sudo udevadm control --reload-rules
sudo udevadm trigger
```

---

### 23. Helmet Service Configuration

Edit `/root/.profile` and `/root/.bashrc`:

```bash
export XDG_RUNTIME_DIR=/run/user/0
export WAYLAND_DISPLAY=wayland-1
export QT_QPA_PLATFORM=wayland
export GST_PLUGIN_PATH=/usr/lib/aarch64-linux-gnu/gstreamer-1.0/
```

Then:

```bash
source /root/.bashrc
source /root/.profile
```

Create service:

```bash
sudo nano /etc/systemd/system/helmet8cpp.service
```

Update:

```ini
[Unit]
Description=Helmet C++ Application
After=weston.service

[Service]
Restart=always
RestartSec=20
User=root
WorkingDirectory=/home/x_user/my_camera_project
Environment="QT_QPA_PLATFORM=wayland"
Environment="XDG_RUNTIME_DIR=/run/user/0"
Environment="WAYLAND_DISPLAY=wayland-1"
ExecStart=/home/x_user/my_camera_project/run_app.sh

[Install]
WantedBy=graphical.target
```

Reload and restart:

```bash
sudo systemctl daemon-reload
sudo systemctl restart helmet8cpp.service
```

---

### 24. Network Scan Service

Create script `/root/startup_nmap_scan.sh`:

```bash
#!/bin/bash
while ! ip -4 addr show wlan0 | grep -q inet; do
    sleep 10
done

perform_scan () {
    IP=$(ip -4 addr show wlan0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
    NETMASK=$(ip -4 addr show wlan0 | awk '/inet/ { split($2, a, "/"); print a[2]}')
    NETWORK=$(ipcalc -n $IP/$NETMASK | awk '/Network:/ {print $2}')
    result=$(sudo nmap -sP $NETWORK --open --min-hostgroup 100)
    echo "$result"
    if echo "$result" | grep -q "Host is up"; then
        return 0
    else
        return 1
    fi
}

perform_scan
while [ $? -ne 0 ]; do
    echo "No hosts up, retrying in 10 seconds..."
    sleep 10
    perform_scan
done
```

Create systemd unit:

```bash
sudo nano /etc/systemd/system/nmap_scan.service
```

Add:

```ini
[Unit]
Description=Run nmap scan after network is up
After=network-online.target systemd-timesyncd

[Service]
Type=oneshot
ExecStart=/root/startup_nmap_scan.sh
RemainAfterExit=no

[Install]
WantedBy=multi-user.target
```

Enable and start:

```bash
sudo systemctl enable nmap_scan
sudo systemctl start nmap_scan
```

---

### 25. Power Monitoring

```bash
sudo apt install powertop
sudo powertop
```

---

### 26. Boost Library

```bash
sudo apt-get install libboost-all-dev
```

---

### 27. Audio Control (Optional - NO NEED)

```bash
sudo apt-get install pavucontrol
```

---

### 28. FFMPEG Build (NO NEED just attempt to replace the gstreamer)

```bash
wget https://ffmpeg.org/releases/ffmpeg-5.1.6.tar.gz
tar -xzf ffmpeg-5.1.6.tar.gz
cd ffmpeg-5.1.6
sudo apt install libx264-dev libv4l-dev libv4l2rds0 v4l-utils
./configure --prefix=/usr/local/ffmpeg --enable-shared --enable-gpl --enable-libx264 --enable-libv4l2
make -j4
sudo make install

export LD_LIBRARY_PATH=/usr/local/ffmpeg/lib:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=/usr/local/ffmpeg/lib/pkgconfig:$PKG_CONFIG_PATH
echo 'export LD_LIBRARY_PATH=/usr/local/ffmpeg/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
echo 'export PKG_CONFIG_PATH=/usr/local/ffmpeg/lib/pkgconfig:$PKG_CONFIG_PATH' >> ~/.bashrc
source ~/.bashrc
```

---

### 29. Perf Tool (Just need for developers)

```bash
sudo apt install linux-perf
perf stat ./my_program
perf record ./my_program
perf report
```

---

### 30. PDF Generation (libharu) NO NEED

```bash
sudo apt install libhpdf-dev libpng-dev build-essential zlib1g-dev fonts-dejavu
sudo apt install cmake
git clone https://github.com/libharu/libharu
cd libharu
mkdir build && cd build
cmake -DENABLE_TTF=ON -DENABLE_UTF8=ON ..
make
sudo make install
sudo ldconfig
```

---

### 31. Update to Qt6 (NO NEED just attempt)

```bash
sudo apt install qt6-base-dev qt6-multimedia-dev
```

---

### 32. GDB Debugger (Just need for developers)

```bash
sudo apt install gdb
```

Example usage:

```bash
gdb ./your_program
```

| Command                                     | Description        |
| ------------------------------------------- | ------------------ |
| `run`                                     | Start the program  |
| `break [file:]line` or `break function` | Set a breakpoint   |
| `next` or `n`                           | Step over          |
| `step` or `s`                           | Step into          |
| `print variable` or `p`                 | Print variable     |
| `backtrace` or `bt`                     | Show stack trace   |
| `continue` or `c`                       | Continue execution |
| `quit`                                    | Exit GDB           |

In `.pro` file:

```qmake
QMAKE_CXXFLAGS += -Wall -Wextra -Wno-deprecated-declarations -Wno-psabi -fsanitize=address -g
QMAKE_LFLAGS += -fsanitize=address
```

---

### 33. QMediaPlayer + GStreamer (NO NEED just attempt)

Update sources list:

```bash
deb http://deb.debian.org/debian bookworm-updates main
```

Then:

```bash
sudo apt update
sudo apt install libqt5multimedia5 libqt5multimediawidgets5 libqt5multimedia5-plugins
sudo apt install gstreamer1.0-qt5 qtmultimedia5-dev
sudo apt install libqt5gstreamer-1.0-0 libqt5gstreamer-dev
```

Add to service:

```bash
Environment="GST_PLUGIN_FEATURE_RANK=vpudec:256"
```

---

### 34. Touch All Files Before Rebuild (sometimes you can not compile the project)

```bash
find . -exec touch {} \;
```

---

### 35. WiFi Connection History (NO NEED just command)

```bash
nmcli connection show | grep wifi
```

### 36. Bulid the project

#### Directory rules

In this directory '/home/x_user/my_camera_project/'
must include the following:
Those directories
assets, assets/images, data, old_logs(empty folder), onnxruntime, onnxruntime/include, onnxruntime/lib, todo(empty folder), vosk-model-small-ar-0.3,
vosk-model-small-en-us-0.15, vosk-model-small-ru-0.22
Those filse
Audio.h, battery_log_book.csv (empty), camera_viewer.cpp, camera_viewer.h, camerareader.h, configuration_ap.json, Configuration.h, epo_parser.cpp,
epo_upload_demo.c, Errors.log(empty), FloatingMessage.h, FOLOG.log(empty), gpio.h, gps_config.cpp, gps_init.sh, gps_parser.cpp, HTTPSession.h,
imu_classifier_thread.h, langs.json, LanguageManager.h, libvosk.so, Logger.h, main.cpp, my_camera_project.pro, Outputs.log(empty), PDFCreator.h,
power_management.h, run_app.sh, speechThread.h, Timer.h, videocontroller.h, vosk_api.h, vpn_start_script.sh, wifi.json, WiFiManager.h

#### TEST Folder

In the directory '/home/x_user/my_camera_project/test/', there are files to test some classes from the main class.

And also there is the training process for man down.

#### GPS building

if you don't do those

```bash
g++ -std=c++17 -o epo_parser epo_parser.cpp -I/usr/local/include -L/usr/local/lib -lnmea
g++ -std=c++17 -pthread -o gps_config gps_config.cpp -lboost_system -lboost_thread
gcc -o epo_upload_demo epo_upload_demo.c (C version of epoloader)
g++ -std=c++17 -pthread gps_parser.cpp -o gps_parser -lboost_system -lboost_thread
```

YOU MUST DO THEM

#### PROJECT building

```bash
qmake my_camera_project.pro
make clean
make
```

### Some Notes

Better to use the Visual Studio code, to bulid and compile, bulid and test the appliaction first time.

install doc2unix if there are problem in the files

```
sudo apt install dos2unix
```

No need the md file, they are just Markdown files used for documentation of the classes or files.
