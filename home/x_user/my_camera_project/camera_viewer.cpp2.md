# Documentation for `second-half-camera-viewer.c`

## Overview
The `CameraViewer` class is a central component of a camera-based user interface application, managing various functionalities including audio control, QR code processing, file handling, and camera control. This class provides methods to transition between different modes, handle user commands, and manage multimedia content such as PDFs and videos. 

## Methods

### 1. `void CameraViewer::complete_standalone_transition(bool _NOWIFI)`

#### Description
Completes the transition into standalone mode by adjusting audio settings and managing downloads. Depending on the presence of a WiFi connection, it may initiate a download process or update the application's status to standalone.

#### Parameters
- `bool _NOWIFI`: A flag indicating whether the transition should occur without using WiFi.

#### Exceptions
Logs errors related to audio control and camera thread operations.

### 2. `std::string CameraViewer::toUpperCase(const std::string& input)`

#### Description
Converts a given string to uppercase using wide character conversion to support UTF-8 strings.

#### Parameters
- `const std::string& input`: The input string to be converted.

#### Returns
- `std::string`: The uppercase version of the input string.

### 3. `std::string CameraViewer::getCurrentDateTime()`

#### Description
Retrieves the current date and time as a formatted string.

#### Returns
- `std::string`: The current date and time in the format "YYYY-MM-DD HH:MM:SS".

### 4. `void CameraViewer::handle_command_recognize(std::string _command)`

#### Description
Processes user commands that have been recognized. Depending on the application's current scenario (such as standalone, audio control, etc.), it carries out the corresponding actions.

#### Parameters
- `std::string _command`: The command to be processed.

#### Exceptions
Logs errors that occur during the command handling process.

### 5. `void CameraViewer::AudioReset()`

#### Description
Resets the audio settings to their default values.

### 6. `void CameraViewer::changeLanguage(std::string _lang)`

#### Description
Changes the application's language settings and updates UI elements accordingly.

#### Parameters
- `std::string _lang`: The language code for the desired language.

### 7. `void CameraViewer::Enter_Low_Power_Mode()`

#### Description
Enters low power mode by disabling certain hardware functionalities.

### 8. `void CameraViewer::Exit_Low_Power_Mode()`

#### Description
Exits low power mode and restores previous settings.

### 9. `void CameraViewer::setVisors(std::string _value)`

#### Description
Sets the state of the visors (HDMI on/off) by writing to a specified GPIO file.

#### Parameters
- `std::string _value`: "0" to disable, "1" to enable.

### 10. `void CameraViewer::setCamera(std::string _value)`

#### Description
Sets the camera state by configuring the related GPIOs.

#### Parameters
- `std::string _value`: "0" to disable, "1" to enable.

### 11. `void CameraViewer::send_audio_settings()`

#### Description
Sends the current audio settings to the session.

### 12. `void CameraViewer::start_audio_channel()`

#### Description
Initializes and starts the audio playback and streaming channels.

### 13. `void CameraViewer::close_audio_channel()`

#### Description
Closes any active audio channels.

### 14. `void CameraViewer::process_event(nlohmann::json _data)`

#### Description
Processes incoming events based on the provided JSON data, updating the state as necessary.

### 15. `void CameraViewer::showdefaultstandalone()`

#### Description
Displays the default standalone interface, including a PDF representation of the current state.

### 16. `void CameraViewer::showpdfmode()`, `void CameraViewer::showtxtmode()`, `void CameraViewer::showvideomode()`

#### Description
Updates the UI to show listings or views appropriate for each content type (PDFs, text files, videos).

### 17. `void CameraViewer::showFilesList(const std::string &folder_path, const std::string &suffix)`

#### Description
Displays a list of files in a specified folder filtered by a file type suffix.

#### Parameters
- `const std::string &folder_path`: The directory to search for files.
- `const std::string &suffix`: The file extension to filter by.

### 18. `void CameraViewer::LoadPDF(const std::string &full_path)`, `void CameraViewer::LoadMP4(const std::string &full_path)`

#### Description
Loads a PDF or MP4 file and prepares it for viewing or playback.

### 19. `void CameraViewer::LoadTXT(const std::string &full_path)`

#### Description
Loads a text file and processes its contents, potentially linking to related documents.

### 20. `void CameraViewer::displayTasks()`

#### Description
Displays the current tasks associated with the application and updates UI elements.

### 21. `void CameraViewer::start_qrcode()` and `void CameraViewer::stop_qrcode()`

#### Description
Initiates or terminates the QR code scanning process, updating UI labels appropriately.

### 22. `std::string CameraViewer::base64_decode_openssl(const std::string &encoded)`

#### Description
Decodes a Base64 encoded string using OpenSSL.

### 23. `std::string CameraViewer::aes_decrypt_ecb(const std::string &cipherText, const std::string &key)`

#### Description
Decrypts a provided ciphertext using AES in ECB mode.

### 24. `void CameraViewer::processQRCode(cv::Mat _frame)`

#### Description
Processes a frame to detect and decode QR codes, handling any resulting data securely.

### 25. `void CameraViewer::batteryiconchange(PowerManagement::BatteryStatus status)`

#### Description
Updates the battery status icon based on the current battery state.

---

### Threads and Concurrency
The application employs multi-threading, particularly for handling tasks such as language changes, media downloading, and QR code processing. It utilizes `QtConcurrent` for running blocking operations on separate threads to ensure a responsive UI.

### Error Handling
The class extensively logs errors, providing traceability for issues that arise during its execution. Each method contains try-catch blocks to handle exceptions and uses logging for error reporting.

### UI Operations
The methods are designed to update UI components such as `QListWidgets`, `QLabels`, and `QGraphicsScenes` dynamically based on user actions or system events. This ensures that the user's experience is fluid and responsive to changes.

### Conclusion
The `CameraViewer` class is a complex component handling multiple aspects of camera-based application functionality, including audio management, file handling, QR code processing, and user interaction. The documentation provides a comprehensive overview of each method, making the class more maintainable and understandable for future developers and contributors.

---
