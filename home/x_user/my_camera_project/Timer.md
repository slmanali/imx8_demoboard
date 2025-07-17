# Timer Class Documentation

The `Timer` class is a utility for executing a callback function at specified intervals in a separate thread. It allows periodic execution of a function while providing thread-safety and automatic cleanup of resources when no longer needed.

## Header File: Timer.h

### Includes
- `<iostream>`: Provides input and output stream functionalities.
- `<thread>`: Provides functionalities for multithreading.
- `<functional>`: Provides functionalities for defining and using function objects (callbacks).
- `<atomic>`: Provides atomic variable functionalities for safe parallel operations.
- `<chrono>`: Provides functionalities for manipulating time durations.

### Class Definition
```cpp
class Timer {
    ...
};
```

### Public Member Functions

#### 1. Constructor
```cpp
Timer();
```
- **Description**: Initializes a `Timer` object. The timer is not running upon initialization.

#### 2. `start`
```cpp
void start(int interval_ms, int _type, std::function<void()> callback);
```
- **Parameters**:
  - `int interval_ms`: The time interval for the timer in milliseconds.
  - `int _type`: The type of timer. This influences the duration between callbacks:
    - **0**: The timer runs at the specified interval (full duration).
    - **2**: The timer runs at a quarter of the specified interval.
  - `std::function<void()> callback`: A callable function that will be executed on each timer iteration.
  
- **Description**: Starts the timer in a separate thread. It first calls `stop()` to ensure any previously running timer is halted before beginning the new one. The method runs a loop which sleeps for the specified `interval_ms` before calling the callback function, provided that the timer is still running.

- **Exceptions**: Catches exceptions that may arise during the thread creation, logging an error message if an exception occurs.

#### 3. `stop`
```cpp
void stop();
```
- **Description**: Stops the timer if it is currently running. This sets the `running` flag to false, causing the loop in the timer thread to exit. It waits for the timer thread to conclude its execution using `join()`.

- **Exceptions**: Catches exceptions that may arise during stopping the timer, logging an error message if one occurs.

#### 4. Destructor
```cpp
~Timer();
```
- **Description**: The destructor stops the timer, ensuring no resources are leaked and the timer thread is joined before the object is destructed.

### Private Member Variables
- `std::thread timer_thread`: Represents the thread on which the timer callbacks execute.
- `std::atomic<bool> running`: A thread-safe boolean flag that indicates whether the timer is currently running.

## Example Usage

Here’s an example of how to use the `Timer` class:

```cpp
#include "Timer.h"

void myCallback() {
    std::cout << "Timer executed" << std::endl;
}

int main() {
    Timer timer;
    timer.start(1000, 0, myCallback); // Execute myCallback every 1 second

    std::this_thread::sleep_for(std::chrono::seconds(5)); // Main thread sleeps for 5 seconds

    timer.stop(); // Stop the timer before exiting
    return 0;
}
```

### Notes
- This class assumes that the callback function provided by the user is thread-safe, as it will be invoked by the timer thread.
- The `_type` parameter provides a mechanism to handle different timing behaviors; other types could be added as needed for more complex scheduling.
- It is strongly recommended to ensure that the callback function is light and non-blocking to avoid delaying the timer's scheduling.

---
