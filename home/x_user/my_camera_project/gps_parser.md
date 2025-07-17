# Detailed Documentation of `gps_parser.cpp`

## Overview

The `gps_parser.cpp` program is designed to read, parse, and log Global Positioning System (GPS) and Global Navigation Satellite System (GLONASS) data from a serial port. It processes NMEA sentences, particularly GGA, GLL, GPGSV, and GLGSV, to extract navigational data and satellite information.

### Key Features:
- Parse NMEA sentences to extract GPS/GLONASS coordinates and satellite details.
- Log details into a CSV and log file.
- Manage multi-threaded access to shared resources using mutex locks.
- Uses Boost.Asio for asynchronous serial port communication.

## Global Variables

- `mutex data_mutex`: Ensures thread safety when accessing shared data.
- `map<int, int> gps_satellites_available`: Stores the GPS satellites' information (PRN and their Signal-to-Noise Ratio).
- `map<int, int> glonass_satellites_available`: Stores the GLONASS satellites' information (PRN and their Signal-to-Noise Ratio).
- `string status`: Holds the current status of the GPS data validity.
- `string mode`: Holds the current mode of operation (e.g., Autonomous, DGPS).
- Paths for CSV and log files are defined as const strings.

## Constants

- `const string CSV_PATH`: Path to the CSV file where GPS coordinates are stored.
- `const string LOG_PATH`: Path to the log file where detailed logs are recorded.
- `const bool LOGGING`: Toggle to enable or disable logging functionality.

## Functions

### `double average(const map<int, int>& m)`

Calculates the average value of the given map.

- **Parameters**: 
    - `m`: A map containing integer values.
- **Returns**: Average value of the integers in the map as a double.

### `int max_value(const map<int, int>& m)`

Finds the maximum value in the given map.

- **Parameters**: 
    - `m`: A map containing integer values.
- **Returns**: Maximum integer value found in the map.

### `vector<int> sorted_values_desc(const map<int, int>& m)`

Sorts the values of the map in descending order.

- **Parameters**: 
    - `m`: A map containing integer values.
- **Returns**: A vector containing sorted values in descending order.

### `void parse_gga(const vector<string>& tokens)`

Parses the GGA (Global Positioning System Fix Data) NMEA sentence.

- **Parameters**: 
    - `tokens`: A vector of strings containing the tokenized GGA sentence.
- **Functionality**: Calculates the geographic coordinates and logs them into a CSV file and a detailed log if logging is enabled.

### `void parse_gll(const vector<string>& tokens)`

Parses the GLL (Geographic Position - Latitude/Longitude) NMEA sentence.

- **Parameters**: 
    - `tokens`: A vector of strings containing the tokenized GLL sentence.
- **Functionality**: Updates the status and mode of GPS based on the parsed information.

### `void parse_gpgsv(const vector<string>& tokens)`

Parses the GPS satellite information from GPGSV NMEA sentences.

- **Parameters**: 
    - `tokens`: A vector of strings containing the tokenized GPGSV sentence.
- **Functionality**: Updates the `gps_satellites_available` map with the new satellite information and ensures thread safety through a mutex lock.

### `void parse_satellite_data(const vector<string>& tokens, map<int, int>& satellites_map, const string& system_name)`

Generalized function to parse satellite data for both GPS and GLONASS systems.

- **Parameters**: 
    - `tokens`: A vector of strings containing the tokenized satellite data sentence.
    - `satellites_map`: Reference to the map that will store the satellites' SNR data.
    - `system_name`: A string to identify the satellite system (either "GPS" or "GLONASS").
- **Functionality**: Updates the specified satellite map with the satellite information while ensuring thread safety.

### `void parse_glgsa(const vector<string>& tokens)`

A specific parser for the GLGSV (GLONASS satellite information), similar to `parse_gpgsv`.

- **Parameters**: 
    - `tokens`: A vector of strings containing the tokenized GLGSV sentence.
- **Functionality**: Updates the `glonass_satellites_available` map.

### `vector<string> tokenize(const string& s)`

Tokenizes a string into substrings based on a delimiter (comma).

- **Parameters**: 
    - `s`: The string to tokenize.
- **Returns**: A vector of substrings that were separated by commas.

### `void receive_loop()`

The main loop function that reads data from the serial port.

- **Functionality**:
    - Continuously reads incoming data, tokenizes it, and delegates parsing to appropriate NMEA sentence handler functions.
    - Continues functioning until an error occurs or the program is terminated.

### `int main()`

Entry point of the program.

- **Functionality**:
    - Opens a serial port for reading data from the GPS/GLONASS device.
    - Starts a background thread that runs the `receive_loop` to process incoming data.
    - Keeps the main thread alive to allow continuous reading of serial data until an exception occurs.

## Compilation

The program can be compiled using the following command:

```bash
g++ -std=c++17 -pthread gps_parser.cpp -o gps_parser -lboost_system -lboost_thread
```

### Dependencies

- Boost Libraries (specifically Boost.Asio) for serial port communication.

## Usage

Upon execution, the program opens a specified serial port. It logs GPS and GLONASS data into designated files and prints error messages to the console if any issues are encountered during serial communication.

## Example Logging Output

The logs will be organized in a `gps.log` file and contain details formatted as follows:

```
Time: [time]
Coordinates: [latitude], [longitude]
Used satellites: [number of satellites]
Status: [status]
Mode: [mode]
GPS      [number of satellites], AVG SNR [average SNR], MAX SNR [maximum SNR], SNR [list of SNRs]
GLONASS  [number of satellites], AVG SNR [average SNR], MAX SNR [maximum SNR], SNR [list of SNRs]
```

## Conclusion

This documentation provides a comprehensive overview of `gps_parser.cpp`, explaining its structure, functionality, and how to use the program effectively. The modular design allows for easy additions and modifications for further enhancements in handling satellite data.
