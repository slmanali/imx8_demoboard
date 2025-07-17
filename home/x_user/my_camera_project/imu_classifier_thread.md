# IMUClassifierThread Class Documentation

The `IMUClassifierThread` class encapsulates functionality for interfacing with an Inertial Measurement Unit (IMU) sensor, performing data classification using a pre-trained model with ONNX (Open Neural Network Exchange). The class manages I2C communication with the sensor, collects accelerometer data, computes features for classification, and invokes an inference engine to classify the data streams.

## Header File: `imu_classifier_thread.h`

### Includes
The class includes several necessary headers:
- Standard Libraries: `<queue>`, `<mutex>`, `<condition_variable>`, `<atomic>`, `<vector>`, `<string>`, `<iostream>`, `<functional>`.
- ONNX Runtime API: `<onnxruntime_cxx_api.h>`.
- Custom Headers: `"Configuration.h"`, `"Timer.h"`.
- Linux I2C Libraries: `<fcntl.h>`, `<unistd.h>`, `<sys/ioctl.h>`, `<linux/i2c-dev.h>` for hardware I2C communication.

### Public Methods

#### `IMUClassifierThread(const IMUConfig& imu_config)`
The constructor initializes an `IMUClassifierThread` object with the provided IMU configuration.

- **Parameters:**
  - `imu_config`: A constant reference to an `IMUConfig` object containing the configuration settings for the IMU.

#### `int init()`
Initializes the ONNX runtime session and sets up I2C communication with the IMU sensor.
- **Returns:** 
  - 0 on success.
  - 1 if initialization fails (either ONNX or I2C-related issues).

#### `void start_IMU(int period_ms)`
Starts a timer that triggers the continuous reading of IMU data at the specified interval.
- **Parameters:**
  - `period_ms`: The period (in milliseconds) for reading the IMU data.

#### `void stop()`
Stops the timer and clears the internal data window.

#### `void setResultCallback(std::function<void(const QString)> callback)`
Sets a callback function to handle the result of the classification.
- **Parameters:**
  - `callback`: A function that will be called with the classification result as a `QString`.

### Private Members

- `IMUConfig imu_config_`: Configuration settings for the IMU.
- `Ort::Env env_`: ONNX runtime environment.
- `Ort::Session session`: Active session for running inference on the model.
- `std::vector<float> features_`: Stores computed features.
- `std::mutex features_mutex_`: Mutex for thread-safe access to features.
- `Timer timer`: Timer object to manage periodic tasks.
- `std::function<void(const QString)> result_callback`: Callback for delivering classification results.
- `std::string input_name`: Name of the model's input tensor.
- `std::string output_name`: Name of the model's output tensor.
- `std::deque<std::array<float, 3>> window_`: Circular buffer for collecting accelerometer readings.
- `ONNXTensorElementDataType output_type_`: The type of the output tensor from the model.
- `std::mutex window_mutex_`: Mutex for safe access to data in the window.
- `std::condition_variable window_cv_`: Condition variable to synchronize processing of window data.
- `bool ready`: Indicates if data is ready for processing by the classifier.
- `int fd`: File descriptor for the I2C device.

### Private Methods

#### `int read_reg(int fd, int reg)`
Reads a byte from a specified register of the IMU sensor over I2C.
- **Parameters:**
  - `fd`: File descriptor for the opened I2C device.
  - `reg`: The register address to read from.
- **Returns:** 
  - The byte read from the register, or `-1` on failure.

#### `void write_reg(int fd, int reg, int value)`
Writes a byte to a specified register of the IMU sensor over I2C.
- **Parameters:**
  - `fd`: File descriptor for the opened I2C device.
  - `reg`: The register address to write to.
  - `value`: The value to write.

#### `int initialize_sensor(int fd)`
Initializes the IMU sensor and checks device identity.
- **Parameters:**
  - `fd`: File descriptor for the opened I2C device.
- **Returns:**
  - 0 on success.
  - 1 if initialization fails (e.g., wrong device).

#### `void read_accel(int fd, int16_t &x, int16_t &y, int16_t &z)`
Reads accelerometer data from the IMU sensor.
- **Parameters:**
  - `fd`: File descriptor for the opened I2C device.
  - `x`, `y`, `z`: References to store the read values.

#### `void ContinuousRead()`
Continuously reads accelerometer data until `ready` is set to `true`. This method is intended to run on a separate thread.

#### `void ContinuousReadOnce()`
Reads accelerometer data `WINDOW_SIZE` times, storing it in a temporary buffer, and then updates the main data window for processing.

#### `void CaptureIMU()`
Processes the collected accelerometer data, computes features, and performs inference using the ONNX model. If a classification result is obtained, it invokes the result callback.

#### `std::vector<float> compute_features(const std::deque<std::array<float, 3>>& win)`
Computes statistical features from a given window of accelerometer data.
- **Parameters:**
  - `win`: A deque containing the samples from the IMU sensor.
- **Returns:** 
  - A vector of computed features which includes means, variances, energies, and correlation coefficients of the accelerometer data across three axes (x, y, z).

### Constants

- `static constexpr size_t WINDOW_SIZE`: Constants defining the size of the buffer window for storing IMU readings (set to 180 samples).

### Usage
This class is intended to be used in real-time applications where IMU data is continuously collected and analyzed, such as activity recognition systems. It relies on a valid configuration and a compatible ONNX model for classification tasks.

### Error Handling
The class handles various errors, such as failed initialization of the ONNX session, issues with I2C communication, and unexpected output shapes or types during inference. It logs errors using the `LOG_ERROR` macro and provides feedback on the system's state.

### Multi-threading Considerations
To preserve data integrity, the class employs mutexes for synchronizing access to shared resources (`window_` and `features_`). The methods that modify these resources are protected with appropriate locks.

---

This documentation captures the purpose, functionality, and usage of the `IMUClassifierThread` class, ensuring that developers have a comprehensive understanding of its structure and capabilities.


---
