Here is the detailed documentation for the `vosk_api.h` header file, which defines the C API for the Vosk speech recognition system. This documentation covers the purpose of each function, structure, and macro, along with detailed parameter and return value descriptions.

### Overview
`vosk_api.h` provides a C language interface to the Vosk speech recognition system. Vosk enables developers to create applications that convert speech into text. The API is structured around several key components, including models for speech recognition and speaker identification, recognizers that process audio data, and batch processing capabilities.

### License Notice
```
/* 
 * This file is part of Vosk speech recognition system, developed by Alpha Cephei Inc. 
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */
```

### Header Guard
```c
#ifndef VOSK_API_H
#define VOSK_API_H
```
This prevents the header file from being included multiple times within a single translation unit.

### Extern "C"
```c
#ifdef __cplusplus
extern "C" {
#endif
```
This block allows the header to be included in C++ projects without name mangling, ensuring functions can be linked correctly.

### Structures
#### VoskModel
```c
typedef struct VoskModel VoskModel;
```
Represents a speech recognition model containing data needed for recognition. It can be shared across multiple processing threads.

#### VoskSpkModel
```c
typedef struct VoskSpkModel VoskSpkModel;
```
Represents a speech recognition model that includes data for speaker identification.

#### VoskRecognizer
```c
typedef struct VoskRecognizer VoskRecognizer;
```
The main object that processes audio input to perform speech recognition. It returns the recognition results in JSON format.

#### VoskBatchModel and VoskBatchRecognizer
```c
typedef struct VoskBatchModel VoskBatchModel;
typedef struct VoskBatchRecognizer VoskBatchRecognizer;
```
Structures for managing batch processing of audio data and recognition results.

### Functions

#### Model Management
- **Loading a Model**
    ```c
    VoskModel *vosk_model_new(const char *model_path);
    ```
    Loads a model from the specified path.
    - **Parameters**: `model_path`: Path to the model directory.
    - **Returns**: Pointer to a `VoskModel` object, or `NULL` on failure.
  
- **Freeing Model Memory**
    ```c
    void vosk_model_free(VoskModel *model);
    ```
    Releases the memory held by the model.

- **Finding Words in the Model**
    ```c
    int vosk_model_find_word(VoskModel *model, const char *word);
    ```
    Checks if a specific word can be recognized by the model.
    - **Parameters**: `word`: The word to check.
    - **Returns**: The symbol of the word if found, or -1 if not available.

#### Speaker Model Management
- **Loading Speaker Model**
    ```c
    VoskSpkModel *vosk_spk_model_new(const char *model_path);
    ```
    Loads a speaker model from the specified path.

- **Freeing Speaker Model**
    ```c
    void vosk_spk_model_free(VoskSpkModel *model);
    ```
    Releases memory associated with a speaker model.

#### Recognizer Management
- **Creating a Recognizer**
    ```c
    VoskRecognizer *vosk_recognizer_new(VoskModel *model, float sample_rate);
    ```
    Creates a recognizer for speech recognition.
    - **Parameters**:
        - `model`: Preloaded `VoskModel` object.
        - `sample_rate`: Sample rate of the audio data.
    - **Returns**: Pointer to a `VoskRecognizer` object, or `NULL` on failure.

- **Creating a Recognizer with Speaker Recognition**
    ```c
    VoskRecognizer *vosk_recognizer_new_spk(VoskModel *model, float sample_rate, VoskSpkModel *spk_model);
    ```
    Similar to the previous function, but includes an additional parameter for the speaker model for identification.

- **Creating a Recognizer with a Grammar List**
    ```c
    VoskRecognizer *vosk_recognizer_new_grm(VoskModel *model, float sample_rate, const char *grammar);
    ```
    Allows creation of a recognizer that uses a specified list of phrases to enhance recognition accuracy.

- **Setting Speaker Model for an Existing Recognizer**
    ```c
    void vosk_recognizer_set_spk_model(VoskRecognizer *recognizer, VoskSpkModel *spk_model);
    ```

- **Configuring Recognizer for Grammar**
    ```c
    void vosk_recognizer_set_grm(VoskRecognizer *recognizer, const char *grammar);
    ```

- **Setting Maximum Alternatives for Recognition**
    ```c
    void vosk_recognizer_set_max_alternatives(VoskRecognizer *recognizer, int max_alternatives);
    ```

- **Enabling Word Timing**
    ```c
    void vosk_recognizer_set_words(VoskRecognizer *recognizer, int words);
    ```

- **Enabling Partial Words in Results**
    ```c
    void vosk_recognizer_set_partial_words(VoskRecognizer *recognizer, int partial_words);
    ```

- **Setting NLSML Output**
    ```c
    void vosk_recognizer_set_nlsml(VoskRecognizer *recognizer, int nlsml);
    ```

#### Audio Data Handling
- **Accepting Waveform Data**
    ```c
    int vosk_recognizer_accept_waveform(VoskRecognizer *recognizer, const char *data, int length);
    ```
    Accepts and processes audio data in PCM 16-bit mono format.
    - **Parameters**:
        - `data`: Pointer to audio data.
        - `length`: Length of the audio data.
    - **Returns**: 1 if silence occurred, 0 if processing continues, -1 if an error occurs.

- **Results Retrieval**
    - **Final Recognition Result**
        ```c
        const char *vosk_recognizer_result(VoskRecognizer *recognizer);
        ```
    - **Partial Recognition Result**
        ```c
        const char *vosk_recognizer_partial_result(VoskRecognizer *recognizer);
        ```
    - **Final Result without Waiting for Silence**
        ```c
        const char *vosk_recognizer_final_result(VoskRecognizer *recognizer);
        ```

#### Recognizer Management
- **Resetting the Recognizer**
    ```c
    void vosk_recognizer_reset(VoskRecognizer *recognizer);
    ```

- **Freeing the Recognizer**
    ```c
    void vosk_recognizer_free(VoskRecognizer *recognizer);
    ```

#### Logging and GPU Management
- **Setting Logging Level**
    ```c
    void vosk_set_log_level(int log_level);
    ```

- **GPU Initialization**
    ```c
    void vosk_gpu_init();
    void vosk_gpu_thread_init();
    ```

### Batch Processing
- **Creating a Batch Model**
    ```c
    VoskBatchModel *vosk_batch_model_new(const char *model_path);
    ```

- **Releasing Batch Model**
    ```c
    void vosk_batch_model_free(VoskBatchModel *model);
    ```

- **Creating a Batch Recognizer**
    ```c
    VoskBatchRecognizer *vosk_batch_recognizer_new(VoskBatchModel *model, float sample_rate);
    ```

- **Accepting Batch Waveform Data**
    ```c
    void vosk_batch_recognizer_accept_waveform(VoskBatchRecognizer *recognizer, const char *data, int length);
    ```

- **Returning Results from Batch Recognizer**
    ```c
    const char *vosk_batch_recognizer_front_result(VoskBatchRecognizer *recognizer);
    ```

### Conclusion
This header file provides a comprehensive API for integrating speech recognition capabilities into applications using the Vosk framework. Each function and structure is designed to facilitate the building and operation of a speech recognition system that can handle real-time audio input, batch processing, and speaker identification.

---
