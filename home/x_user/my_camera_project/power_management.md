# Power Management Module Documentation

The `PowerManagement` class is designed to handle various aspects of power management for a system, particularly focusing on battery status monitoring and management. This documentation provides a detailed overview of the class, its methods, and its usage.

## Header Guards

```cpp
#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H
```
This prevents multiple inclusions of the header file, ensuring that the compiler processes it only once.

## Includes

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <jsoncpp/json/json.h>
#include <chrono>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <csignal>
#include <cmath>
#include <atomic>
#include <functional>
#include "Logger.h"
#include "Configuration.h"
#include "Timer.h"
```
This file includes various standard and external libraries to provide functionality for file handling, I2C communication, timing, and logging. Key dependencies include JSON for configuration management and `Logger.h` for logging events.

## Class Definition

### `PowerManagement`

The `PowerManagement` class manages the power system, particularly battery status monitoring.

#### Constructors and Destructor

- **Constructor**: 
  ```cpp
  PowerManagement(Configuration& _config);
  ```
  Initializes the `PowerManagement` instance with a configuration object.
  
  **Parameters**:
  - `_config`: A reference to a `Configuration` object that contains various settings for the power management functionalities.

  During initialization, it reads the last log entry to increment the current counter for logging purposes. If reading log data fails, it logs an error.

- **Destructor**: 
  ```cpp
  ~PowerManagement();
  ```
  Stops any ongoing battery status updates by calling `stop_updates()`.

#### Public Methods

- **set_battery_status_callback**:
  ```cpp
  void set_battery_status_callback(const std::function<void(BatteryStatus)>& callback);
  ```
  Sets a callback function that will be triggered whenever the battery status changes.

- **start_updates**:
  ```cpp
  void start_updates();
  ```
  Starts periodic updates for battery status using a timer. The update interval is determined by `config.READING_DELAY`.

- **stop_updates**:
  ```cpp
  void stop_updates();
  ```
  Stops the periodic battery status updates.

#### Private Members

The core of the class is composed of various helper methods and member variables that facilitate battery monitoring.

- **BatteryStatus Enum**:
  Represents the different battery states: `RED`, `YELLOW`, `GREEN`, and `CRITICAL`.

- **Member Variables**:
  - `Configuration& config`: Reference to the configuration settings.
  - `std::atomic<bool> stop_thread`: Flag to signal updates to stop.
  - `int REPEAT_INIT`: Number of repetitions for average readings.
  - `int current_counter`: Counter for each log entry.
  - `std::chrono::steady_clock::time_point last_log_time`: Timestamp of the last log entry.
  - `Timer timer`: Timer instance used for periodic updates.
  - `std::function<void(BatteryStatus)> battery_status_callback`: Callback function for battery status changes.

#### Private Methods

- **battery_status_to_string**:
  Converts the `BatteryStatus` enum to a string representation.
  
- **read_adc**:
  Reads the ADC (Analog-to-Digital Converter) value via I2C.
  - **Parameters**:
    - `bool smooth`: Determines if the read value should be smoothed (averaged over several readings).
  - **Returns**: 
    - The reading from the ADC.

- **calculate_battery_level**:
  Calculates the actual battery voltage based on ADC reading.
  
- **calculate_battery_percentage**:
  Computes the battery percentage based on defined threshold values.

- **read_battery**:
  Obtains the battery level and percentage by performing the ADC reading multiple times to get an average.

- **check_battery_status**:
  Periodically checks the battery status and logs information. Additionally, it triggers the battery status callback if there are changes in the battery state.

- **shutdown_board**:
  Initiates a shutdown process for the system in the case of low battery voltage. It performs the following actions:
  1. Displays a low battery warning image.
  2. Plays an alert sound.
  3. Blinks an LED connected to a specified GPIO pin.
  4. Executes a system shutdown command.

## Usage Example

Here's an example of how to use the `PowerManagement` class:

```cpp
#include "power_management.h"

int main() {
    Configuration config; // Assume this is properly initialized
    PowerManagement pm(config);

    pm.set_battery_status_callback([](PowerManagement::BatteryStatus status) {
        std::cout << "Battery Status Changed: " << static_cast<int>(status) << std::endl;
    });

    pm.start_updates();
    
    // Run application code...

    pm.stop_updates();
    return 0;
}
```

### Conclusion

The `PowerManagement` class is a crucial component for monitoring and managing battery power in a system. It provides methods to read battery levels, calculate percentages, log the status, and handle shutdown procedures safely. Proper use of this class allows for effective monitoring of battery health and proactive management of low power situations.

---
