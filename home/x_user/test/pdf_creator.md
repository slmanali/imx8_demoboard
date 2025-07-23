# Code Documentation for `pdf_creator.cpp`

## Overview
The file `pdf_creator.cpp` is a C++ program that demonstrates the functionality of creating and manipulating PDF files using a class called `PDFCreator`. The program includes text and image insertion capabilities, allows for easy resetting of the PDF content, and saves the final output to a specified file. 

## Include Directives
```cpp
#include <iostream>
#include "PDFCreator.h"
```
- `<iostream>`: This header is included to enable input and output functionality, particularly for printing messages to the console.
- `"PDFCreator.h"`: This custom header file presumably contains the declaration of the `PDFCreator` class, which encapsulates the logic for PDF creation and manipulation.

## Compilation Command
To compile this code, the following command is used:
```bash
g++ pdf_creator.cpp PDFCreator.h -o pdfdemo -lhpdf -lpng -lz
```
- `g++`: The GNU C++ compiler.
- `-o pdfdemo`: Specifies the output executable name as `pdfdemo`.
- `-lhpdf -lpng -lz`: Links against the necessary libraries for handling PDF files, PNG images, and compression (zlib).

## Function Definitions

### 1. `getCurrentDateTime`
```cpp
std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
```
- **Purpose**: This function retrieves the current date and time, formats it as a string, and returns it.
- **Logic**:
  - Uses `std::chrono` to obtain the current system time.
  - Converts this time to `time_t` format for easier manipulation.
  - Formats the time into a human-readable string using `std::put_time`.
- **Return Value**: A string representation of the current date and time formatted as "YYYY-MM-DD HH:MM:SS".

### 2. `main`
```cpp
int main() {
    try {
        PDFCreator pdf;  
        pdf.reset();
        // Add text
        for (int i = 1; i <= 20; ++i) {
            pdf.addText("Главная страница приложения - " + getCurrentDateTime());
        }
        
        // Add image
        pdf.addImage("screenshot.png");
        
        // Add more text
        pdf.addText("Oткрытый режим ");
        
        // Save to file
        pdf.saveToFile("Отчёт_.pdf");
        
        std::cout << "PDF created successfully!" << std::endl;

        /////////////

        pdf.reset();
        // Add text
        for (int i = 1; i <= 20; ++i) {
            pdf.addText("Line " + std::to_string(i));
        }
        
        // Add image
        pdf.addImage("screenshot.png");
        
        // Add more text
        pdf.addText("Text after image");
        
        // Save to file
        pdf.saveToFile("output1.pdf");
        
        std::cout << "PDF created successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```
- **Purpose**: The `main` function serves as the entry point of the program and orchestrates the PDF creation process by utilizing the `PDFCreator` class.
- **Logic**:
  - A `PDFCreator` object (`pdf`) is instantiated.
  - The `reset` method is called on the `pdf` object to clear any previous content.
  - A loop adds 20 lines of text to the PDF with the heading "Главная страница приложения - " followed by the current date and time.
  - An image, `screenshot.png`, is added to the PDF.
  - Additional text "Oткрытый режим" is inserted.
  - The PDF is saved to a file named "Отчёт_.pdf".
  - A success message is printed to the console.
  - The process is repeated with the same steps in a second instance; however, the text is altered to demonstrate different content. The file is saved as "output1.pdf".
- **Error Handling**: A try-catch block is used to catch exceptions. If an exception occurs, an error message is printed, and the program returns a non-zero status indicating failure.

## Additional Information
### Assumed Functionality of PDFCreator
While the implementation details of the `PDFCreator` class are not provided, it can be assumed that the following methods are part of the class:
- `reset()`: Resets the PDF document to an empty state.
- `addText(const std::string&)`: Adds a specified text string to the PDF.
- `addImage(const std::string&)`: Inserts an image into the PDF.
- `saveToFile(const std::string&)`: Saves the current PDF content to a file with the specified name.

### Notes
- The program can be extended to include more functionalities, such as adding multiple pages, changing fonts and sizes, or customizing image placements.
- When running the code, ensure that the image file "screenshot.png" exists in the same directory, or else an error will be raised when attempting to add it to the PDF.

---