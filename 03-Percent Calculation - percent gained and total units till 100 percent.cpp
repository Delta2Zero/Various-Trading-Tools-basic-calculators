#include <iostream>
#include <string>
#include <limits>  // For std::numeric_limits

#ifdef _WIN32
#include <windows.h>
#endif


// Function to safely convert a string to a double
bool stringToDouble(const std::string& str, double& value) {
    try {
        value = std::stod(str);  // Try converting to double
        return true;
    }
    catch (...) {
        return false;  // Return false if the conversion fails
    }
}


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
    while (true) {
        std::string input;
        double percentGain = 0.0;
        double quantity = 0.0;

        std::cout << "Enter percent: ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        if (!stringToDouble(input, percentGain) || percentGain <= 0) {
            std::cout << "Invalid input! Please enter a valid number greater than zero for percent.\n\n";
            continue;
        }

        std::cout << "Enter quantity for that percentage (e.g. 1): ";
        std::getline(std::cin, input);

        if (input == "exit") break;

        if (!stringToDouble(input, quantity) || quantity <= 0) {
            std::cout << "Invalid input! Please enter a valid number greater than zero for quantity.\n\n";
            continue;
        }

        double totalUnits = (100.0 * quantity) / percentGain;

        std::cout << "\n*****************************************************\n";
        std::cout << "*******************************************************\n";
        std::cout << "Percent gain per unit: " << percentGain << "%\n";
        std::cout << "Units counted: " << quantity << "\n";
        std::cout << "Estimated total units to reach 100%: " << totalUnits << "\n";
        std::cout << "*******************************************************\n";
        std::cout << "*****************************************************\n\n";
    }

    return 0;
}

