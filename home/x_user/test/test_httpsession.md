# Code Documentation for `test_httpsession.cpp`

## Overview

The `test_httpsession.cpp` file is a test suite designed to validate the functionality of the `HTTPSession` class, as well as the configuration handling in the application. It encompasses the following key operations:

- Configuration setup
- Directory creation for testing
- Testing member functions of the `HTTPSession` class
- Cleanup of test artifacts

The code relies on a variety of external libraries, notably the CURL library for HTTPS operations, ZIP for file extraction, and JSONCpp for handling JSON files. The code is designed to be compiled with the GNU C++ compiler and requires C++17 features.

## Structure

### Includes

- `#include "Configuration.h"`: Includes the header for the `Configuration` class, which manages configurations for the application.
- `#include "HTTPSession.h"`: Includes the definition of the `HTTPSession` class, which is responsible for handling all HTTPS operations.
- `<iostream>`: Includes functionality for input and output streams, enabling the use of `std::cout` and `std::cerr`.
- `<cassert>`: Structure for C-style assertions to validate conditions during testing.
- `<fstream>`: Provides functionalities for file input and output, particularly for reading and writing files during tests.
- `<filesystem>`: Introduces file system operations, such as directory creation and file checking.

### Namespaces

- `namespace fs = std::filesystem;`: Simplifies the use of filesystem-related functions by creating an alias for `std::filesystem`.

## Functionality

### Main Function

```cpp
int main() {
    // Configuration setup
    ...
    // Create temporary directories
    ...
    // Initialize HTTPSession
    ...
    // Test standalone_request()
    ...
    // Test check_standalone()
    ...
    // Test Download_standalone_FILES()
    ...
    // Cleanup
    ...
    return 0;
}
```

#### Configuration Setup

This section defines important configuration parameters required to initialize the `HTTPSession`:

- `api_url`: The base URL for the API endpoints.
- `ssl_cert_path`: Path to the SSL certificate (not used here as SSL verification is disabled).
- `api_key`: The API key that authenticates the user's access.
- `helmet_status`: A status string indicating the mode of operation.

The `Configuration` object is created with the path to a JSON configuration file.

#### Directory Creation

Creates necessary temporary directories for testing using the `std::filesystem` library. It ensures that both the FTP folder and the directory specified in the configuration are available.

#### HTTPSession Initialization

```cpp
HTTPSession session(api_url, config.ssl_cert_path, config.api_key, "offline", config);
```

An instance of the `HTTPSession` class is created, passing the API URL, SSL certificate path, API key, connection status, and configuration object.

#### Testing `standalone_request`

The code then invokes the `standalone_request()` method to validate its functionality:

```cpp
session.standalone_request();
```

#### Testing `check_standalone`

The code contains two tests for the `check_standalone()` method that determines whether a specific directory exists or not:

- **Test 1**: Creates a directory using the API key and checks if `check_standalone()` returns `1`.
- **Test 2**: Deletes the previously created directory and checks that `check_standalone()` returns `0`.

```cpp
int result = session.check_standalone();
```

#### Testing `Download_standalone_FILES`

This section tests the `Download_standalone_FILES()` method to validate the download and extraction of files:

1. Creates a test directory and writes a sample file.
2. Clears any previous contents of the extraction directory specified in the config.
3. Invokes the download method.
4. Asserts that the file exists after extraction and verifies the content of the extracted file.

```cpp
bool file_exists = fs::exists(config.todo + "/file1.txt");
```

#### Cleanup

Finally, the code removes all the temporary directories and files created during the tests to ensure no artifacts remain:

```cpp
fs::remove_all(ftp_folder);
fs::remove_all(config.todo);
```

### Conclusion

The test suite verifies the essential functionalities of the `HTTPSession` class and configuration handling, ensuring that the system behaves as expected under various conditions. At the end of the execution, it confirms that all tests have passed successfully.

### Additional Compilation Instructions

This file can be compiled using the following command:

```bash
g++ -std=c++17 -o test_httpsession test_httpsession.cpp -lcurl -lzip -ljsoncpp
```

Ensure all dependencies are correctly linked to avoid compilation errors. This includes the CURL library for HTTPS communication, a ZIP library for handling compressed files, and JSONCpp for processing JSON content.

---