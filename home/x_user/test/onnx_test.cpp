#include <onnxruntime_cxx_api.h>
#include <iostream>
#include <vector>
#include <algorithm>
// simple test of the model
// g++ onnx_test.cpp -o onnx_test   -Ionnxruntime/include   -LIonnxruntime/lib   -lonnxruntime
// ./onnx_test
int main() {
    Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
    Ort::SessionOptions session_options;
    Ort::Session session(env, "Class_Freq_R.onnx", session_options);

    // Prepare dummy input (17 features, must match your model input!)
    std::vector<float> input_features = {
        0.1, 0.2, 0.3,   // means
        1.0, 1.0, 1.0,   // stds
        2.0, 2.0, 2.0,   // vars
        3.0, 3.0, 3.0,   // energy
        0.5, 0.6, 0.7,   // correlations
        0.0, 0.0, 0.0    // p-values or unused
    };
    size_t feature_dim = input_features.size();
    std::array<int64_t, 2> input_shape{1, (int64_t)feature_dim};

    // Get input/output names (need allocator as 2nd arg in ONNX Runtime >= 1.17)
    Ort::AllocatorWithDefaultOptions allocator;
    const char* input_name = "float_input";
    const char* output_name = "output_label"; // or whatever your output is named

    // Create tensor
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info, input_features.data(), input_features.size(),
        input_shape.data(), input_shape.size()
    );

    // Run inference
    std::vector<const char*> input_names{input_name};
    std::vector<const char*> output_names{output_name};
    auto output_tensors = session.Run(
        Ort::RunOptions{nullptr},
        input_names.data(), &input_tensor, 1,
        output_names.data(), 1
    );

    // Get and print result (assuming 3-class output)
    float* preds = output_tensors[0].GetTensorMutableData<float>();
    std::cout << "Class probabilities: ";
    for (size_t i = 0; i < 3; ++i) {
        std::cout << preds[i] << " ";
    }
    std::cout << std::endl;

    int max_idx = std::distance(preds, std::max_element(preds, preds + 3));
    std::string activity = (max_idx == 0 ? "Work" : max_idx == 1 ? "Relax" : "Fall");
    std::cout << "Predicted: " << activity << std::endl;
    return 0;
}
