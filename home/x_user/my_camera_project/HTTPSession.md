# HTTPSession Class Documentation

The `HTTPSession` class is a C++ class that provides functionalities to manage HTTP sessions for a system, particularly for interacting with an API related to helmets. This class includes methods for authentication, status checking, and data management—using HTTP requests to send and receive information in both JSON and binary formats using the `libcurl` library for handling the HTTP requests.

## Include Guards
```cpp
#ifndef HTTPSESSION_H
#define HTTPSESSION_H
```
This prevents multiple inclusions of the header guard.

## Header Files
- `<iostream>`: Basic input-output stream library for logging and error messages.
- `<string>`: Provides the `std::string` class for handling strings.
- `<random>`: For generating random numbers (e.g., for generating query parameters).
- `<chrono>`: Time utilities for measuring time durations.
- `<thread>`: Enables multi-threading functionalities such as sleep.
- `<fstream>`: File stream classes for file input and output.
- `<nlohmann/json.hpp>`: A header for parsing JSON data.
- `<curl/curl.h>`: A library for making HTTP requests.
- `"Logger.h"`: Custom logging class (assumed to be defined elsewhere).
- `<cstring>`: Provides C-style string manipulation functions.
- `<net/if.h>`, `<sys/ioctl.h>`, `<unistd.h>`: System headers for network operations and IO control.
- `"Timer.h"`: Custom timer class for timing operations.
- `"Configuration.h"`: Custom configuration class for handling configuration settings.
- `<atomic>`, `<filesystem>`, `<zip.h>`: Used for atomic operations, working with filesystem paths and ZIP file manipulation, respectively.
- `<sstream>`: Stream class that operates on strings, particularly for parsing.
- `<mutex>`: Provides a mutex class for thread synchronization.

## Class Definition
```cpp
class HTTPSession {
```

### Constructor
```cpp
HTTPSession(const std::string& api_url, const std::string& ssl_cert_path, const std::string& api_key, std::string _helmet_status, Configuration& config)
```
- **Parameters**:
  - `api_url`: URL to the API endpoint.
  - `ssl_cert_path`: Path to the SSL certificate for secure connections.
  - `api_key`: API key for authentication.
  - `_helmet_status`: Initial helmet status.
  - `config`: A reference to a `Configuration` object containing application configuration.

The constructor initializes various member variables and invokes `curl_global_init()` to initialize the libcurl library.

### Destructor
```cpp
~HTTPSession()
```
Cleans up resources by stopping timers and calling `curl_global_cleanup()`.

### Public Member Functions

#### Authentication
```cpp
bool authenticate()
```
Attempts to authenticate with the server by sending a ping and checking for success. It retries up to 5 times with a one-second interval.

#### Obtain Certificate
```cpp
void get_certificate(const std::string& certificate_file)
```
Downloads a certificate from the API and saves it to the specified file.

#### Record Client IP Address
```cpp
void record_ip(const std::string& client_address)
```
Records the client's IP address by sending it to the API.

#### Check HTTP Availability
```cpp
bool is_http_available(const std::string& host)
```
Checks whether the HTTP service is available by sending a GET request to a specified host.

#### Check VPN Availability
```cpp
bool is_vpn_available()
```
Checks whether a VPN service is available by sending a GET request to a specific hardcoded IP address.

#### Update API URL
```cpp
void update_api_url(const std::string& _api_url)
```
Updates the API URL used in the session.

#### Update Helmet Status
```cpp
void update_helmet_status(const std::string _helmet_status)
```
Updates the helmet status in a thread-safe manner using a mutex.

#### Get Helmet Status
```cpp
std::string get_helmet_status()
```
Returns the current helmet status in a thread-safe manner.

#### Update Operator Status
```cpp
void set_operator_status(std::string _operator_status)
```
Sets the operator status.

#### Notifications Management
```cpp
void generate_notify()
void stop_notify()
```
Starts and stops timers for notifying status changes.

#### Record Event
```cpp
void record_event(const std::string& cmd, const std::string& data)
```
Records an event by sending a command and data pair to the API.

#### Update Event
```cpp
void update_event(int idCallEvent, std::string key, std::string value)
```
Updates an event with a specific ID by sending a key-value pair to the API.

#### Request Wi-Fi List
```cpp
void request_wifi()
```
Requests a list of Wi-Fi networks from the API.

#### Request Support
```cpp
void request_support()
```
Requests technical support from the API.

#### Standalone Request
```cpp
void standalone_request()
```
Sends a request to get the status of the helmet while in standalone mode.

#### Terminate Support
```cpp
void terminate_support()
```
Terminates support by sending an appropriate request to the API.

#### Handle Tasks
```cpp
void handle_tasks_next()
void handle_tasks_back()
void handle_tasks_reset()
```
Mechanisms for sending task control commands to the API.

#### Check Standalone Mode
```cpp
int check_standalone()
```
Checks if the service is available and responds appropriately based on the status code.

#### Download Standalone Files
```cpp
void Download_standalone_FILES()
```
Downloads a ZIP file and extracts it to a predefined location.

### Private Member Functions
- Various private methods exist for making HTTP POST and GET requests, handling responses, setting system time, reading GPS and temperature, etc.

### Data Members
- `std::string api_url`: Holds the base URL for the API.
- `std::string ssl_cert_path`: Path for the SSL certificate.
- `std::string api_key`: API key for authentication.
- `std::string helmet_status`: Current helmet status.
- `Configuration& config`: A reference to configuration settings.
- Other members assist with managing status, responses, and utilities like timers and JSON handling.

### Static Callback Functions
There are several functions defined to manage response actions from the `libcurl` callbacks used for receiving data generated during HTTP requests.

### Exception Handling
The class consistently uses try-catch blocks to handle potential exceptions, logging errors through the assumed `LOG_ERROR` and `LOG_INFO` methods provided by the `Logger` class.

### Additional Notes
The `HTTPSession` class encapsulates functionalities related to the helmet APIs and provides methods that follow a structured and consistent manner for interacting with the server. The extensive use of JSON for data transmission as well as thread-safe member functions suggests that it's designed for concurrent operations within a multi-threaded environment.

---
