#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <onnxruntime_cxx_api.h>
// test the IMU sensor with the model
//g++ -std=c++17 IMU_test.cpp -o imu_test -I/home/x_user/my_camera_project/onnxruntime/include -lonnxruntime -lm
// --- Constants ---
const std::string I2C_DEV = "/dev/i2c-3";
const int I2C_ADDR = 0x19;
const int WHO_AM_I = 0x0F;
const int CTRL1 = 0x20;
const int ON_CTRL1 = 0x60;
const int OUT_X_L = 0x28;
const int OUT_X_H = 0x29;
const int OUT_Y_L = 0x2A;
const int OUT_Y_H = 0x2B;
const int OUT_Z_L = 0x2C;
const int OUT_Z_H = 0x2D;

// --- Helper Functions ---
int read_reg(int fd, int reg) {
    uint8_t buf[1] = {static_cast<uint8_t>(reg)};
    if (write(fd, buf, 1) != 1) {
        throw std::runtime_error("Failed to write to I2C device");
    }
    if (read(fd, buf, 1) != 1) {
        throw std::runtime_error("Failed to read from I2C device");
    }
    return buf[0];
}

void write_reg(int fd, int reg, int value) {
    uint8_t buf[2] = {static_cast<uint8_t>(reg), static_cast<uint8_t>(value)};
    if (write(fd, buf, 2) != 2) {
        throw std::runtime_error("Failed to write to I2C device");
    }
}

void initialize_sensor(int fd) {
    int who_am_i = read_reg(fd, WHO_AM_I);
    if (who_am_i != 0x44) {
        throw std::runtime_error("Device not LIS2DW12TR! WHO_AM_I returned " + std::to_string(who_am_i));
    }
    write_reg(fd, CTRL1, ON_CTRL1);
}

void read_accel(int fd, int16_t& x, int16_t& y, int16_t& z) {
    auto read_axis = [&](int l, int h) {
        int16_t val = (read_reg(fd, h) << 8) | read_reg(fd, l);
        if (val >= 32768) val -= 65536;
        return val;
    };
    x = read_axis(OUT_X_L, OUT_X_H);
    y = read_axis(OUT_Y_L, OUT_Y_H);
    z = read_axis(OUT_Z_L, OUT_Z_H);
}

// --- Feature Extraction ---
std::vector<float> compute_features(const std::vector<float>& X, 
                                   const std::vector<float>& Y, 
                                   const std::vector<float>& Z) {
    auto mean = [](const std::vector<float>& v) {
        return std::accumulate(v.begin(), v.end(), 0.0f) / v.size();
    };

    auto stddev = [](const std::vector<float>& v, float m) {
        float sum = 0.0f;
        for (auto x : v) sum += (x - m) * (x - m);
        return std::sqrt(sum / v.size());
    };

    auto var = [](const std::vector<float>& v, float m) {
        float sum = 0.0f;
        for (auto x : v) sum += (x - m) * (x - m);
        return sum / v.size();
    };

    auto ener = [](const std::vector<float>& v) {
        float sum = 0.0f;
        for (auto x : v) sum += x * x;
        return sum;
    };

    auto pearson = [](const std::vector<float>& a, const std::vector<float>& b) {
        if (a.size() != b.size()) return std::make_pair(0.0f, 0.0f);
        
        float sum_a = std::accumulate(a.begin(), a.end(), 0.0f);
        float sum_b = std::accumulate(b.begin(), b.end(), 0.0f);
        float mean_a = sum_a / a.size();
        float mean_b = sum_b / b.size();
        
        float cov = 0.0f, var_a = 0.0f, var_b = 0.0f;
        for (size_t i = 0; i < a.size(); ++i) {
            float da = a[i] - mean_a;
            float db = b[i] - mean_b;
            cov += da * db;
            var_a += da * da;
            var_b += db * db;
        }
        
        float denom = std::sqrt(var_a * var_b);
        float r = (denom != 0) ? cov / denom : 0.0f;
        return std::make_pair(r, 0.0f); // p-value not calculated
    };

    float x_mean = mean(X);
    float y_mean = mean(Y);
    float z_mean = mean(Z);

    float xstd = stddev(X, x_mean);
    float ystd = stddev(Y, y_mean);
    float zstd = stddev(Z, z_mean);

    float xvar = var(X, x_mean);
    float yvar = var(Y, y_mean);
    float zvar = var(Z, z_mean);

    float xner = ener(X);
    float yner = ener(Y);
    float zner = ener(Z);

    auto [xcor_r, xcor_p] = pearson(X, Y);
    auto [ycor_r, ycor_p] = pearson(Y, Z);
    auto [zcor_r, zcor_p] = pearson(Z, X);

    std::vector<float> features = {
        x_mean, y_mean, z_mean,
        xstd, ystd, zstd,
        xvar, yvar, zvar,
        xner, yner, zner,
        xcor_r, ycor_r, zcor_r,
        xcor_p, ycor_p, zcor_p
    };

    return features;
}

// --- Main ---
int main() {
    try {
        // Initialize I2C (same as before)
        int fd = open(I2C_DEV.c_str(), O_RDWR);
        if (fd < 0) throw std::runtime_error("Can't open I2C device");
        if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
            close(fd);
            throw std::runtime_error("Failed to acquire bus access");
        }
        initialize_sensor(fd);

        // Initialize ONNX Runtime
        Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "IMU_Classifier");
        Ort::SessionOptions session_options;
        Ort::Session session(env, "Class_Freq_R.onnx", session_options);

        // Get input/output info (MODIFIED)
        Ort::AllocatorWithDefaultOptions allocator;
        const char* input_name = "float_input";
        const char* output_name = "output_label"; // Or "output_probability" if you want probabilities
        std::vector<const char*> input_names{input_name};
        std::vector<const char*> output_names{output_name};
        
        // Check output type (NEW)
        size_t num_outputs = session.GetOutputCount();
        // if (num_outputs != 1) throw std::runtime_error("Expected 1 output");
        
        Ort::TypeInfo output_type_info = session.GetOutputTypeInfo(0);
        auto output_tensor_info = output_type_info.GetTensorTypeAndShapeInfo();
        ONNXTensorElementDataType output_type = output_tensor_info.GetElementType();
        
        // std::string output_name = session.GetOutputNameAllocated(0, allocator).get();
        std::cout << "Output type: " << output_type << std::endl;  // 1=float, 7=int64

        std::vector<float> X, Y, Z;
        const std::vector<std::string> activities = {"Work", "Relax", "Fall"};

        while (true) {
            int16_t x, y, z;
            read_accel(fd, x, y, z);
            X.push_back(static_cast<float>(x));
            Y.push_back(static_cast<float>(y));
            Z.push_back(static_cast<float>(z));

            if (X.size() == 180) {
                std::vector<float> features = compute_features(X, Y, Z);
                std::cout << "features.size(): " << features.size() << std::endl;

                // Prepare input tensor (same as before)
                std::array<int64_t, 2> input_shape = {1, static_cast<int64_t>(features.size())};
                Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
                Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
                    memory_info, features.data(), features.size(),
                    input_shape.data(), input_shape.size()
                );

                auto output_tensors = session.Run(
                    Ort::RunOptions{nullptr},
                    input_names.data(), &input_tensor, 1,
                    output_names.data(), 1
                );

                // Assume output is a label (int64) or probability vector (float), depends on your ONNX export
                float* preds = output_tensors[0].GetTensorMutableData<float>();
                int max_idx = std::distance(preds, std::max_element(preds, preds + 3));
                std::string activity = (max_idx == 0 ? "Work" : max_idx == 1 ? "Relax" : "Fall");
                std::cout << "Prediction: " << activity << std::endl;

                X.clear();
                Y.clear();
                Z.clear();
            }

            usleep(10000); // ~100Hz
        }

        close(fd);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}