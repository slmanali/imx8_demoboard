# Code Documentation for `onnx_test.cpp`

This document provides a detailed overview of the `onnx_test.cpp` source file, which is designed to perform inference using an ONNX (Open Neural Network Exchange) model named `Class_Freq_R.onnx`. The file demonstrates how to load the model, prepare input data, execute inference, and retrieve the results.

## Overview

The `onnx_test.cpp` file serves as a minimal example of how to use the ONNX Runtime C++ API for model inference. It showcases the core steps required to interact with an ONNX model, including initializing the environment, creating input tensors, running the model, and processing the output.

### Compilation Instructions

To compile the code, use the following command (assuming appropriate paths for ONNX Runtime’s headers and libraries):

```bash
g++ onnx_test.cpp -o onnx_test -Ionnxruntime/include -LIonnxruntime/lib -lonnxruntime
```

### Execution Instructions

Run the compiled program using:

```bash
./onnx_test
```

## Detailed Code Explanation

### Headers and Namespace

```cpp
#include <onnxruntime_cxx_api.h>
#include <iostream>
#include <vector>
#include <algorithm>
```

- `onnxruntime_cxx_api.h`: Header file for ONNX Runtime C++ API, allowing interaction with ONNX models.
- `<iostream>`: Standard input and output stream library for printing results to the console.
- `<vector>`: Standard library to use the `std::vector` container, which supports dynamic arrays.
- `<algorithm>`: Standard library providing a set of algorithms, including `std::max_element`.

### Main Function

The execution begins with the `main` function.

```cpp
int main() {
```

#### Environment and Session Initialization

```cpp
Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "test");
Ort::SessionOptions session_options;
Ort::Session session(env, "Class_Freq_R.onnx", session_options);
```

- The `Ort::Env` object is created to encapsulate the ONNX Runtime environment, initialized with a logging level of `WARNING`.
- `Ort::SessionOptions` holds configuration options for the session but is not configured beyond its instantiation.
- The `Ort::Session` object is instantiated with the ONNX model file `Class_Freq_R.onnx`.

#### Input Preparation

```cpp
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
```

- A `std::vector<float>` named `input_features` is initialized with 17 feature values. These values must match the expected input format for the model.
- `feature_dim` captures the dimension of the input features.
- `input_shape` is defined as a 2D tensor shape, where the first dimension is `1` (batch size) and the second dimension is the size of the feature vector.

#### Allocator and Input/Output Names

```cpp
Ort::AllocatorWithDefaultOptions allocator;
const char* input_name = "float_input";
const char* output_name = "output_label"; // or whatever your output is named
```

- An allocator for memory management in ONNX Runtime is created.
- `input_name` and `output_name` store the names of the input and output tensors as expected by the ONNX model.

#### Tensor Creation

```cpp
Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
    memory_info, input_features.data(), input_features.size(),
    input_shape.data(), input_shape.size()
);
```

- `Ort::MemoryInfo` specifies the memory type for the tensor, here indicating CPU usage.
- The `Ort::Value::CreateTensor` function creates a tensor using the input data, feature size, and shape.

#### Model Inference

```cpp
std::vector<const char*> input_names{input_name};
std::vector<const char*> output_names{output_name};
auto output_tensors = session.Run(
    Ort::RunOptions{nullptr},
    input_names.data(), &input_tensor, 1,
    output_names.data(), 1
);
```

- Vectors for input and output names are prepared.
- The `session.Run` method executes the ONNX model, passing in input names, tensor data, and expecting output names.

#### Results Processing and Output

```cpp
float* preds = output_tensors[0].GetTensorMutableData<float>();
std::cout << "Class probabilities: ";
for (size_t i = 0; i < 3; ++i) {
    std::cout << preds[i] << " ";
}
std::cout << std::endl;

int max_idx = std::distance(preds, std::max_element(preds, preds + 3));
std::string activity = (max_idx == 0 ? "Work" : max_idx == 1 ? "Relax" : "Fall");
std::cout << "Predicted: " << activity << std::endl;
```

- The predictions are extracted from the output tensor.
- The predicted class probabilities are printed to the console.
- `std::max_element` is used to determine the index of the highest predicted probability.
- A string representation of the activity (Work, Relax, or Fall) is generated based on the index value and printed.

### Return Statement

```cpp
return 0;
}
```

- The `main` function returns `0`, indicating successful execution.

## Conclusion

The `onnx_test.cpp` file is a complete example demonstrating how to use the ONNX Runtime to perform inference on a pre-trained ONNX model. It initializes the necessary environment and session, prepares input data, invokes the model, and outputs the results. This code can serve as a foundation for further development and experimentation with ONNX models in C++.

---
