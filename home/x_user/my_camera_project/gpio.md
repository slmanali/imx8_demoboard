# GPIOThread Class Documentation

## Overview
The `GPIOThread` class is designed to facilitate the management of GPIO lines in a threaded environment, specifically for the purpose of monitoring button presses. It utilizes the `gpiod` library to interact with GPIO lines and encompasses threading capabilities for continuous monitoring. When a button press is detected, a user-defined callback function can be executed.

## Header Guard
```cpp
#ifndef GPIO_H
#define GPIO_H
```

Prevents multiple inclusions of the header file.

## Includes
```cpp
#include <iostream>
#include <thread>
#include <chrono>
#include <gpiod.h> 
#include <atomic>
#include "Logger.h"
#include <functional>
```
- `iostream`: For input and output stream functionality.
- `thread`: To manage threads for asynchronous operations.
- `chrono`: For time-related functions, typically to handle delays.
- `gpiod.h`: A library that provides functions for GPIO line control.
- `atomic`: For managing atomic operations, specifically for stopping the thread safely.
- `Logger.h`: Custom logging functionalities to capture errors.
- `functional`: To use `std::function` for callbacks.

## Class Definition
```cpp
class GPIOThread {
```
The `GPIOThread` class encapsulates all the properties and methods associated with GPIO line monitoring in a separate thread.

### Constructor
```cpp
GPIOThread(const std::string &chip_name, unsigned int line_number);
```
**Parameters:**
- `chip_name`: String representing the name of the GPIO chip (e.g., "gpiochip0").
- `line_number`: An unsigned integer specifying the GPIO line number to use.

The constructor attempts to open the specified GPIO chip and request the GPIO line as an input. If any operation fails, it logs the error and throws a `std::runtime_error`.

### Public Member Functions

#### `void start()`
Starts the thread that runs the GPIO monitoring loop. The monitoring loop continually checks the GPIO line for changes in the input value. If an exception occurs during thread creation, it logs the error.

#### `void stop()`
Stops the GPIO monitoring thread safely by setting a flag and joining the thread if it's joinable. Any exceptions during the stopping process are logged.

#### `void set_button_pressed_callback(const std::function<void()> &callback)`
Sets the callback function that will be invoked when a button press is detected. The callback is of type `std::function<void()>`, allowing any callable object (like a lambda or a function pointer) that matches the signature.

### Destructor
```cpp
~GPIOThread();
```
Cleans up by stopping the thread, releasing the GPIO line, and closing the GPIO chip. Any exceptions during destruction are logged.

### Private Member Variables
- `std::string chip_name`: Stores the name of the GPIO chip.
- `unsigned int line_number`: Stores the GPIO line number.
- `struct gpiod_chip *chip`: Pointer to the open GPIO chip.
- `struct gpiod_line *line`: Pointer to the specific GPIO line.
- `std::thread thread`: Thread object for executing the GPIO monitoring loop.
- `std::atomic<bool> stop_thread`: Atomic boolean to signal when to stop the monitoring thread.
- `std::function<void()> button_pressed_callback`: Callback function to be invoked on button presses.

### Private Member Function

#### `void run()`
The core function that the thread executes. It continuously polls the state of the GPIO line every 100 milliseconds. If a change is detected (specifically, if the line transitions to low, indicating a button press if active-low), it checks if the callback is set and executes it. Any errors encountered during value reading are logged.

```cpp
while (!stop_thread) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Polling interval
    int value = gpiod_line_get_value(line);
    if (value < 0) {
        LOG_ERROR("Failed to read GPIO value");
        continue;
    }
    if (last_value != value && value == 0) { // Assuming active low button
        if (button_pressed_callback) {
            button_pressed_callback();
        }
    }
    last_value = value;
}
```

## Error Handling
Throughout the class methods, the logging utility (`LOG_ERROR`) is called to log any errors that occur. This captures failures in GPIO chip access or thread control and provides stack trace clarity for debugging.

## Conclusion
The `GPIOThread` class provides a structured way to monitor GPIO input lines in a dedicated thread while allowing for custom callback handling upon specific input transitions. It effectively encapsulates GPIO logic, error handling, and thread management, making it a versatile tool for applications requiring GPIO interaction.

---
