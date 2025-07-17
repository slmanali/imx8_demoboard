# Documentation for `weston.ini`

The `weston.ini` file is a configuration file for Weston, the reference Wayland compositor. This file allows users to customize the behavior and appearance of the Weston environment. Below is a detailed breakdown of the configuration keys, their possible values, and the impact they have on the overall system.

## Sections of `weston.ini`

### [core]

- **gbm-format**
    - *Type:* String (optional)
    - *Description:* Determines the format of the graphics buffer manager (GBM). The default is typically set to `argb8888`.
    - *Example:* Uncommenting this line can set the format explicitly as `gbm-format=argb8888`.

- **use-g2d**
    - *Type:* Boolean
    - *Description:* Enables the use of G2D (Graphics 2D) acceleration which may enhance performance on certain hardware.
    - *Default:* `true`
  
- **repaint-window**
    - *Type:* Integer
    - *Description:* Specifies the time in milliseconds for the repaint window. This can help with managing how frequently the screen is redrawn.
    - *Default:* `16` (approximately 60 frames per second)

- **idle-time**
    - *Type:* Integer
    - *Description:* Time in seconds after which the compositor enters an idle state. Setting this to `0` disables idle timeout.
    - *Default:* `0`

- **xwayland**
    - *Type:* Boolean
    - *Description:* Enables XWayland, which allows X11 applications to run under Wayland.
    - *Default:* `true`

- **enable-overlay-view**
    - *Type:* Boolean (optional)
    - *Description:* Enables support for overlay views. Overlay views are surfaces that are rendered on top of other surfaces.
  
### [shell]

- **background-image**
    - *Type:* String
    - *Description:* The path to the background image to be used for the desktop environment.
    - *Example:* `background-image=/usr/share/images/desktop-base/default`
  
- **background-type**
    - *Type:* String
    - *Description:* Defines how the background image should be displayed. Options include `scale-crop`, `scale`, etc.
  
- **cursor-theme**
    - *Type:* String
    - *Description:* Specifies the cursor theme to use. Themes typically can customize the look and feel of the mouse cursor.
    - *Example:* `cursor-theme=Adwaita`
  
- **cursor-size**
    - *Type:* Integer
    - *Description:* Sets the size of the cursor in pixels. 
    - *Example:* `cursor-size=22`

### [output]

#### General Structure:

Each `[output]` section corresponds to a display output with configurable video settings.

- **name**
    - *Type:* String
    - *Description:* Specifies which output device to configure, such as monitors or screens.
  
- **mode**
    - *Type:* String
    - *Description:* Sets the display resolution and refresh rate. The format is typically `WIDTHxHEIGHT@REFRESH`.
    - *Example:* `mode=1920x1080@60`

- **mode=off**
    - *Description:* Disables the output. This can be used to turn off a monitor that is not being actively used.

- **preferred/current**
    - *Description:* Allows the compositor to utilize the preferred resolution of the display or the current mode the display is operating in.

- **transform**
    - *Type:* Integer (optional)
    - *Description:* Applies a transformation to the output, such as rotating the display. The value `90` corresponds to a 90-degree rotation.

### [screen-share]

- **command**
    - *Type:* String
    - *Description:* Defines the command to start a screen sharing session, specifically for Remote Desktop Protocol (RDP) backends.
    - *Example:* `command=/usr/bin/weston --backend=rdp-backend.so --shell=fullscreen-shell.so --no-clients-resize`

### [launcher]

The `[launcher]` sections define application launchers that appear in the Weston interface.

- **icon**
    - *Type:* String
    - *Description:* The path to the icon for the application launcher.
  
- **path**
    - *Type:* String
    - *Description:* Specifies the command to be executed when the application launcher is activated. This is often the full path to the executable or script.

## Example Launcher Configurations:

1. **Terminal Launcher**
    ```ini
    [launcher]
    icon=/usr/share/weston/terminal.png
    path=/usr/bin/weston-terminal
    ```

2. **Chrome Launcher**
    ```ini
    [launcher]
    icon=/usr/share/icons/hicolor/24x24/apps/chromium.png
    path=/usr/sbin/runuser -l weston -c chromium
    ```

## Conclusion

The `weston.ini` file is a powerful tool for customizing the Weston compositor environment. By properly configuring sections such as `[core]`, `[shell]`, `[output]`, `[screen-share]`, and `[launcher]`, users can tailor their experience to suit their hardware capabilities and personal preferences.
