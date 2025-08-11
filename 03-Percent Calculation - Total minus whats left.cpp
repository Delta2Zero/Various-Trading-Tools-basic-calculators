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
    std::string totalInput, leftInput;
    
    while (true) {
        double totalAmount = 0.0, amountLeft = 0.0;

        // Prompt for total amount
        std::cout << "\n*******************************************************************\n";
        std::cout << "*******************************************************************\n";
        std::cout << "Please type the amount to continue or type exit to exit the program\n\n";

        std::cout << "Total Amount Representing 100%: ";
        std::getline(std::cin, totalInput);
        if (totalInput == "exit") break;

        std::stringstream totalStream(totalInput);
        if (!(totalStream >> totalAmount)) {
            std::cout << "Invalid number. Restarting...\n";
            continue;
        }

        // Prompt for remaining amount
        std::cout << "Amount Left/Gain: ";
        std::getline(std::cin, leftInput);
        if (leftInput == "exit") break;

        std::stringstream leftStream(leftInput);
        if (!(leftStream >> amountLeft)) {
            std::cout << "Invalid number. Restarting...\n";
            continue;
        }

        // Calculation
        double difference = totalAmount - amountLeft;
        double percentageDifference1 = (difference / totalAmount) * 100.0;
        double percentageDifference2 = (amountLeft / difference) * 100.0;

        // Output
        std::cout << "\n*******************************\n";
        std::cout << "*******************************\n\n";

        std::cout << std::fixed << std::setprecision(2);

        std::cout << "Total amount: " << totalAmount << "\n";
        std::cout << "Amount left: " << amountLeft << "\n";
        std::cout << "Difference: " << difference << "\n";
        std::cout << "Percentage in Loss (Total Amount / Difference): " << percentageDifference1 << "%\n\n";
        std::cout << "Percentage in Gain(Amount Left / Difference): " << percentageDifference2 << "%\n\n";

        std::cout << "*******************************\n";
        std::cout << "*******************************\n";
    }

    std::cout << "\nProgram exited.\n";
    return 0;
}
