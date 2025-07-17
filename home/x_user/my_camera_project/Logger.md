# Logger.h Documentation

## Overview

`Logger.h` is a header file that defines a logging utility class, `Logger`, for C++ applications. This class allows developers to log messages with different severity levels: `INFO`, `ERROR`, and `WARN`. The logs are written to a specified log file in a formatted manner which includes the timestamp, message severity level, and the source code location (file name, function name, and line number).

### Key Features:

- **Severity Levels:** Logs can be categorized into INFO, ERROR, and WARN.
- **Timestamping:** Each log entry includes a precise timestamp, down to the millisecond.
- **File Logging:** Appends log messages to a log file located at a specific path: `/home/x_user/my_camera_project/FOLOG.log`.
- **Macro Support:** Provides macros for ease of use, automatically capturing file name, function name, and line number.

## Class Documentation

### Logger

#### Public Methods

- **static void logInfoImpl(const std::string &message, const char* file, const char* function, int line)**  
  Logs an informational message. This method captures the provided message along with the originating file, function, and line number, and passes it to the `log` method with the severity level "INFO".

  **Parameters:**
  - `message`: The message to log, of type `std::string`.
  - `file`: The name of the source file, of type `const char*`.
  - `function`: The name of the calling function, of type `const char*`.
  - `line`: The line number where the log is invoked, of type `int`.

- **static void logErrorImpl(const std::string &message, const char* file, const char* function, int line)**  
  Logs an error message. Similar to `logInfoImpl`, but with the severity level "ERROR".

- **static void logWarnImpl(const std::string &message, const char* file, const char* function, int line)**  
  Logs a warning message. Similar to `logInfoImpl`, but with the severity level "WARN".

#### Private Methods

- **static void log(const std::string &level, const std::string &message, const char* file, const char* function, int line)**  
  Handles the actual logging logic. It formats the log entry with a timestamp, the severity level, and the source location before writing it to the log file.

  **Parameters:**
  - `level`: The log level (e.g., "INFO", "ERROR", "WARN"), of type `std::string`.
  - `message`: The message to log, of type `std::string`.
  - `file`: The name of the source file, of type `const char*`.
  - `function`: The name of the calling function, of type `const char*`.
  - `line`: The line number where the log is invoked, of type `int`.

## Macros

- **LOG_INFO(msg)**  
  A convenience macro that allows logging of informational messages. It captures the `message` and automatically uses the current file, function name, and line number.

- **LOG_ERROR(msg)**  
  A convenience macro that allows logging of error messages, similar to `LOG_INFO`.

- **LOG_WARN(msg)**  
  A convenience macro that allows logging of warning messages, similar to `LOG_INFO`.

### Example Usage

```cpp
#include "Logger.h"

void functionA() {
    LOG_INFO("This is an informational message.");
    LOG_WARN("This is a warning message.");
}

void functionB() {
    LOG_ERROR("An error occurred.");
}
```

In the example above:
- Calling `LOG_INFO` will log an informational message.
- `LOG_WARN` will log a warning.
- `LOG_ERROR` will log an error. Each log will include a timestamp, the severity level, and the location from where the log function was called.

### Implementation Notes

1. **File Path Configuration:** 
   The log file path is hardcoded in the `log` function. It may be beneficial to make this configurable via a constructor or setter method in the `Logger` class.

2. **Thread Safety:**
   The current implementation does not guarantee thread safety. If this logger will be used in a multithreaded context, consider adding mechanisms such as mutexes to protect file access.

3. **Error Handling:**
   The implementation does not handle cases where the log file cannot be opened. You may want to add error handling logic to manage such scenarios and possibly log this error elsewhere (e.g., stderr).

### Conclusion
The `Logger.h` file provides a robust and straightforward logging utility for C++ applications. It helps maintain code clarity and assists in debugging and monitoring application behavior through structured log messages.

---
