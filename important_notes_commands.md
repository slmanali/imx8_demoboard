# General Documentation for Commands and Scripts

This document provides an overview and explanation of various commands and scripts used for networking, media processing, and system management on a Linux environment, particularly around interfacing with WiFi, GStreamer for media handling, and some system services.

## WiFi Connection Commands

### Connect to WiFi Networks

These commands utilize `nmcli`, the command-line interface for NetworkManager, to connect to various WiFi networks:

- ```bash
  sudo nmcli device wifi connect "TP-Link_71C0_5G" password "30963501"
  ```
  Connects to the 5GHz TP-Link network with the given password.

- ```bash
  sudo nmcli device wifi connect "TP-Link_71C0" password "30963501"
  ```
  Connects to the 2.4GHz TP-Link network.

- ```bash
  sudo nmcli device wifi connect "SH_LP_NUC_office" password "12345678"
  ```
  Connects to a different network with the specified password.

### Alternative Connection Method

- ```bash
  sudo /opt/wifi_connect.sh wlan0 TP-Link_71C0 30963501
  ```
  This script connects to the specified WiFi network using interface `wlan0`.

### WPA Supplicant for WiFi Authentication

- ```bash
  wpa_supplicant -B -i wlan0 -c <(wpa_passphrase "TP-Link_71C0" "30963501")
  ```
  This command starts `wpa_supplicant` to manage and authenticate the WiFi connection without relying on NetworkManager.

- ```bash
  sudo wpa_passphrase "SH_LP_NUC_UZ" "12345678" > /etc/wpa_supplicant.conf
  ```
  Generates a configuration file for `wpa_supplicant` containing the specified network and password.

### WiFi Configuration Details Example

- ```plaintext
  ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
  update_config=1

  network={
      ssid="TP-Link_71C0_5G"
      psk="30963501"
  }
  ```
  This is an example of a `wpa_supplicant` configuration file format.

## GStreamer Commands

GStreamer is a powerful framework for building media-handling components. Below are command examples for streaming and processing audio and video.

### Streaming Video

To stream video from a camera:

- ```bash
  gst-launch-1.0 v4l2src device=/dev/video1 ! videoconvert ! autovideosink
  ```
  Captures video from the device `/dev/video1`, converts it, and displays it.

### Streaming Over UDP

- ```bash
  gst-launch-1.0 v4l2src device=/dev/video0 ! videoconvert ! vpuenc_h264 bitrate=5000 ! rtph264pay ! udpsink host=192.168.1.145 port=7002
  ```
  Streams video encoded in H264 format over UDP to the specified host and port.

### Receiving Video

To receive and play video streamed over UDP:

- ```bash
  gst-launch-1.0 udpsrc port=7002 caps="application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink
  ```
  Listens on UDP port `7002`, receives H264 video, and plays it back.

### Processing Audio

Stream audio using GStreamer:

- ```bash
  gst-launch-1.0 pulsesrc device=alsa_input.platform-sound-wm8904.stereo-fallback ! audio/x-raw, channels=1 ! audioconvert ! audioresample ! autoaudiosink
  ```
  Captures audio from a specified input device and plays it back.

## System Management Commands

### Managing Services

Here are some commands for managing systemd services:

- **Start, Stop, and Check Status of Services:**
  ```bash
  sudo systemctl stop <service_name>
  sudo systemctl start <service_name>
  sudo systemctl status <service_name>
  ```

- **Enable Services on Boot:**
  ```bash
  sudo systemctl enable <service_name>
  ```

- **View Logs for Services:**
  ```bash
  sudo journalctl -u <service_name> -f  # Follow logs in real-time
  ```

### Backup Commands

For backing up system images:

- ```bash
  sudo dd if=/dev/mmcblk2 of=/root/tmp/backup_imageIMX8.img bs=4M status=progress
  ```
  This command creates a backup image of the specified block device.

- ```bash
  sudo dd if=/dev/mmcblk2p1 bs=4M | gzip > /root/IMX8.img.gz
  ```
  This command backs up a specific partition and compresses it.

## Configuration and Environment Variables

### Network Manager Configuration

Ensure `wpa_supplicant` is enabled:

- ```bash
  sudo systemctl enable wpa_supplicant
  ```

### Setting Up the Environment for Qt Applications

Configure environment variables for Qt applications:

- ```bash
  export QT_QPA_PLATFORM=wayland
  ```

This sets the Qt platform for Wayland, required for graphics applications.

## Notes on Command Usage

- Always ensure you verify WiFi passwords, network names, and device paths before executing commands.
- Use the appropriate permissions (e.g., `sudo`) where necessary, especially with system-level commands.
- For GStreamer, ensure the necessary plugins are installed to avoid dependency issues.

## Conclusion

This document serves as a guide to some essential commands and configurations used in a Linux environment for networking, media processing, and service management. Always proceed with caution when executing commands that alter network settings or manage system services.

---