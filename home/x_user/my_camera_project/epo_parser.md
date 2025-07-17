# Code Documentation for `epo_parser.cpp`

## Overview
The `epo_parser.cpp` program is a C++ utility designed for parsing and extracting information from EPO (Ephemeris) files. These files contain GPS satellite data and the program interprets this data to display the associated GPS week and hour in UTC format. The program supports two types of EPO files, identified by their headers.

## Header Files
```cpp
#include <iostream>         // Provides input/output functionalities
#include <fstream>         // For file handling (reading/writing)
#include <iomanip>         // For output manipulation (e.g., formatting)
#include <ctime>           // For date/time functions
#include <vector>          // For using dynamic arrays (vectors)
#include <cstring>         // For handling C-style strings and memory
#include <cstdint>         // For using fixed-width integer types
```

## Constants
```cpp
constexpr uint32_t SECONDS_PER_HOUR = 3600;          // Number of seconds in an hour
constexpr uint32_t GPS_OFFSET_SECONDS = 315964786;    // Offset to convert GPS time to UTC
constexpr uint32_t HOURS_PER_WEEK = 168;               // Total number of hours in a week
```

## Functions
### `std::string Convert2UTC(int32_t GPSHour)`
Converts a GPS hour into a UTC time string.

- **Parameters**: 
  - `GPSHour` (int32_t): The GPS hour to convert.
  
- **Returns**: 
  - A string representing the UTC time formatted as "YYYY-MM-DD HH:MM:SS".

#### Implementation Details
1. Converts `GPSHour` to `time_t` by multiplying with `SECONDS_PER_HOUR` and adding `GPS_OFFSET_SECONDS`.
2. Uses the `gmtime` function to convert `time_t` into a `tm` structure representing UTC.
3. Formats the `tm` structure into a string buffer using `strftime`.

### `int main(int argc, char* argv[])`
Main entry point of the program. It handles file reading and data processing.

- **Parameters**:
  - `argc` (int): The number of command line arguments.
  - `argv` (char*[]): An array of C-style strings representing command line arguments.

- **Returns**:
  - An exit status code (1 for errors, 0 for success).

#### Implementation Details
1. **Argument Handling**: Checks if a single command line argument (input file name) is provided.
2. **File Handling**: Opens the specified binary file. If unsuccessful, it outputs an error message and exits.
3. **Header Reading**:
    - Reads the first 75 bytes to determine the EPO file type (Type I or Type II).
    - Uses `memcmp` to compare relevant portions of the header.
4. **EPO Set Processing**: 
    - Initializes variables for processing each set of data.
    - Reads EPO sets in a loop until the end of the file is reached:
        - Extracts the GPS hour from the first three bytes of the EPO set using little-endian order.
        - Sign-extends the 24-bit value to a 32-bit integer.
        - Calculates the GPS week and hour within the week.
        - Outputs formatted information for each set.
5. **Summary Output**: After processing all sets, it prints a summary indicating the number of EPO sets read and their valid date ranges.

### Example Output
The output consists of messages regarding:
- The type of EPO file being processed.
- Details of each EPO set including:
  - EPO set number
  - GPS week and hour
  - Corresponding UTC time range
- A summary at the end showing the total number of EPO sets and their valid date range in UTC.

### Compilation Instructions
To compile the program, use a C++ compiler, for example:
```sh
g++ epo_parser.cpp -o epo_parser
```

### Usage
Run the program via the command line, providing the path to the EPO file as an argument:
```sh
./epo_parser <EPO_File>
```

### Error Handling
The program includes error handling for:
- Incorrect usage without an EPO file argument.
- File reading errors.
- Unrecognized EPO file types.

## Conclusion
The `epo_parser.cpp` program efficiently reads EPO files, processes the satellite data, and outputs meaningful information to the user, while handling common errors during file operations and argument processing.
