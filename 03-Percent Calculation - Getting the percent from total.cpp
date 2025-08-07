#include <iostream>
#include <iomanip>  // for std::fixed and std::setprecision
#include <string>   // for std::string
#include <sstream>  // for stringstream

#ifdef _WIN32
#include <windows.h>
#endif

void resizeConsoleWindow() {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    if (console != NULL) {
        // MoveWindow(hwnd, x, y, width_px, height_px, repaint)
        MoveWindow(console, 100, 100, 580, 500, TRUE); // Width and height in pixels
    }

    // Set the screen buffer size so lines don't wrap or scroll unnecessarily
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        COORD newSize = {120, 500}; // Width in characters, height in lines
        SetConsoleScreenBufferSize(hOut, newSize);
    }
#endif
}

int main() {
    resizeConsoleWindow(); // set up console size
    std::string totalInput, percentInput;
    
    while (true) {
        double totalAmount = 0.0, percent = 0.0;

        // Prompt for total amount
        std::cout << "\nAmount total (or type 'exit' to quit): ";
        std::getline(std::cin, totalInput);
        if (totalInput == "exit") break;

        std::stringstream totalStream(totalInput);
        if (!(totalStream >> totalAmount)) {
            std::cout << "Invalid number. Restarting...\n";
            continue;
        }

        // Prompt for percent
        std::cout << "Amount in percent (or type 'exit' to quit): ";
        std::getline(std::cin, percentInput);
        if (percentInput == "exit") break;

        std::stringstream percentStream(percentInput);
        if (!(percentStream >> percent)) {
            std::cout << "Invalid number. Restarting...\n";
            continue;
        }

        // Calculation
        double percentValue = totalAmount * (percent / 100.0);
        double remaining = totalAmount - percentValue;

        // Output
        std::cout << "\n*****************************************************\n";
        std::cout << "*****************************************************\n\n";

        std::cout << std::fixed << std::setprecision(2);

        std::cout << "Total Amount: " << totalAmount << "\n";
        std::cout << "Amount in Percent: " << percent << "%\n\n";

        std::cout << "The " << percent << "% of " << totalAmount << " is: " << percentValue << "\n";
        std::cout << "Remaining amount: " << remaining << "\n\n";

        std::cout << "*****************************************************\n";
        std::cout << "*****************************************************\n";
    }

    std::cout << "\nProgram exited.\n";
    return 0;
}

