# 📚 Command Summary Documentation

This document provides a categorized summary of essential commands used in development, debugging, networking, GStreamer, system management, and more — based on real-world usage from the provided log.

---

## 🔧 1. **Wi-Fi & Network Configuration**

### Connect to Wi-Fi using `nmcli`

```bash
sudo nmcli device wifi connect "SSID" password "PASSWORD"
```

**Examples:**

```bash
sudo nmcli device wifi connect "TP-Link_71C0_5G" password "30963501"
sudo nmcli device wifi connect "TP-Link_1912_5G" password "34773970"
sudo nmcli device wifi connect "SH_LP_NUC_office" password "12345678"
```

### Manual Wi-Fi setup with `wpa_supplicant`

Generate config:

```bash
wpa_passphrase "SSID" "PASSWORD" > /etc/wpa_supplicant.conf
```

Start `wpa_supplicant`:

```bash
wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf
```

Or specify driver explicitly:

```bash
wpa_supplicant -Dnl80211 -iwlan0 -c /etc/wpa_supplicant.conf -B
```

### Stop/Start NetworkManager

```bash
sudo systemctl stop NetworkManager
sudo systemctl start NetworkManager
sudo systemctl enable NetworkManager
```

### Renew IP address

```bash
dhclient -r wlan0  # Release
dhclient -v wlan0  # Renew
```

### Scan available networks

```bash
iw wlan0 scan
```

---

## 📺 2. **GStreamer Commands (Video/Audio Streaming)**

### Video: Stream from camera to UDP (H.264)

```bash
gst-launch-1.0 v4l2src device=/dev/video3 ! \
video/x-raw,format=YUY2,width=1280,height=720 ! \
imxvideoconvert_g2d ! \
vpuenc_h264 bitrate=5000 ! \
rtph264pay config-interval=3 mtu=1400 ! \
udpsink host=192.168.1.145 port=7002
```

### Video: Stream with high bitrate and advanced encoding

```bash
gst-launch-1.0 v4l2src device=/dev/video3 ! \
video/x-raw,width=1280,height=720 ! \
videoconvert ! \
vpuenc_h264 bitrate=8000 gop-size=15 qp-min=10 qp-max=40 ! \
rtph264pay config-interval=3 mtu=1400 aggregate-mode=zero-latency ! \
udpsink host=192.168.1.145 port=7009
```

### Video: Receive and display H.264 stream

```bash
gst-launch-1.0 udpsrc port=7002 caps="application/x-rtp,media=video,encoding-name=H264,payload=96" ! \
rtpjitterbuffer ! \
rtph264depay ! \
h264parse ! \
avdec_h264 ! \
videoconvert ! \
autovideosink
```

### Video: Capture and save snapshots from stream

```bash
gst-launch-1.0 udpsrc uri=udp://localhost:7002 caps="application/x-rtp,media=video,clock-rate=90000,encoding-name=H264,payload=96" ! \
rtpjitterbuffer ! \
rtph264depay ! \
h264parse ! \
queue ! \
imxvpudec ! \
tee name=t \
t. ! queue ! videoconvert ! imxeglvivsink fullscreen=True \
t. ! queue ! videoconvert ! jpegenc ! multifilesink location="snapshot-%05d.jpeg" sync=false
```

### Video: Stream using imxv4l2videosrc (i.MX platform)

```bash
gst-launch-1.0 imxv4l2videosrc device=/dev/video0 ! \
video/x-raw,width=640,height=480 ! \
videoconvert ! \
imxvpuenc_h264 bitrate=5000 ! \
rtph264pay config-interval=3 mtu=1400 ! \
udpsink host=192.168.1.145 port=7002
```

### Audio: Stream microphone via OPUS

```bash
alsasrc device=hw:1,0 ! \
opusenc complexity=0 frame-size=60 bitrate=32000 ! \
rtpopuspay ! \
udpsink host=192.168.1.116 port=5001
```

### Audio: Stream with volume control and resampling

```bash
alsasrc device=hw:0,0 ! \
volume volume=1 ! \
audioconvert ! \
audioresample ! \
audio/x-raw,channels=1,rate=8000 ! \
opusenc complexity=0 frame-size=60 bitrate=32000 ! \
rtpopuspay ! \
udpsink host=192.168.0.123 port=6000
```

### Audio: Receive and play OPUS stream

```bash
udpsrc port=5001 caps="application/x-rtp,encoding-name=OPUS,clock-rate=48000" ! \
rtpopusdepay ! \
opusdec ! \
audioconvert ! \
autoaudiosink
```

### Audio: Loopback test

```bash
gst-launch-1.0 alsasrc ! audioconvert ! audioresample ! autoaudiosink
```

### Audio: Test source with encoding

```bash
gst-launch-1.0 audiotestsrc ! \
audioconvert ! \
opusenc ! \
rtpopuspay ! \
udpsink host=192.168.0.123 port=6000
```

---

## 🔍 3. **System & Hardware Debugging**

### Check CPU frequency and governor

```bash
cat /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
```

### Set CPU to performance mode

```bash
sudo cpupower frequency-set -g performance
```

### List V4L2 devices

```bash
v4l2-ctl --list-devices
```

### Get camera info

```bash
v4l2-ctl -d /dev/video3 --info
```

### Set camera parameters

```bash
v4l2-ctl -d /dev/video3 --set-fmt-video=width=1280,height=720,pixelformat=YUYV
v4l2-ctl -d /dev/video3 --set-parm=30  # 30 FPS
```

### Control camera sub-device (e.g., OV5640)

```bash
v4l2-ctl -d /dev/v4l-subdev2 --set-ctrl=contrast=0
v4l2-ctl -d /dev/v4l-subdev2 --set-ctrl=red_balance=2000
v4l2-ctl -d /dev/v4l-subdev2 --list-ctrls
```

---

## 🔌 4. **GPIO Control**

### Control GPIO pins using `gpioset`

```bash
gpioset gpiochip2 6=1    # Power on camera
gpioset gpiochip2 6=0    # Power off
gpioset gpiochip2 9=0    # Assert reset
gpioset gpiochip2 9=1    # Release reset
```

### Camera power/reset sequence

```bash
gpioset gpiochip2 6=1; sleep 0.1; gpioset gpiochip2 6=0  # Power cycle
gpioset gpiochip2 9=0; sleep 0.1; gpioset gpiochip2 9=1  # Reset
```

---

## 🗃️ 5. **File & Image Backup**

### Create compressed disk image

```bash
sudo dd if=/dev/mmcblk2 bs=4M | gzip > /root/IMX8.img.gz
```

### Restore from compressed image

```bash
gunzip -c /root/tmp/imx6_8_29_24.img.gz | dd of=/dev/mmcblk2
```

### Copy image over SSH

```bash
sudo dd if=/dev/mmcblk2 bs=4M | gzip -c | ssh ubuntu@109.95.85.186 'cat > imx8_image_15_07_2024.img.gz'
```

### Check disk usage

```bash
du -h --max-depth=1 /
```

---

## 🔄 6. **Service & System Management**

### Manage systemd service

```bash
sudo systemctl stop helmet8cpp.service
sudo systemctl restart helmet8cpp.service
sudo systemctl daemon-reload
```

### View service logs

```bash
sudo journalctl -u helmet8cpp.service -f
sudo journalctl -u sh_client.service -f
sudo journalctl -u helmet8cpp.service --since "2024-11-05 10:00:00"
sudo journalctl -u helmet8cpp.service --priority=err
```

### Clean up journal logs

```bash
journalctl --vacuum-time=10d    # Keep last 10 days
journalctl --vacuum-size=100M   # Limit to 100MB
```

---

## 🖥️ 7. **Cross-Compilation & SDK Setup**

### Sync sysroot from target device

```bash
rsync -avz root@192.168.1.123:/usr/include/ /cygdrive/c/Users/user/Desktop/mx8_image_file/DH/sysroot/include
rsync -avz root@192.168.1.123:/usr/lib/ /cygdrive/c/Users/user/Desktop/mx8_image_file/DH/sysroot/lib
rsync -avz root@192.168.1.123:/lib/ /cygdrive/c/Users/user/Desktop/mx8_image_file/DH/sysroot/lib
rsync -avz root@192.168.1.123:/usr/local/ /cygdrive/c/Users/user/Desktop/mx8_image_file/DH/sysroot/local
```

### Add cross-compiler to PATH

```bash
export PATH=$PATH:/cygdrive/c/Users/user/Desktop/mx8_image_file/gcc-linaro-5.5.0-2017.10-x86_64_aarch64-linux-gnu/bin
```

---

## 🐞 8. **Debugging & Development Tools**

### Compile with OpenCV

```bash
g++ main.cpp -o gst_opencv_capture $(pkg-config --cflags --libs opencv4)
```

### Compile GStreamer + Vosk example

```bash
g++ vosk_gstreamer_example.cpp -o vosk_gstreamer_example \
-lgstapp-1.0 -lgstreamer-1.0 -ljsoncpp -l/home/x_user/my_camera_project/libvosk.so
```

### Set environment variables for Qt/Wayland

```bash
export QT_QPA_PLATFORM=wayland
export XDG_RUNTIME_DIR=/run/user/0
export WAYLAND_DISPLAY=wayland-1
os.environ['QT_OPENGL'] = "es2"
```

### GDB Debugging Shortcuts

| Command                 | Description        |
| ----------------------- | ------------------ |
| `c` or `continue`   | Continue execution |
| `n` or `next`       | Step over          |
| `s` or `step`       | Step into          |
| `bt` or `backtrace` | Show call stack    |
| `print var`           | Print variable     |

---

## 📁 9. **Remote File Transfer**

### Copy files via SCP

```bash
scp -r /path/to/source/folder user@target-device-ip:/path/to/destination
```

---

## 🧹 10. **Miscellaneous Useful Commands**

### Kill all instances of a process

```bash
sudo killall wpa_supplicant
```

### Run Python Gunicorn server

```bash
sudo systemctl stop smarthelmet.service 
cd /home/ubuntu/Server/
/home/ubuntu/venv/bin/python3 /home/ubuntu/venv/bin/gunicorn --bind 0.0.0.0:5000 wsgi --workers 1
```

### Test audio source

```bash
gst-launch-1.0 audiotestsrc ! audioconvert ! autoaudiosink
```

### Set environment for plugin path

```bash
export GST_PLUGIN_PATH=/usr/lib/aarch64-linux-gnu/gstreamer-1.0/
```

### Generate ARPA language model (KenLM)

```bash
echo "WORD LIST HERE" > corpus.txt
./bin/lmplz -o 3 < corpus.txt > lm.arpa
```

---

## ✅ Tips & Notes

- Use `-v` flag in `gst-launch-1.0` for verbose output.
- Always ensure camera is powered and reset properly before streaming.
- When `Device or resource busy` occurs on I2C (`0x3c`), camera may be locked — try power cycling via GPIO.
- For stable video streaming, use `imxvpuenc_h264` and `imxvpudec` on i.MX8 platforms.
- Avoid using `optimize_full` and `-O3` when debugging with sanitizers.

---
