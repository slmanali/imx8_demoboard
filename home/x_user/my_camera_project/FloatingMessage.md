# FloatingMessage Class Documentation

The `FloatingMessage` class is a custom Qt widget designed to display floating messages or images on the screen. This widget is frameless, transparent, and can stay on top of other windows while being non-intrusive. 

## Header File: FloatingMessage.h

The following sections provide an overview of the class and its methods, member variables, and their functionality.

### Include Guards

```cpp
#ifndef FLOATINGMESSAGE_H
#define FLOATINGMESSAGE_H
```
These preprocessor directives prevent multiple inclusions of the header file, which can lead to redefinition errors.

### Includes

```cpp
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
```
Imports the necessary Qt modules:
- `QWidget`: Base class for all UI objects.
- `QLabel`: Widget that displays text or images.
- `QTimer`: Provides a way to execute code at specific time intervals.
- `QVBoxLayout`: Layout manager that arranges child widgets vertically.

### Class Declaration

```cpp
class FloatingMessage : public QWidget {
    Q_OBJECT
```
The `FloatingMessage` class inherits from `QWidget`. It uses the `Q_OBJECT` macro, which is necessary for classes that define or use signals and slots.

### Constructor

```cpp
explicit FloatingMessage(QWidget *parent = nullptr)
```
- **Parameters**: 
  - `parent`: A pointer to the parent widget. If `nullptr`, the widget operates as a top-level window.
- **Initialization**:
  - The widget is created without a frame and set to stay on top of all other windows.
  - Attributes for translucent background and showing without activation are set.
  - A `QLabel` is initialized to display text or images.
  - A `QVBoxLayout` is used to manage the layout, ensuring the label is centered.
  - Timer is initialized as a single-shot timer to automatically hide the message after a specified duration.
  - The widget starts with a minimum size and is hidden by default.

### Member Function: `showMessage`

```cpp
void showMessage(const QString& message, int type = 0)
```
#### Description:
Displays a message or an image based on the `type` parameter.

#### Parameters:
- `message`: The message to be displayed. Can be a text string or the path to an image file.
- `type`: An integer to specify the type of message:
  - `0`: Standard text message, appears at the top-right corner.
  - `1`: Centered text message.
  - `2`: Centered image message.
  - Other values display the image directly.

#### Logic:
- If `currenttype` is `1` (default), it checks if the `type` is less than `3`:
  - Sets text and adjusts position according to the specified `type`.
  - Triggers the timer for hiding the message after 2 or 5 seconds based on the message type.
- If `type` is `3` or higher, it treats the input as an image, scales it to a specified size, sets it to the label, and centers it in the parent widget.

### Member Function: `timer_stop`

```cpp
void timer_stop()
```
#### Description:
Immediately starts the timer with a 1 ms interval and resets `currenttype` to `1`, allowing for the message to be displayed continuously until explicitly hidden.

### Private Members

```cpp
private:
    QLabel *label;
    QTimer *timer;
    int currenttype = 1;
```
- **label**: Points to the `QLabel` that displays the message or image.
- **timer**: Manages timing for how long the message remains visible.
- **currenttype**: An integer to track the current type of displayed message (default is `1`).

### Style and Appearance

The widget's appearance is controlled through stylesheets. The `QLabel` has specific styling to make the text red and the background transparent for a floating effect.

### Usage

To use the `FloatingMessage` class, create an instance of it, and call the `showMessage` method with the desired message and type. The widget will automatically position itself based on the parent widget's geometry and will hide after a specified duration.

## Example Code Snippet

```cpp
FloatingMessage *floatingMessage = new FloatingMessage(parentWidget);
floatingMessage->showMessage("Hello, World!", 0);
```

This code snippet creates a new `FloatingMessage` and displays the text "Hello, World!" at the top-right position of its parent widget. The message will disappear after 2 seconds.

### Conclusion

The `FloatingMessage` class is a versatile and customizable widget ideal for notifications, alerts, or any floating message interface in a Qt application. The use of signals, timers, and layouts allows for robust behavior with minimal code configuration required by the user.

---
