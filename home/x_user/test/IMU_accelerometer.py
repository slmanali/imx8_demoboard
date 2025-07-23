# import time
# import numpy as np
# import onnxruntime as ort
# from smbus2 import SMBus, i2c_msg


# # --- Constants ---
# I2C_BUS = 3
# I2C_ADDR = 0x19
# WHO_AM_I = 0x0F
# CTRL1 = 0x20
# ON_CTRL1 = 0x60
# OUT_X_L = 0x28
# OUT_X_H = 0x29
# OUT_Y_L = 0x2A
# OUT_Y_H = 0x2B
# OUT_Z_L = 0x2C
# OUT_Z_H = 0x2D

# # --- Helper Functions ---

# def read_reg(bus, reg):
#     write = i2c_msg.write(I2C_ADDR, [reg])
#     read = i2c_msg.read(I2C_ADDR, 1)
#     bus.i2c_rdwr(write, read)
#     return list(read)[0]

# def write_reg(bus, reg, value):
#     bus.write_byte_data(I2C_ADDR, reg, value)

# def initialize_sensor(bus):
#     who_am_i = read_reg(bus, WHO_AM_I)
#     if who_am_i != 0x44:
#         raise Exception(f"Device not LIS2DW12TR! WHO_AM_I returned {hex(who_am_i)}")
#     write_reg(bus, CTRL1, ON_CTRL1)

# def read_accel(bus):
#     def read_axis(l, h):
#         low = read_reg(bus, l)
#         high = read_reg(bus, h)
#         val = (high << 8) | low
#         if val >= 32768:
#             val -= 65536
#         return val
#     x = read_axis(OUT_X_L, OUT_X_H)
#     y = read_axis(OUT_Y_L, OUT_Y_H)
#     z = read_axis(OUT_Z_L, OUT_Z_H)
#     return x, y, z

# # --- Feature Extraction ---

# def compute_features(X, Y, Z):
#     def mean(v):
#         return sum(v) / len(v)

#     def stddev(v, m):
#         return np.std(v)

#     def var(v, m):
#         return np.var(v)

#     def ener(v):
#         return sum(x*x for x in v)

#     def pearson(a, b):
#         if len(a) != len(b):
#             return 0.0, 0.0
#         corr_matrix = np.corrcoef(a, b)
#         return corr_matrix[0, 1], corr_matrix[1, 0] 

#     x_mean = mean(X)
#     y_mean = mean(Y)
#     z_mean = mean(Z)

#     xstd = stddev(X, x_mean)
#     ystd = stddev(Y, y_mean)
#     zstd = stddev(Z, z_mean)

#     xvar = var(X, x_mean)
#     yvar = var(Y, y_mean)
#     zvar = var(Z, z_mean)

#     xner = ener(X)
#     yner = ener(Y)
#     zner = ener(Z)

#     xcor_r, xcor_p = pearson(X, Y)
#     ycor_r, ycor_p = pearson(Y, Z)
#     zcor_r, zcor_p = pearson(Z, X)

#     # p-values not calculated here
#     features = [
#         x_mean, y_mean, z_mean,
#         xstd, ystd, zstd,
#         xvar, yvar, zvar,
#         xner, yner, zner,
#         xcor_r, ycor_r, zcor_r,
#         xcor_p, ycor_p, zcor_p
#     ]
#     return features

# # --- Main Loop ---

# def main():
#     bus = SMBus(I2C_BUS)
#     initialize_sensor(bus)

#     # Load ONNX model
#     sess = ort.InferenceSession("Class_Freq_R.onnx")

#     input_name = sess.get_inputs()[0].name
#     output_name = sess.get_outputs()[0].name

#     X, Y, Z = [], [], []

#     try:
#         while True:
#             x, y, z = read_accel(bus)
#             X.append(x)
#             Y.append(y)
#             Z.append(z)

#             if len(X) == 180:
#                 features = compute_features(X, Y, Z)
#                 print(f"features.size(): {len(features)}")

#                 # Run inference
#                 input_data = np.array([features], dtype=np.float32)
#                 outputs = sess.run([output_name], {input_name: input_data})
                
#                 preds = outputs[0].flatten()  # Normalize output
#                 print("Output shape:", outputs[0].shape)
#                 print("Raw output:", preds)
#                 label = int(preds[0]) - 1
#                 print("label:", label)
#                 activities = ["Work", "Relax", "Fall"]

#                 # Safety check to prevent index out-of-range
#                 # if label < 0 or label >= len(activities):
#                 #     print(f"Unexpected label: {label}")
#                 #     activity = "Unknown"
#                 # else:
#                 activity = activities[label]
#                 print("activity:", activity)

#                 # Clear buffers
#                 X.clear()
#                 Y.clear()
#                 Z.clear()

#             time.sleep(0.01)  # ~100Hz

#     except KeyboardInterrupt:
#         print("Exiting...")
#     finally:
#         bus.close()

# if __name__ == "__main__":
#     main()

# # import multiprocessing
# # import smbus
# # import time
# # import math
# # import numpy as np
# # import logging
# # import datetime
# # import csv
# # import os
# # import sys
# # import subprocess
# # import json
# # import numpy as np
# # from scipy.stats.stats import pearsonr
# # import joblib

# # # IMU_LOG_FILE_PATH = '/home/x_user/test/IMU_log'
# # Model_File = '/home/x_user/test/Class_Freq_R.pkl'
# # rf = joblib.load(Model_File)

# # # IMU LIS2DW12TR registers
# # # LIS2DW12TR I2C address
# # I2C_CHANNEL = 3  # initializes the I2C channel related to the accelerometer
# # I2C_ADDR = 0x19  # Default I2C address
# # WHO_AM_I = 0x0F
# # CTRL1 = 0x20
# # ON_CTRL1 = 0x60
# # OUT_X_L = 0x28
# # OUT_X_H = 0x29
# # OUT_Y_L = 0x2A
# # OUT_Y_H = 0x2B
# # OUT_Z_L = 0x2C
# # OUT_Z_H = 0x2D


# # # Configure LIS2DW12TR for 100Hz ODR in high-performance mode
# # def initialize_sensor(bus):
# #     # Check if the device is LIS2DW12TR
# #     who_am_i = bus.read_byte_data(I2C_ADDR, WHO_AM_I)
# #     if who_am_i != 0x44:
# #         raise ValueError("Device is not LIS2DW12TR!")

# #     # Set the data rate to 100Hz and enable all axes
# #     bus.write_byte_data(I2C_ADDR, CTRL1, ON_CTRL1)


# # def read_acceleration(bus):
# #     x = (bus.read_byte_data(I2C_ADDR, OUT_X_H) << 8) | bus.read_byte_data(I2C_ADDR, OUT_X_L)
# #     y = (bus.read_byte_data(I2C_ADDR, OUT_Y_H) << 8) | bus.read_byte_data(I2C_ADDR, OUT_Y_L)
# #     z = (bus.read_byte_data(I2C_ADDR, OUT_Z_H) << 8) | bus.read_byte_data(I2C_ADDR, OUT_Z_L)
# #     # Convert to signed values
# #     x = x if x < 32768 else x - 65536
# #     y = y if y < 32768 else y - 65536
# #     z = z if z < 32768 else z - 65536
# #     return x, y, z


# # def get_name(index):
# #     for name, idx in labels.items():
# #         if idx == index:
# #             return name
# #     return None


# # # def magnitude_rate_time(magnitude,prev_magnitude,prev_time):
# # #     current_time = time.time()
# # #     if prev_magnitude is not None and prev_time is not None:
# # #         delta_t = current_time - prev_time
# # #         rate_of_change = (magnitude - prev_magnitude) / delta_t
# # #         prev_magnitude=magnitude
# # #         prev_time=current_time
# # #         return rate_of_change,prev_magnitude,prev_time
# # #     else:
# # #         prev_magnitude = magnitude
# # #         prev_time = current_time
# # #         return 0, prev_magnitude, prev_time
# # #
# # # def roll_pitch(x,y,z):
# # #     roll = math.atan2(y, math.sqrt(x ** 2 + z ** 2))
# # #     pitch = math.atan2(x, math.sqrt(y ** 2 + z ** 2))
# # #
# # #     # Convert to degrees if needed
# # #     roll_deg = math.degrees(roll)
# # #     pitch_deg = math.degrees(pitch)
# # #     return roll_deg,pitch_deg


# # if __name__ == '__main__':
# #     # print("==================================================================================================")
# #     # print("__main__ Accelerometer_IMU__")
# #     # print("Start")
# #     bus = smbus.SMBus(I2C_CHANNEL)  # Use I2C bus 1 on Raspberry Pi
# #     initialize_sensor(bus)
# #     # argv = sys.argv[1]
# #     Time_run = datetime.datetime.now()
# #     # IMU_LOG_FILE_PATH=IMU_LOG_FILE_PATH+Time_run.strftime('%Y_%m_%d_%H_%M_%S')+".csv"
# #     IMU_LOG_FILE = "/home/x_user/test/IMU_log.csv"
# #     IMU_RES_FILE = "/home/x_user/test/IMU_res.csv"
# #     Frames = []
# #     X = np.array([])
# #     Y = np.array([])
# #     Z = np.array([])
# #     labels = {'Work': 1, 'Relax': 2, 'Fall': 3}
# #     Time_run=None
# #     _init=0
# #     while True:
        
# #         x, y, z = read_acceleration(bus)
# #         timestamp = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')
# #         run_imu = '0'
# #         try:
# #             with open('/home/x_user/client/variable_store.json', 'r') as f:
# #                 data = json.load(f)
# #                 run_imu = data.get('run_imu')
# #         except Exception as e:
# #             pass
# #         if run_imu == '1':
# #             with open(IMU_LOG_FILE, 'a+', newline='') as csvfile:
# #                 csv_writer = csv.writer(csvfile)
# #                 if Time_run == None:
# #                     Time_run = datetime.datetime.now()
# #                     csv_writer.writerow(["Timestamp", "X", "Y", "Z"])
# #                 csv_writer.writerow([timestamp, x, y, z])
# #         if Time_run != None:
# #             difference = datetime.datetime.now() - Time_run
# #             if (difference.total_seconds() / 60) > 3:
# #                 Time_run = None
# #                 data['run_imu'] = "0"
# #                 with open('/home/x_user/my_camera_project/variable_store.json', 'w') as f:
# #                     json.dump(data, f, indent=4)
# #                 subprocess.check_output("sh /home/x_user/my_camera_project/scripts/copy_IMU.sh", shell=True).decode("utf-8")
# #         X = np.append(X, x)
# #         Y = np.append(Y, y)
# #         Z = np.append(Z, z)
# #         time.sleep(0.01)  # 100Hz sampling rate
# #         ##if difference.total_seconds() > 3:
# #         if len(X) == 180:
# #             Feature_list = []
# #             x_mean = X.mean()
# #             y_mean = Y.mean()
# #             z_mean = Z.mean()
# #             xstd = X.std()
# #             ystd = Y.std()
# #             zstd = Z.std()
# #             xvar = X.var()
# #             yvar = Y.var()
# #             zvar = Z.var()
# #             xner = sum(X ** 2)
# #             yner = sum(Y ** 2)
# #             zner = sum(Z ** 2)
# #             xcor = pearsonr(X, Y)
# #             ycor = pearsonr(Y, Z)
# #             zcor = pearsonr(Z, X)
# #             Feature_list.append([x_mean, y_mean, z_mean, xstd, ystd, zstd, xvar, yvar, zvar,
# #                                  xner, yner, zner, xcor[0], ycor[0], zcor[0], xcor[1], ycor[1], zcor[1]])
# #             preds = rf.predict_proba(Feature_list)
# #             pf = rf.predict(Feature_list)
# #             X = np.array([])
# #             Y = np.array([])
# #             Z = np.array([])
# #             result = get_name(pf)
# #             with open(IMU_RES_FILE, 'a+', newline='') as csvfile:
# #                 csv_writer = csv.writer(csvfile)
# #                 if _init == 0:
# #                     csv_writer.writerow(["Timestamp", "result"])
# #                     _init = 1
# #                 csv_writer.writerow([timestamp, result])
# #             # print(result)

# # # try:
# # #     # Initialize variables
# # #     prev_magnitude1 = None
# # #     prev_time1 = None
# # #     while True:
# # #
# # #         # Combine low and high bytes
# # #         acceleration_x,acceleration_y,acceleration_z = read_acceleration(bus,address)
# # #         magnitude = math.sqrt(acceleration_x**2 + acceleration_y**2 + acceleration_z**2)
# # #         magnitude_change,prev_magnitude,prev_time = magnitude_rate_time(magnitude,prev_magnitude1,prev_time1)
# # #         roll_deg,pitch_deg = roll_pitch(acceleration_x,acceleration_y,acceleration_z)
# # #
# # #
# # #
# # #         # convert to m/s^2 by multiplying by 9.81
# # #         acceleration_x_m_s2 = acceleration_x * 9.81
# # #         acceleration_y_m_s2 = acceleration_y * 9.81
# # #         acceleration_z_m_s2 = acceleration_z * 9.81
# # #
# # #         feature_vector = np.array([[round(acceleration_x_m_s2,2), round(acceleration_y_m_s2,2), round(acceleration_z_m_s2,2), magnitude, magnitude_change,
# # #                                     roll_deg, pitch_deg]])  # Add other features if needed
# # #         # feature_vector = scaler.transform(feature_vector)
# # #         # print("Acceleration on X-axis in m/s^2: ", round(acceleration_x_m_s2,2))
# # #         # print("Acceleration on Y-axis in m/s^2: ", round(acceleration_y_m_s2,2))
# # #         # print("Acceleration on Z-axis in m/s^2: ", round(acceleration_z_m_s2,2))
# # #         print("feature vector : ", feature_vector)
# # #         prev_magnitude1=prev_magnitude
# # #         prev_time1=prev_time
# # #         # Sleep for 500 ms
# # #         time.sleep(0.5)
# # #
# # # except KeyboardInterrupt:
# # #     print("Stopped.")

## check model output
import onnx
import onnxruntime as ort

# Load and inspect model
model = onnx.load("/home/x_user/my_camera_project/Class_Freq_R.onnx")
onnx.checker.check_model(model)

# Print input/output info
print("Inputs:")
for inp in model.graph.input:
    print(f"  {inp.name} {inp.type.tensor_type.shape.dim}")

print("\nOutputs:")
for out in model.graph.output:
    print(f"  {out.name} {out.type.tensor_type.shape.dim}")

# Alternatively use ONNX Runtime
session = ort.InferenceSession("/home/x_user/my_camera_project/Class_Freq_R.onnx")
print(session.get_inputs())
print(session.get_outputs())