#include <iostream>
#include "PDFCreator.h"

// void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
//     std::cerr << "ERROR: code=" << error_no << " (0x" << std::hex << error_no << "), detail=" << std::dec << detail_no << std::endl;
//     throw std::runtime_error("PDF error");
// }

// HPDF_Page ensurePageHasSpace(HPDF_Doc pdf, HPDF_Page currentPage, float& currentY, float lineSpacing, float bottomMargin) {
//     if (currentY - lineSpacing < bottomMargin) {
//         HPDF_Page_EndText(currentPage);
//         currentPage = HPDF_AddPage(pdf);
//         if (!currentPage) {
//             throw std::runtime_error("Cannot add new page");
//         }
//         HPDF_Page_SetSize(currentPage, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
//         currentY = HPDF_Page_GetHeight(currentPage) - 50;
//         HPDF_Page_SetFontAndSize(currentPage, HPDF_GetFont(pdf, "Helvetica", nullptr), 24);
//         HPDF_Page_BeginText(currentPage);
//     }
//     return currentPage;
// }

// int main() {
//     HPDF_Doc pdf = HPDF_New(error_handler, nullptr);
//     if (!pdf) {
//         std::cerr << "HPDF_New failed to create PDF document" << std::endl;
//         return 1;
//     }

//     try {
//         HPDF_Page page = HPDF_AddPage(pdf);
//         HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
//         HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
//         float fontSize = 24;
//         float lineSpacing = fontSize * 1.2;
//         float bottomMargin = 50;
//         float currentY = HPDF_Page_GetHeight(page) - 50;

//         HPDF_Page_SetFontAndSize(page, font, fontSize);
//         HPDF_Page_BeginText(page);

//         std::string texts[] = {
//             "Working PDF!", "Line 2: Testing page fill", "Line 3: More content",
//             "Line 4: Keep going", "Line 5", "Line 6", "Line 7", "Line 8",
//             "Line 9", "Line 10", "Line 11", "Line 12", "Line 13", "Line 14", "Line 15",
//             "Line 16", "Line 17", "Line 18", "Line 19", "Line 20", "Line 21", "Line 22",
//             "Line 23", "Line 24", "Line 25", "Line 26", "Line 27", "Line 28", "Line 29"
//         };

//         for (const auto& text : texts) {
//             page = ensurePageHasSpace(pdf, page, currentY, lineSpacing, bottomMargin);
//             HPDF_Page_TextOut(page, 50, currentY, text.c_str());
//             currentY -= lineSpacing;
//         }

//         HPDF_Page_EndText(page);

//         HPDF_SaveToFile(pdf, "output.pdf");
//         HPDF_Free(pdf);
//         std::cout << "PDF created successfully!" << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Exception: " << e.what() << std::endl;
//         HPDF_Free(pdf);
//         return 1;
//     }

//     return 0;
// }

int main() {
    try {
        PDFCreator pdf;        
        pdf.init();
        // Add text
        for (int i = 1; i <= 20; ++i) {
            pdf.addText("Line " + std::to_string(i));
        }
        
        // Add image
        pdf.addImage("screenshot.png");
        
        // Add more text
        pdf.addText("Text after image");
        
        // Save to file
        pdf.saveToFile("output.pdf");
        
        std::cout << "PDF created successfully!" << std::endl;

        /////////////
        
        pdf.init();
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