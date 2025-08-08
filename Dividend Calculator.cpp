#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

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

int main()
{
    resizeConsoleWindow(); // set up console size
    
    std::string input;
    double dividendLast = 0.0;
    double shares = 0.0;

    while (true)
    {
        std::cout << "\nEnter Dividend per Share (type 'exit' to quit): ";
        std::getline(std::cin, input);
        if (input == "exit") break;

        std::stringstream ssDividend(input);
        if (!(ssDividend >> dividendLast)) {
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::cout << "Enter Number of Shares Owned (type 'exit' to quit): ";
        std::getline(std::cin, input);
        if (input == "exit") break;

        std::stringstream ssShares(input);
        if (!(ssShares >> shares)) {
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        double totalIncome = dividendLast * shares;
        double tenPercenttax = totalIncome * 0.1;
        double taxedIncome = totalIncome - tenPercenttax;
        double monthlyIncome = taxedIncome / 12.0;
        

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n========== Dividend Summary ==========\n";
        std::cout << "Dividend: PHP " << dividendLast << "\n";
        std::cout << "Shares: " << shares << "\n";
        std::cout << "Dividend Paid Total (Pre-Tax): PHP " << totalIncome << "\n";
        std::cout << "Dividend Paid Total (10% Tax): PHP " << taxedIncome << "\n";
        std::cout << "Dividend Paid Per Month: PHP " << monthlyIncome << "\n";
        std::cout << "======================================\n";
    }

    std::cout << "\nExited program. Goodbye!\n";
    return 0;
}
