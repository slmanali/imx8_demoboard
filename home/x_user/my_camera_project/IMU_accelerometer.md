# IMU Accelerometer Code Documentation

## Overview
The `IMU_accelerometer.py` script is designed to interface with an LIS2DW12TR accelerometer through the I2C protocol. It initializes the sensor, reads acceleration data, computes features from the collected data, and finally utilizes an ONNX model for activity recognition. Activities recognized are "Work", "Relax", and "Fall".

## Requirements
- `numpy`: For numerical operations and feature computations.
- `onnxruntime`: For running the ONNX model for activity recognition.
- `smbus2`: For I2C communication with the accelerometer.

## Constants
The following constants are defined to facilitate communication with the LIS2DW12TR accelerometer:

```python
I2C_BUS = 3                # I2C bus number
I2C_ADDR = 0x19           # I2C address of the accelerometer
WHO_AM_I = 0x0F           # Register to read the device ID
CTRL1 = 0x20              # Control register for setting performance parameters
ON_CTRL1 = 0x60           # Value to set control register for high-performance mode
OUT_X_L, OUT_X_H = 0x28, 0x29 # X-axis output registers
OUT_Y_L, OUT_Y_H = 0x2A, 0x2B # Y-axis output registers
OUT_Z_L, OUT_Z_H = 0x2C, 0x2D # Z-axis output registers
```

## Function Definitions

### 1. `read_reg(bus, reg)`
* Reads a single byte from the specified register of the I2C device.

#### Parameters:
- `bus`: An `SMBus` object representing the I2C bus.
- `reg`: The register address to read from.

#### Returns:
- The byte value read from the specified register.

### 2. `write_reg(bus, reg, value)`
* Writes a single byte to the specified register of the I2C device.

#### Parameters:
- `bus`: An `SMBus` object representing the I2C bus.
- `reg`: The register address to write to.
- `value`: The byte value to write.

### 3. `initialize_sensor(bus)`
* Initializes the accelerometer sensor by checking its identity and setting control parameters for operation.

#### Parameters:
- `bus`: An `SMBus` object representing the I2C bus.

#### Raises:
- `Exception` if the WHO_AM_I register does not return the expected device ID (0x44).

### 4. `read_accel(bus)`
* Reads acceleration data from the accelerometer for all three axes (X, Y, Z).

#### Parameters:
- `bus`: An `SMBus` object representing the I2C bus.

#### Returns:
- A tuple containing the acceleration values for X, Y, Z (in integer format).

### 5. `compute_features(X, Y, Z)`
* Computes features from the collected acceleration data. Features include mean, standard deviation, variance, energy, and Pearson correlation coefficients between axes.

#### Parameters:
- `X`, `Y`, `Z`: Lists of acceleration data points for the respective axes.

#### Returns:
- A list containing computed features.

## Main Loop
The `main` function contains the core of the script that executes the following steps:

1. **Initialize the I2C Bus**: Opens the I2C bus.
2. **Initialize Sensor**: Calls `initialize_sensor` to set up the accelerometer.
3. **Load the ONNX Model**: Prepares the activity recognition model for inference.
4. **Data Collection Loop**: Continuously collects acceleration data:
   - Once enough data points are collected (180), computes features using `compute_features`.
   - Runs inference with the ONNX model and gets the predicted activity label.
   - Maps the label to a human-readable activity name.
   - Clears accelerometer buffers for the next iteration.
5. **Graceful Exit**: Closes the bus on keyboard interruption.

### Functionality Flow
1. Start the main function.
2. Initialize communication with the I2C accelerometer.
3. Collect acceleration data every 10 milliseconds (~100 Hz sampling rate).
4. Compute features every time 180 samples are collected.
5. Use the ONNX model to predict the activity type based on the features.
6. Print the resulting activity and the predicted label.

### Example of Usage
To run the script, ensure all dependencies are installed, and execute:

```bash
python IMU_accelerometer.py
```

## Important Notes
- The accelerometer is assumed to be located at the specified I2C address (0x19). Any change in hardware may require adjusting the `I2C_ADDR`.
- The ONNX model file `Class_Freq_R.onnx` must exist in the same directory as the script. If the path changes, update the line where `InferenceSession` is initiated.
- The script caught a `KeyboardInterrupt` (usually via Ctrl+C) to exit gracefully, ensuring any resources such as the I2C bus are cleaned up.

This documentation serves as a comprehensive overview of how the `IMU_accelerometer.py` functions, identifying the purpose of each function, the constants used, and providing usage instructions.

---
