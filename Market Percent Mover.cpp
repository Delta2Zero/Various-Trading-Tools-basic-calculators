#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib> // for system("cls")

#ifdef _WIN32
#include <windows.h>
#endif

void resizeConsoleWindow() {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    if (console != NULL) {
        // MoveWindow(hwnd, x, y, width_px, height_px, repaint)
        MoveWindow(console, 100, 100, 500, 500, TRUE); // Width and height in pixels
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

bool isNumber(const std::string& s, double& outVal) {
    std::istringstream iss(s);
    return (iss >> outVal) && (iss.eof());
}

int main() {
    resizeConsoleWindow(); // set up console size
    
    std::string inputPrice, inputPercent;

    while (true) {
        double marketPrice = 0.0;
        double percentMove = 0.0;

        // Get market price input
        std::cout << "Enter Market Price Entry: ";
        std::getline(std::cin, inputPrice);

        if (inputPrice == "exit" || inputPrice == "EXIT") break;
        if (inputPrice == "clear" || inputPrice == "CLEAR") {
            system("cls"); // Clear console on Windows
            continue;
        }
        if (!isNumber(inputPrice, marketPrice)) {
            std::cout << "Invalid input. Restarting...\n\n";
            continue;
        }

        // Get percentage input
        std::cout << "Enter Market Price Movement (%): ";
        std::getline(std::cin, inputPercent);

        if (inputPercent == "exit" || inputPercent == "EXIT") break;
        if (inputPercent == "clear" || inputPercent == "CLEAR") {
            system("cls");
            continue;
        }
        if (!isNumber(inputPercent, percentMove)) {
            std::cout << "Invalid input. Restarting...\n\n";
            continue;
        }

        // Calculations
        double priceMovement = marketPrice * (percentMove / 100.0);
        double exitPrice = marketPrice + priceMovement;
        double makerBuffered = exitPrice + (exitPrice * 0.006); // 0.6%
        double takerBuffered = exitPrice + (exitPrice * 0.007); // 0.7%

        // Output block
        std::cout << "\n*****************************************\n";
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Market Price Entry:               " << marketPrice << "\n";
        std::cout << "Market Price Movement:           " << percentMove << "%\n";
        std::cout << "Market Price Exit:               " << exitPrice << "\n";
        std::cout << "Market Price with Fee Buffer Maker: " << makerBuffered << "\n";
        std::cout << "Market Price with Fee Buffer Taker: " << takerBuffered << "\n";
        std::cout << "*****************************************\n\n";
    }

    std::cout << "Program exited.\n";
    return 0;
}

