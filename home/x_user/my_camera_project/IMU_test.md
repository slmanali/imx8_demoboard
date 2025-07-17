# IMU_test.cpp Code Documentation

The `IMU_test.cpp` file is responsible for interfacing with an Inertial Measurement Unit (IMU) sensor, reading acceleration data, and executing a machine learning model using ONNX Runtime to classify activities based on the sensor data. This documentation provides an overview of the code structure, constants, helper functions, feature extraction logic, and the main application flow.

## Constants

The constants defined in the code are used to configure the I2C interface and to access various registers of the LIS2DW12TR accelerometer. Their meanings are as follows:

- `const std::string I2C_DEV`: Path to the I2C device file.
- `const int I2C_ADDR`: I2C address of the IMU sensor.
- `const int WHO_AM_I`: Register address to identify the device.
- `const int CTRL1`: Control register address for the device.
- `const int ON_CTRL1`: Configuration value to enable the sensor.
- `const int OUT_X_L`, `OUT_X_H`, `OUT_Y_L`, `OUT_Y_H`, `OUT_Z_L`, `OUT_Z_H`: Register addresses for reading the x, y, and z axis acceleration data (low and high bytes).

## Helper Functions

### `int read_reg(int fd, int reg)`
Reads a single byte from a specified register of the I2C device.

**Parameters:**
- `fd`: The file descriptor for the I2C device.
- `reg`: Register address to read from.

**Returns:**
- The value read from the specified register.

**Exceptions:**
- Throws an exception if writing or reading fails.

### `void write_reg(int fd, int reg, int value)`
Writes a single byte to a specified register of the I2C device.

**Parameters:**
- `fd`: The file descriptor for the I2C device.
- `reg`: Register address to write to.
- `value`: Value to be written.

**Exceptions:**
- Throws an exception if writing fails.

### `void initialize_sensor(int fd)`
Initializes the IMU sensor by checking its identity and enabling it.

**Parameters:**
- `fd`: The file descriptor for the I2C device.

**Exceptions:**
- Throws an exception if the device does not match the expected identity or if initialization fails.

### `void read_accel(int fd, int16_t& x, int16_t& y, int16_t& z)`
Reads the acceleration data along the x, y, and z axes.

**Parameters:**
- `fd`: The file descriptor for the I2C device.
- `x`, `y`, `z`: References to store the acceleration readings for the x, y, and z axes.

## Feature Extraction

### `std::vector<float> compute_features(const std::vector<float>& X, const std::vector<float>& Y, const std::vector<float>& Z)`
Computes various statistical features from the acceleration data collected over a period.

**Parameters:**
- `X`, `Y`, `Z`: Vectors of float values representing acceleration data along the x, y, and z axes.

**Returns:**
- A vector of features including mean, standard deviation, variance, energy, and Pearson correlation coefficients for each axis.

### Features Calculated:
1. Mean for each axis (X, Y, Z)
2. Standard deviation for each axis (X, Y, Z)
3. Variance for each axis (X, Y, Z)
4. Energy for each axis (X, Y, Z)
5. Pearson correlation coefficients between axes (X vs. Y, Y vs. Z, Z vs. X)

## Main Function

The entry point of the application is defined within the `main()` function. Key actions performed are:

1. **I2C Initialization:**
   - Opens the I2C device and configures it to communicate with the IMU sensor.
   
2. **Sensor Initialization:**
   - Invokes `initialize_sensor()` to verify the connection and set operational parameters.

3. **ONNX Runtime Initialization:**
   - Sets up the ONNX Runtime environment and session to load a pre-trained model for classification.
   - Retrieves input and output information from the model.

4. **Data Collection Loop:**
   - Continuously reads acceleration data from the IMU.
   - If 180 samples have been collected, it computes features from the readings and prepares an input tensor for the ONNX model.
   - Runs inference on the model and processes the output to determine the corresponding activity label.

5. **Output Handling:**
   - Displays raw output values and the identified activity based on the classification result.
   - Resets the data collection vectors after processing.

6. **Error Handling:**
   - Employs a try-catch block to manage exceptions, printing error messages when necessary.

## Execution Notes

- The loop collects data at approximately 100Hz (`usleep(10000);`), which implies the program processes new data roughly every 10 milliseconds.
- The program keeps running indefinitely until terminated externally.
- It expects specific output configurations from the ONNX model, handling both integer and float outputs seamlessly.
- Users must ensure that the ONNX model file and input shapes match the processing expectations detailed in the code.

This documentation provides a comprehensive understanding of the code's structure and functionality, facilitating easier maintenance and further development.

---
