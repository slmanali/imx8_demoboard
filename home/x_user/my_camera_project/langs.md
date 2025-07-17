# Documentation for `langs.json`

The `langs.json` file is a configuration file containing the localization strings for a multilingual application. It provides language-specific phrases and labels to be used within the application interface. The file is structured as a JSON object with language codes as keys, and each key contains various sections that align with different functionalities of the application.

## File Structure

The JSON file is categorized into sections based on the language. The following sections are included:

### 1. Language Categories

Each supported language has its own key:

- `lang_eng`: English
- `lang_rus`: Russian
- `lang_ara`: Arabic

In addition, there is a `languagesList` that contains an array of supported languages.

### 2. Default Tab Translations

Each language category contains a `defaulttab` object that lists the primary commands and navigation items in that language. Here are the components of this object:

- **Key-Value Pairs:** Each key is a command or navigational item, and each value is its translation in the respective language.
  
Example for English:
```json
"defaulttab": {
    "langs": "1-LANGS",
    // ... other entries
}
```

### 3. Navigation Tab

Each language section includes a `Navigationtab` array containing navigation items in that language.

Example for English:
```json
"Navigationtab": [
    "1 - DOCUMENT", 
    "2 - TASK", 
    "3 - VIDEO", 
    "4 - LANGS",
    "5 - AUDIO",
    "6 - EXIT"
]
```

### 4. Video Tab

Grouped under `Videotab`, this array lists video control commands available in the current language.

Example for English:
```json
"Videotab": [
    "1 - STOP", 
    "2 - PAUSE", 
    // ... other entries
]
```

### 5. Standalone Tab

The `standalonetab` object contains messages and commands specifically for a standalone mode of the application. This includes:

- Titles and labels for user interface components.
- Feedback messages that indicate the status or action.

Example for English:
```json
"standalonetab": {
    "msgboxtitle": "INFORMATION: ",
    // ... other entries
}
```

### 6. Language and Audio Command Tabs

The relevant sections `languagestab`, `audiocomandtab`, and `audiotab` provide translations for language selection options and audio control commands respectively.

Example for English:
```json
"languagestab": {
    "arabic": "ARABIC",
    // ... other languages
}
```

### 7. Grammar Rules

Each language section includes a `grammar` array which lists the command words and phrases that the application recognizes for voice commands.

Example for English:
```json
"grammar": [
    "close", "first", "second", 
    // ... other entries
]
```

### 8. Vosk Model

The `vosk_model` property specifies the location of the Vosk speech recognition model for each language. This allows the application to utilize the appropriate model for voice commands based on the selected language.

Example for English:
```json
"vosk_model": "vosk-model-small-en-us-0.15"
```

### 9. Error Messages

The `error_message` object contains predefined error strings for common issues that may occur during application operation.

Example for English:
```json
"error_message": {
    "NOWIFI": "WIFI DISABLED",
    // ... other entries
}
```

### 10. PDF Message

The `pdf_message` object consisting of strings related to PDF operations that may display information related to documents within the application context.

Example for English:
```json
"pdf_message": {
    "name": "Report",
    // ... other entries
}
```

## Languages List

The `languagesList` key contains an array of supported languages, where each language is represented as an object mapping of the language name displayed to the user and its corresponding key in the JSON.

### Example:
```json
"languagesList": [
    {"English": "lang_eng"},
    {"عربي": "lang_ara"},
    {"Русский": "lang_rus"}
]
```

## Conclusion

This `langs.json` structure allows for a flexible way to manage localizable strings within the application, ensuring that it can provide a consistent user experience across multiple languages. By modifying this file, developers can easily add new languages or update existing translations without altering the core application logic.

---
