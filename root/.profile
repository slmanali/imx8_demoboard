# ~/.profile: executed by Bourne-compatible login shells.

if [ "$BASH" ]; then
  if [ -f ~/.bashrc ]; then
    . ~/.bashrc
  fi
fi

mesg n 2> /dev/null || true
export XDG_RUNTIME_DIR=/run/user/0
export WAYLAND_DISPLAY=wayland-1
export QT_QPA_PLATFORM=wayland
export GST_PLUGIN_PATH=/usr/lib/aarch64-linux-gnu/gstreamer-1.0/