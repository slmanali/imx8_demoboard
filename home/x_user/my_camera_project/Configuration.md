# Configuration.h - Detailed Code Documentation

The `Configuration.h` file defines a configuration management system that interfaces with a JSON configuration file to load and manage various settings essential for an application. The configuration is encapsulated in the `Configuration` class which utilizes the `IMUConfig` structure to handle IMU specific configurations. 

## Prerequisites
- This implementation relies on the **JsonCpp** library for parsing JSON files. Ensure that the library is installed:
  ```
  sudo apt-get install libjsoncpp-dev
  ```
- Compile the source code using:
  ```
  g++ -o main main.cpp -ljsoncpp
  ```

## File Overview
The file begins with include guards to prevent multiple inclusions and proceeds to include various headers necessary for input/output operations, string manipulations, error handling, and JSON parsing. 

```cpp
#ifndef CONFIGURATION_H
#define CONFIGURATION_H
```

### Included Libraries:
- `<iostream>`, `<fstream>`, `<sstream>`, `<string>`, `<map>`: Standard C++ libraries for input/output, file handling, strings, and associative containers.
- `"/usr/include/jsoncpp/json/json.h"`: JsonCpp library header for JSON parsing.
- `<stdexcept>`, `<algorithm>`: Standard exceptions for error handling and utility functions.
- `"Logger.h"`: Custom header for logging functionality (assumed to be defined elsewhere).
- `<regex>`: Regular expressions (system-wide).

### Struct: IMUConfig
The `IMUConfig` struct holds parameters related to the Inertial Measurement Unit (IMU):

```cpp
struct IMUConfig {
    std::string imu_model_path;
    std::string i2c_device;
    int i2c_addr;
    int WHO_AM_I;
    int CTRL1;
    int ON_CTRL1;
    int OUT_X_L, OUT_X_H, OUT_Y_L, OUT_Y_H, OUT_Z_L, OUT_Z_H;
};
```
**Members:**
- `imu_model_path`: File path for the IMU model.
- `i2c_device`: The I2C device identifier.
- `i2c_addr`: Address of the I2C device.
- `WHO_AM_I`, `CTRL1`, `ON_CTRL1`, etc.: Configuration registers related to the IMU.

### Class: Configuration
The `Configuration` class encapsulates all configuration settings necessary for the application and provides methods to manipulate these settings.

```cpp
class Configuration {
```

#### Public Members
1. **File Paths and Networking**
   - Various string variables to hold file paths, API keys, and server configurations.

2. **IMU Configuration**
   - An instance of `IMUConfig` for specific IMU settings.

3. **Audio Settings**
   - Ports and pipeline strings for managing audio input and output.

4. **Video Settings**
   - Various settings for capturing and processing video such as resolution, bitrate, and streaming configurations.

5. **System Parameters**
   - Parameters like voltage thresholds, debug flags, and handling working intervals for power management.

#### Constructor: Configuration
The constructor accepts a file path to the configuration JSON file, reads it, and initializes the respective member variables.

```cpp
Configuration(const std::string &path);
```

**Error Handling:**
- Throws a `runtime_error` if the file fails to open or if there are issues parsing the JSON.

**Member Initialization:**
- Within the constructor, member variables are initialized by parsing the JSON content.
- The `replacePlaceholder` function is used extensively to customize pipeline strings, accommodating variables such as width, height, and framerate.

#### Methods

1. **replacePlaceholder**
   ```cpp
   std::string replacePlaceholder(const std::string &str, const std::string &placeholder, const std::string &value);
   ```
   - Replaces all occurrences of a specified placeholder in a string with the given value.
   - Catches and logs errors during replacement.

2. **updateAudioOutcoming**
   ```cpp
   void updateAudioOutcoming(const std::string &addr);
   ```
   - Updates the audio outcoming pipeline string with a new server address by replacing the placeholder.

3. **updateApiUri**
   ```cpp
   void updateApiUri(const std::string &uri);
   ```
   - Updates the API URI in the configuration by only modifying the IP part of the URI while preserving other components.

4. **updatestreaming**
   ```cpp
   void updatestreaming(int _bitrate, int _fps, int _width, int _height);
   ```
   - Updates streaming parameters like bitrate, fps, width, and height while revitalizing the streaming pipeline string based on the updated values.

5. **updateDefaultLanguage**
   ```cpp
   void updateDefaultLanguage(const std::string& new_language);
   ```
   - Updates the default language within the configuration file and the class member variable.
   - Utilizes private helper functions to read/write the configuration file.

6. **readFileToString**
   ```cpp
   std::string readFileToString(const std::string& path);
   ```
   - Reads the contents of a file into a string for further manipulation or analysis.

7. **getNumberMappings**
   ```cpp
   const std::map<std::string, int>& getNumberMappings() const;
   ```
   - Provides access to the number mappings from the configuration.

### Conclusion
The `Configuration.h` file constitutes a robust framework for managing application settings through JSON, facilitating dynamic updates and organized structure for configuration management. It handles various application parameters ranging from networking and audio to video and system-specific settings, exemplifying encapsulation and functionality in C++ programming.

---