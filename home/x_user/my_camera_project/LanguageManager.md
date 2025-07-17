Certainly! Below is a detailed documentation for the `LanguageManager` class, defining its structure, purpose, and the functionality provided through its methods and attributes.

---

# LanguageManager Class Documentation

## Overview
The `LanguageManager` class provides functionality to manage multiple languages for an application. It allows loading language definitions from a JSON file, setting the current language, and retrieving translations for specific keys and sections. This class serves as a central point for localization in an application.

## Include Guards
```cpp
#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H
```
These lines prevent multiple inclusions of the header file.

## Required Headers
```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <algorithm>
#include "Logger.h"
```
### Description:
- `iostream`: For input and output operations.
- `fstream`: For file input and output streams.
- `sstream`: For string stream handling.
- `string`: To use the string class.
- `map`: For mapping display names to language codes.
- `nlohmann/json.hpp`: For handling JSON operations.
- `stdexcept`: For standard exceptions.
- `algorithm`: For various algorithms (not utilized in the current class but potentially useful).
- `Logger.h`: Custom logger header for logging informational and error messages.

## Typedefs
```cpp
using json = nlohmann::json;
```
This statement defines a shortcut type `json` for the `nlohmann::json` namespace, simplifying its usage throughout the class.

## Class Definition
```cpp
class LanguageManager {
```
The `LanguageManager` class is defined here.

### Constructor
```cpp
LanguageManager(const std::string _default_language, const std::string& jsonFilePath = "/home/x_user/my_camera_project/langs.json")
```
**Parameters:**
- `_default_language`: The language code for the default language to be set initially.
- `jsonFilePath`: The file path from where the language entries will be loaded (default is `/home/x_user/my_camera_project/langs.json`).

**Functionality:**
- Reads the JSON file from the specified path.
- Parses the languages and their translations from the JSON file.
- Sets the default language using `setLanguage`.

### Member Variables
- `std::string default_language`: Holds the current default language being used.
- `json langs`: Holds the parsed JSON data containing languages and translations.
- `json translations`: Stores the translations for the currently set language.
- `std::map<std::string, std::string> language_keys`: Maps display names to their corresponding language codes.

### Methods

#### `void setLanguage(const std::string& lang)`
**Parameters:**
- `lang`: The language code to set as the current language.

**Functionality:**
- Sets the current language based on the provided code.
- Logs an error message and throws an exception if the language is unsupported.

#### `std::string getDefaultLanguage() const`
**Returns:**
- `default_language`: The currently set default language code.

#### `std::string getText(const std::string& section, const std::string& key)`
**Parameters:**
- `section`: The section in the JSON containing the translation.
- `key`: The specific key for the text to be retrieved.

**Returns:**
- The translated text if found; otherwise, it returns the key as a fallback and logs an error.

#### `std::string getVosk()`
**Returns:**
- The VOSK model name for speech recognition based on current translations.
- An empty string and logs an error if the retrieval fails.

#### `std::string getGrammar()`
**Returns:**
- A JSON-formatted string representing the grammar definitions for the current language.
- Returns an empty array (`[]`) if the grammar section is not an array or if retrieval fails, logging the corresponding error.

#### `std::string getSection(const std::string& section)`
**Parameters:**
- `section`: The name of the section to retrieve from the current translations.

**Returns:**
- A JSON string representation of the section's contents.
- An empty JSON object (`{}`) if an error occurs during retrieval, logging the error.

## Error Handling
The class includes robust error handling for file access, JSON parsing, and retrieval operations. It uses logging to capture error information, which can be helpful for debugging when the application runs into issues.

## Conclusion
The `LanguageManager` class is a crucial component for implementing multi-language support in an application. It effectively abstracts the complexity of loading and retrieving language data, allowing developers to focus on adding translations without worrying about underlying logic.

--- 

This documentation can serve as a comprehensive reference for anyone looking to understand or modify the `LanguageManager` class in future development efforts.

---
