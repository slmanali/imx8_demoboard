#include <iostream>
#include "/home/x_user/my_camera_project/PDFCreator.h"
// g++ pdf_creator.cpp PDFCreator.h -o pdfdemo -lhpdf -lpng -lz

std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();

}

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