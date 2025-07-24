# 🖥️ i.MX8 System Configuration & Debug Guide

This document provides a detailed summary of essential commands, configurations, and procedures for working with **i.MX8MP-based systems** (e.g., Variscite DART-MX8M-PLUS), including boot management, device tree manipulation, system services, GPIO control, imaging, and performance monitoring.

---

## 🔌 1. **Network & Connectivity**

### Install Network Tools
```bash
sudo apt install ipcalc
sudo apt install nmap
```

---

## 🌐 2. **Automated Nmap Scan on Network Up**

### `startup_nmap_scan.sh`
A script that waits for Wi-Fi connection and performs an `nmap` scan to detect active hosts.

```bash
#!/bin/bash
# Wait for the Internet connection on wlan0
while ! ip -4 addr show wlan0 | grep -q inet; do
    sleep 10  # Check every 10 seconds
done

# Function to perform nmap scan
perform_scan() {
    IP=$(ip -4 addr show wlan0 | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
    NETMASK=$(ip -4 addr show wlan0 | awk '/inet / {split($2, a, "/"); print a[2]}')
    NETWORK=$(ipcalc -n $IP/$NETMASK | awk '/Network:/ {print $2}')
    
    echo "IP: $IP"
    echo "Network: $NETWORK"
    
    result=$(sudo nmap -sP $NETWORK --open --min-hostgroup 100)
    echo "$result"

    if echo "$result" | grep -q "Host is up"; then
        echo "At least one host is up."
        return 0
    else
        return 1
    fi
}

# Initial scan
perform_scan

# Retry if no hosts found
while [ $? -ne 0 ]; do
    echo "No hosts up, retrying in 10 seconds..."
    sleep 10
    perform_scan
done
```

### Install as Systemd Service
```bash
sudo nano /etc/systemd/system/nmap_scan.service
```

```ini
[Unit]
Description=Run nmap scan after network is up
Requires=custom-timesync.service
After=network-online.target systemd-timesyncd

[Service]
Type=oneshot
ExecStart=/root/startup_nmap_scan.sh
RemainAfterExit=no

[Install]
WantedBy=multi-user.target
```

### Enable & Start Service
```bash
sudo systemctl daemon-reload
sudo systemctl enable nmap_scan.service
sudo systemctl restart nmap_scan.service
sudo systemctl status nmap_scan.service
journalctl -u nmap_scan.service
```

---

## ⏱️ 3. **Time Synchronization (NTP)**

### Install NTP
```bash
sudo apt-get install ntp
sudo chmod +x /usr/sbin/ntpd
```

### Create `custom-timesync.service`
```bash
sudo nano /etc/systemd/system/custom-timesync.service
```

```ini
[Unit]
Description=Custom time synchronization
After=network-online.target
Wants=network-online.target

[Service]
ExecStartPre=/bin/sh -c '/usr/bin/killall -q ntpd || true'
ExecStart=/usr/sbin/ntpd -gN
TimeoutStartSec=10
Type=oneshot
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
```

### Manage Service
```bash
sudo systemctl daemon-reload
sudo systemctl restart custom-timesync.service
sudo systemctl status custom-timesync.service
sudo journalctl -u custom-timesync.service -f
sudo journalctl -u systemd-timesyncd.service -b
```

---

## 💾 4. **Imaging & Backup**

### Backup Boot Partition
```bash
sudo dd if=/dev/mmcblk2boot0 of=/root/tmp/imx-boot-sd.bin bs=512 count=2048
```

> 💡 Use `skip=1` to skip first sector if needed.

### Inspect Boot Image
```bash
binwalk /root/tmp/imx-boot-sd.bin
```

---

## 📦 5. **Root Filesystem Backup & Restore**

### Create Compressed RootFS Backup
```bash
sudo tar -czf /root/tmp/debian_rootfs_backup_12_5_vcpp_3.tar.gz \
  --one-file-system \
  --exclude=/root/tmp/* \
  --exclude=/proc/* \
  --exclude=/sys/* \
  --exclude=/dev/* \
  --exclude=/run/* \
  --exclude=/mnt/* \
  --exclude=/media/* \
  --exclude=/tmp/* \
  /
```

### Stream Backup Over SSH
```bash
sudo tar -czf - \
  --one-file-system \
  --exclude=/root/tmp/* \
  --exclude=/home/x_user/client/voice_recognition/* \
  --exclude=/proc/* \
  --exclude=/sys/* \
  --exclude=/dev/* \
  --exclude=/run/* \
  --exclude=/mnt/* \
  --exclude=/media/* \
  --exclude=/tmp/* \
  / | ssh ubuntu@109.95.85.186 'cat > debian_rootfs_backup_12_5.tar.gz'
```

### Restore Image
```bash
mkdir /root/tmp
mount /dev/sda /root/tmp
```

> Then extract `.tar.gz` to the mounted location.

---

## 🛠️ 6. **U-Boot & Boot Process**

### Common U-Boot Errors
```
Failed to load '/boot/boot.scr'
Failed to load '/boot/uEnv.txt'
```
→ Ensure boot partition has correct files and permissions.

### U-Boot Environment Commands
```bash
gpio set 9; gpio set 131; gpio set 154; gpio set 155; gpio set 21; gpio clear 70; gpio set 73; gpio set 86; gpio set 88; gpio set 89; gpio set 120; run bsp_bootcmd
```

> Used to manually set GPIOs and boot from eMMC.

---

## 🌳 7. **Device Tree (DTS/DTB) Manipulation**

### Convert DTB to DTS
```bash
dtc -I dtb -O dts -o imx8_kernel5_default.dts /boot/imx8mp-var-dart-dt8mcustomboard.dtb
```

### Convert DTS to DTB
```bash
dtc -I dts -O dtb -o /boot/imx8mp-var-dart-dt8mcustomboard3.dtb imx8_kernel6_helmet3.dts
```

### Useful Conversions
```bash
dtc -I dtb -O dts -o imx8_kernel6_helmet.dts /boot/imx8mp-var-dart-dt8mcustomboard3.dtb
dtc -I dts -O dtb -o /boot/imx8mp-var-dart-dt8mcustomboard3.dtb imx8_kernel5_default.dts
dtc -I dtb -O dts -o symphony_ov5640.dts /boot/imx8mp-var-som-1.x-symphony-2nd-ov5640.dtb
```

---

## 📹 8. **GStreamer: Video Recording & Motion Detection**

### Record Video to AVI (ZMBV)
```bash
gst-launch-1.0 imxv4l2videosrc device=/dev/video0 ! \
  videoconvert ! \
  video/x-raw,format=RGB8P ! \
  avenc_zmbv ! \
  avimux ! \
  filesink location=output.avi
```

### Motion Detection Pipeline
```bash
gst-launch-1.0 -e imxv4l2videosrc device=/dev/video0 ! \
  videoconvert ! \
  videoscale ! \
  video/x-raw,width=640,height=480,framerate=15/1 ! \
  tee name=main_tee \
  main_tee. ! queue ! videodiff ! videoconvert ! video/x-raw,format=I420 ! \
    videorate ! video/x-raw,framerate=1/1 ! \
    fakesink name=motion_sink signal-handoffs=true async=false \
  main_tee. ! queue ! imxvpuenc_h264 bitrate=2000 ! \
    h264parse ! mp4mux ! \
    filesink location=motion_detected.mp4
```

### Motion Detection with Contrast Enhancement
```bash
gst-launch-1.0 -e imxv4l2videosrc device=/dev/video0 ! \
  videoconvert ! \
  videoscale ! \
  video/x-raw,width=640,height=480,framerate=15/1 ! \
  tee name=main_tee \
  main_tee. ! queue ! videodiff ! videoconvert ! video/x-raw,format=I420 ! \
    videobalance contrast=2.0 ! videorate ! video/x-raw,framerate=1/1 ! \
    fakesink name=motion_sink signal-handoffs=true async=false \
  main_tee. ! queue ! imxvpuenc_h264 bitrate=2000 ! \
    h264parse ! mp4mux ! \
    filesink location=motion_detected.mp4
```

---

## 🔧 9. **Compilation & Development**

### Compile OpenCV App
```bash
g++ -o camera_c camera_c.cpp $(pkg-config --cflags --libs opencv4)
```

### Compile GTKMM + GStreamerMM App
```bash
g++ -o gtk_c gtk_c.cpp `pkg-config --cflags --libs gtkmm-3.0 gstreamermm-1.0`
```

### Install PyQt5 (Specific Version)
```bash
sudo pip3 install PyQt5==5.15.6 PyQt5-sip==12.9.0 PyQt5-Qt5==5.15.2
```

### Install QML Modules
```bash
sudo apt-get install qml-module-qtquick-controls qml-module-qtquick2
```

### Set PKG_CONFIG Path
```bash
export PKG_CONFIG_PATH=/usr/lib/pkgconfig:/usr/lib/aarch64-linux-gnu/pkgconfig:$PKG_CONFIG_PATH
```

### Compile with GPIO, JSON, Audio, HTTP
```bash
g++ -o main main.cpp -lgpiod                    # GPIO
g++ -o main main.cpp -I/usr/include/jsoncpp -ljsoncpp  # JSON
g++ -o main main.cpp `pkg-config --cflags --libs gstreamer-1.0`  # GStreamer
g++ -o main main.cpp -lcurl                    # HTTP/CURL
```

---

## 🖥️ 10. **Environment Setup for GUI & GStreamer**

### Launch App with EGLFS (No X11)
```bash
sudo XDG_RUNTIME_DIR=/run/user/0 \
DISPLAY=:0 \
QT_QPA_PLATFORM=eglfs \
GST_PLUGIN_PATH=/usr/lib/aarch64-linux-gnu/gstreamer-1.0/ \
/home/x_user/my_camera_project/my_camera_project
```

---

## 🔊 11. **Audio Pipelines (PulseAudio)**

### Microphone to Speaker Loop
```bash
pulsesrc device=alsa_input.platform-sound-wm8904.stereo-fallback ! \
  audioconvert ! \
  audioresample ! \
  audio/x-raw,format=S16LE,rate=16000,channels=1 ! \
  volume volume=5.0 ! \
  pulsesink device=alsa_output.platform-sound-wm8904.stereo-fallback
```

### AppSrc to Headphones
```json
{
  "headphones_pipeline": "appsrc name=source format=time caps=audio/x-raw,format=S16LE,layout=interleaved,rate=16000,channels=1 ! queue ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,rate=44100,channels=2 ! pulsesink device=alsa_output.platform-sound-wm8904.stereo-fallback"
}
```

---

## 🐍 12. **Performance Monitoring**

### Install `py-spy` (Python Profiler)
```bash
pip install py-spy
py-spy top --pid <PID>
```

### Use `perf` for System Profiling
```bash
sudo perf top --sort comm,dso
```

---

## 📂 13. **Kernel Source Setup**

### Extract Kernel Source
```bash
tar -xvf linux-source-5.10.tar.xz -C /usr/src/
```

### Clone i.MX Linux Kernel
```bash
git clone https://github.com/varigit/linux-imx.git
```

---

## 📁 14. **System Diagnostics**

### Check Disk Usage
```bash
du -h --max-depth=1 /
```

### Flash Full Image to eMMC
```bash
dd if=/root/tmp/dart-mx8m-plus-image-20241203.img.gz of=/dev/mmcblk2 bs=4M
```

---

## ✅ Tips & Notes

- Use `--open` with `nmap` to show only hosts with open ports.
- Always exclude `/proc`, `/sys`, `/dev` when backing up rootfs.
- Device Tree edits are critical — test in a backup environment.
- Use `imxv4l2videosrc` instead of `v4l2src` for better i.MX8 integration.
- If `Device or resource busy` occurs on I2C (`0x3c`), power-cycle camera via GPIO.
- `videodiff` element is useful for motion detection.
- Use `splitmuxsink` or `mp4mux` for reliable recording.

---