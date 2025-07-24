# Documentation for IMX8 Setup and Configuration

This document serves as a comprehensive guide for configuring and managing a system based on the IMX8 architecture. It covers the installation of required tools, setup of network scanning services using `nmap`, custom time synchronization, image backup, and installation of Debian. Additionally, it involves GPIO configurations, video processing pipelines using GStreamer, and audio processing.

## 1. Prerequisites

Before proceeding, ensure your system meets the following requirements:

- IMX8 Hardware (e.g., Variscite DART-MX8M-PLUS)
- A working Debian-based operating system
- Sudo privileges

## 2. Installation of Required Packages

Ensure the necessary packages are installed. To install `ipcalc` and `nmap`, use the following commands:

```bash
sudo apt install ipcalc
sudo apt install nmap
```

## 3. Network Scanning Script (`startup_nmap_scan.sh`)

This script automates network scanning to find hosts that are up in the network.

### 3.1. Script Overview

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
    echo $IP
    echo $NETWORK
    result=$(sudo nmap -sP $NETWORK --open --min-hostgroup 100)
    echo "$result"
    
    if echo "$result" | grep -q "Host is up"; then
        echo "At least one host is up."
        return 0  # success
    else
        return 1  # failure
    fi
}

# Initial scan
perform_scan

# If no hosts were up, keep retrying
while [ $? -ne 0 ]; do
    echo "No hosts up, retrying in 10 seconds..."
    sleep 10
    perform_scan
done
```

#### 3.2. How It Works
 
- The script starts by checking if the interface `wlan0` has an assigned IPv4 address.
- Once connected, it defines a function `perform_scan` that:
    - Retrieves the IP and netmask for the current network interface.
    - Derives the network address using `ipcalc`.
    - Executes a network scan with `nmap` for active hosts.
- If no hosts are found, it continues to retry every 10 seconds until a host is detected.

#### 3.3. Setting Up the Service

To enable this script to run as a service, create a service file:

```bash
sudo nano /etc/systemd/system/nmap_scan.service
```

##### Contents of `nmap_scan.service`:

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

After saving the service file, execute the following commands:

```bash
sudo systemctl daemon-reload
sudo systemctl enable nmap_scan.service
sudo systemctl restart nmap_scan.service
sudo systemctl status nmap_scan.service
```

## 4. Custom Time Synchronization Service

### 4.1. Install NTP

```bash
sudo apt-get install ntp
sudo chmod +x /usr/sbin/ntpd
```

### 4.2. Service File for Time Synchronization

Create a service file for custom time synchronization:

```bash
sudo nano /etc/systemd/system/custom-timesync.service
```

##### Contents of `custom-timesync.service`:

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

After creating the file, reload the daemon and start the service:

```bash
sudo systemctl daemon-reload
sudo systemctl restart custom-timesync.service
sudo systemctl status custom-timesync.service
```

## 5. Image Backup using `tar`

### 5.1. Create a Backup

You can create a `.tar.gz` backup of the root file system while excluding unnecessary directories:

```bash
sudo tar -czf /root/tmp/debian_rootfs_backup_12_5_vcpp_3.tar.gz --one-file-system \
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

To transfer this backup to a remote server:

```bash
sudo tar -czf - --one-file-system \
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

## 6. GPIO Configuration

Example of setting GPIO pins:

```bash
gpio set 9
gpio set 131
...
gpio clear 70
```

## 7. Using GStreamer for Video Processing

Example pipeline to capture video and encode it:

```bash
gst-launch-1.0 imxv4l2videosrc device=/dev/video0 ! \
    videoconvert ! \
    video/x-raw,format=RGB8P ! \
    avenc_zmbv ! \
    avmux_avi name=mux ! \
    filesink location=output.avi
```

For motion detection, you can use:

```bash
gst-launch-1.0 -e imxv4l2videosrc device=/dev/video0 ! \
    videoconvert ! \
    videoscale ! \
    video/x-raw,width=640,height=480,framerate=15/1 ! \
    tee name=main_tee main_tee. ! queue ! \
    videodiff ! videoconvert ! video/x-raw,format=I420 ! \
    fakesink name=motion_sink signal-handoffs=true async=false main_tee. ! queue ! \
    imxvpuenc_h264 bitrate=2000 ! \
    h264parse ! \
    mp4mux ! \
    filesink location=motion_detected.mp4
```

## 8. Audio Processing with GStreamer

Example pipeline for audio processing:

```json
{
  "microphone_pipeline": "pulsesrc device=alsa_input.platform-sound-wm8904.stereo-fallback ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,rate=16000,channels=1 ! volume volume=5.0 ! pulsesink device=alsa_output.platform-sound-wm8904.stereo-fallback",
  "headphones_pipeline": "appsrc name=source format=time caps=audio/x-raw,format=S16LE,layout=interleaved,rate=16000,channels=1 ! queue ! audioconvert ! audioresample ! audio/x-raw,format=S16LE,rate=44100,channels=2 ! pulsesink device=alsa_output.platform-sound-wm8904.stereo-fallback"
}
```

## Conclusion

This document covers a wide range of steps and configurations tailored for the IMX8 platform. Following the instructions in this guide will help ensure your system is set up for networking, image processing, and audio functionality effectively. Adjust the configurations according to your specific application requirements.

---