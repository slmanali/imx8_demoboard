# PDFCreator Class Documentation

The `PDFCreator` class provides functionality to create and manipulate PDF documents programmatically. This class utilizes the Haru Free PDF Library (libharu) for PDF generation and includes features for adding text and images, managing page layouts, and error handling.

## Header File: PDFCreator.h

```cpp
#include <iostream>
#include <string>
#include <hpdf.h>
#include <stdexcept>
#include "Logger.h"
```

### Overview

The class encapsulates the essential operations required for creating a PDF, including:

- Initializing and resetting the PDF document
- Adding text and images to the PDF
- Managing layout and page transitions
- Error handling via logging

### Usage

To use the `PDFCreator`, instantiate an object, call the `addText` or `addImage` methods to populate the document, and finalize it by calling `saveToFile`. The object is designed to manage all aspects of PDF generation within its methods.

## Class Members

### Private Members

- **HPDF_Doc m_pdf**: The main PDF document object.
- **HPDF_Page m_currentPage**: The currently active page being edited.
- **HPDF_Font m_font**: The font used for text output.
- **float m_currentY**: The current vertical position for text/image placement.
- **float m_fontSize**: The size of the font used for text.
- **float m_topMargin**: The top margin for the page.
- **float m_bottomMargin**: The bottom margin for the page.
- **float m_lineSpacing**: Spacing between lines of text.
- **bool m_textBlockActive**: Indicates whether a text block is currently active.
- **int m_pageCount**: Tracks the total number of pages created.
- **int m_lineCount**: Tracks the total number of lines of text added.

### Private Methods

- **void createNewPage()**: 
  - Creates a new page in the PDF document.
  - If there is an active text block, it ends that block before starting a new page.
  - Adjusts the current Y position based on margins.

- **void ensureSpaceForText()**: 
  - Checks if there is enough space on the current page for new text.
  - If not, it creates a new page.

- **void ensureSpaceForImage(float imageHeight)**: 
  - Checks if there is enough space on the current page for a new image.
  - If there isn't enough space, it creates new pages until space is available.

### Public Methods

- **PDFCreator()**: 
  - Constructor that initializes the PDF document and sets default values for private members.
  - Creates the first page and configures the font.

- **~PDFCreator()**: 
  - Destructor that cleans up the PDF document resources.

- **void addText(const std::string& text)**: 
  - Adds a line of text to the current page.
  - Manages the layout by ensuring adequate space is available.

- **void reset()**: 
  - Resets the internal state of the PDF creator and starts a new document.

- **void addImage(const std::string& imagePath)**: 
  - Adds a JPEG image (after compressing) to the current PDF page.
  - Manages layout and ensures there is enough space.

- **void addImage1(const std::string& imagePath)**: 
  - Adds a PNG image to the current PDF page.
  - Checks for image validity and ensures layout space.

- **void saveToFile(const std::string& filename)**: 
  - Saves the generated PDF document to a specified file location.
  - Manages the ending of any active text blocks before saving.

- **static void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void*)**: 
  - A static error handling function for the PDF library that logs errors and throws exceptions on failure.

- **int getPageCount() const**: 
  - Returns the total number of pages created in the PDF document.

- **int getLineCount() const**: 
  - Returns the total number of lines of text that have been added to the document.

### Copy Semantics

- **PDFCreator(const PDFCreator&) = delete**: 
  - Copy constructor is deleted to prevent unwanted copies of PDFCreator instances.

- **PDFCreator& operator=(const PDFCreator&) = delete**: 
  - Copy assignment operator is deleted to prevent unwanted assignments.

### Error Handling

The class utilizes the `Logger` class to log errors and information throughout its operations. In case of critical failures (like failed PDF creation, font loading issues, etc.), exceptions are thrown to signal errors to the caller.

### Notes

- Make sure to link against the Haru PDF library when compiling this code.
- Ensure that the QImage class from Qt is available for image manipulation.
- The default font path in the `reset()` method may need updates based on the environment.

### Example Usage

```cpp
PDFCreator pdfCreator;
try {
    pdfCreator.addText("Hello, World!");
    pdfCreator.addImage("example_image.png");
    pdfCreator.saveToFile("output.pdf");
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

This example initializes the `PDFCreator`, adds text and an image to the PDF, and saves the file, handling any errors that arise during the process.

---
