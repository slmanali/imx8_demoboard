# Code Documentation for `epoloader.cpp`

## Overview

The `epoloader` is a command-line application designed to interact with GPS devices using EPO (Extended Prediction Orbit) data. It reads EPO files, sends the data to a specified GPS device, and optionally configures the GPS settings (such as time and location). Built using the Boost.Asio library, this application ensures efficient serial communication with the GPS device.

## Libraries Used

- `iostream`: For standard input and output.
- `fstream`: For file I/O operations.
- `sstream`: For manipulating strings as streams.
- `vector` and `string`: For using dynamic arrays and string manipulation.
- `cstring`: For C-style string operations.
- `ctime` and `chrono`: For time-related functionalities.
- `thread`, `mutex`, and `condition_variable`: For multithreading and synchronization.
- `atomic`: For atomic operations on shared variables.
- `stdexcept`: For standard exception handling.
- `boost/asio`: For asynchronous I/O operations.
- `boost/program_options`: For command-line argument handling.
- `boost/algorithm/string.hpp`: For string algorithms.
- POSIX functions (`fcntl`, `unistd`, `termios`): For direct handling of serial port attributes.

## Constants

```cpp
const uint16_t PREAMBLE = 0x2404;
const uint16_t EPO_CMD = 0x02d2;
const uint16_t EOW = 0x0a0d;
const uint8_t UART_CMD = 253;
const uint32_t SECONDS_PER_HOUR = 3600;
const uint32_t GPS_OFFSET_SECONDS = 315964786;
const uint32_t HOURS_PER_WEEK = 168;
```

These constants are used for configuring communication frames and GPS settings.

## Class: `EPOLoader`

### Description

The `EPOLoader` class encapsulates the logic for loading EPO data and communicating with the GPS device. The class manages the entire process from reading the EPO file to configuring the GPS device and sending the data.

### Constructor

```cpp
EPOLoader(const string& device, int speed, bool keep_new_speed, bool clear_epo, bool no_init,
          const string& input_file, const string& time_string, const string& location_string)
```
- **Parameters:**
  - `device`: The path to the GPS serial device.
  - `speed`: The baud rate for communication.
  - `keep_new_speed`: Flag to retain the new baud rate after operation.
  - `clear_epo`: Flag to indicate whether to clear existing EPO data.
  - `no_init`: Flag to skip initialization.
  - `input_file`: Path to the input EPO file (can also be `"-"` to skip).
  - `time_string`: Current UTC time string.
  - `location_string`: Current location string formatted as "lat,lon,alt".

### Public Methods

#### `int run()`

Initiates the loading process, including:
1. Opening and validating the EPO file.
2. Opening the serial port for communication.
3. Configuring the GPS device (time and location).
4. Sending EPO data to the GPS device.
5. Performing cleanup after completion.

#### Private Methods

- **`string Convert2UTC(int32_t GPSHour)`**
  - Converts the given GPS hour to a UTC string format.

- **`uint8_t crc8(const vector<uint8_t>& data)`**
  - Calculates and returns an 8-bit CRC for the provided data.

- **`bool open_epo_file()`**
  - Opens the specified EPO file, reads its header to determine the EPO type, and validates its size.

- **`void open_serial_port()`**
  - Opens the specified serial port and sets the baud rate and other communication settings.

- **`void start_receive_thread()`**
  - Starts a thread to listen for responses from the GPS device.

- **`void send_string(const string& str)`**
  - Sends a command string to the GPS device after appending necessary CRC and terminators.

- **`bool send_and_wait(const string& cmd, double timeout_sec = 1.0, int retries = 1)`**
  - Sends a command and waits for a response, allowing for a configurable number of retries.

- **`bool configure_gps()`**
  - Configures the GPS device with time, location, and possibly clears existing EPO data.

- **`string get_current_utc()`**
  - Retrieves the current UTC time in the specified string format.

- **`bool send_epo_data()`**
  - Reads EPO data from the input file and sends it to the GPS device.

- **`void cleanup()`**
  - Cleans up resources, stops threads, and restores the original serial port settings.

## Main Function

```cpp
int main(int argc, char* argv[])
```

The entry point of the program that:
- Parses command-line arguments using Boost.ProgramOptions.
- Initializes an `EPOLoader` instance with the provided parameters.
- Calls the `run` method to execute the loading process.
- Handles exceptions and outputs errors.

### Command-Line Options

- `--help, -h`: Displays usage information.
- `--speed, -s`: Sets the interface speed (default 115200).
- `--keep-new-speed, -k`: Keeps the new baud rate after execution.
- `--clear, -c`: Clears existing EPO data before loading new data.
- `--no-init, -n`: Skips any initialization steps when configuring GPS.
- `--time, -t`: Provides current UTC time or placeholder `-`.
- `--location, -l`: Specifies the current location in lat, lon, alt format.
- `--input_file`: Requires the path to the EPO file or `"-"` for no input file.
- `--output_device`: Requires the path to the GPS serial device.

## Example Usage

```bash
./epoloader -s 115200 -t 2023,10,15,12,00,00 -l 55.47199,37.54504166666667,180 MTK14.EPO /dev/ttymxc1
```

In this example, the program will set the GPS's baud rate to 115200, configure the time, load EPO data from `MTK14.EPO`, and send it to the GPS device located at `/dev/ttymxc1`.

## Conclusion

The `epoloader.cpp` application effectively manages the task of loading EPO data into GPS devices while providing options for GPS configuration. The implementation focuses on reliability and error handling during the communication process, ensuring smooth operation in various scenarios.
