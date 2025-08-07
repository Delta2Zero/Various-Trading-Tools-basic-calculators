#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // for atof and atoi
#include <iomanip>  // for std::setprecision
#include <fstream>  // for file writing
#include <ctime>    // for date/time
#include <sstream>  // for ostringstream

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

struct LotRule {
    double minPrice;
    double maxPrice;
    double tickSize;
    int lotSize;
};

double roundToTick(double price, double tick) {
    return static_cast<int>(price / tick + 0.5) * tick;
}

int main() {
    resizeConsoleWindow(); // set up console size
    
    std::vector<LotRule> rules{
        {0.0001, 0.0099, 0.0001, 1000000},
   		{0.0100, 0.0490, 0.0010, 100000},
        {0.0500, 0.2490, 0.0010, 10000},
        {0.25, 0.495, 0.005, 10000},
        {0.5000, 4.99, 0.01, 1000},
        {5, 9.99, 0.01, 100},
        {10, 19.98, 0.02, 100},
        {20, 49.95, 0.05, 100},
        {50, 99.95, 0.05, 10},
        {100, 199.9, 0.1, 10},
        {200, 499.8, 0.2, 10},
        {500, 999.5, 0.5, 10},
        {1000, 1999, 1, 5},
        {2000, 4998, 2, 5},
        {5000, 1e9, 5, 5}
    };

    std::string input;
    while (true) {
        std::cout << "\n********************************************************************\n";
        std::cout << "\n                    Type exit to exit the program\n";
        std::cout << "\n                       Written By: ChatGPT\n";
        std::cout << "\n        Prompt Code Checking and Testing by: DeltaTwoZero\n";
        std::cout << "\n********************************************************************\n";
        std::cout << "\nCurrent Market price of the stock: ";
        std::getline(std::cin, input);

        if (input == "exit" || input == "EXIT" || input == "Exit") {
        	std::cout << "\n********************************************************************\n";
            std::cout << "Exiting program. Goodbye!\n";
        	std::cout << "\n********************************************************************\n";
            break;
        }

        double marketPrice = atof(input.c_str());

        if (marketPrice <= 0) {
        	std::cout << "\n********************************************************************\n";
            std::cout << "Invalid input. Please enter a valid number or type 'exit'.\n";
        	std::cout << "\n********************************************************************\n";
            continue;
        }

        std::cout << "Number of shares: ";
        std::getline(std::cin, input);
        int numShares = atoi(input.c_str());

        if (numShares <= 0) {
        	std::cout << "\n********************************************************************\n";
            std::cout << "Invalid number of shares.\n";
        	std::cout << "\n********************************************************************\n";
            continue;
        }

        // Determine tick size and lot size based on market price
        double tickSize = 0.01;
        int lotSize = 1;
        for (const LotRule& rule : rules) {
            if (marketPrice >= rule.minPrice && marketPrice <= rule.maxPrice) {
                tickSize = rule.tickSize;
                lotSize = rule.lotSize;
                break;
            }
        }

        std::string boardType = (numShares % lotSize == 0) ? "Board Lot: Normal" : "Board Lot: Odd";

        double roundedPrice = roundToTick(marketPrice, tickSize);
        double grossAmount = roundedPrice * numShares;
        double commission = grossAmount * 0.0025;
        double vat = commission * 0.12;
        double pseFee = grossAmount * 0.00005;
        double sccpFee = grossAmount * 0.0001;

        double totalFees = commission + vat + pseFee + sccpFee;
        double totalCost = grossAmount + totalFees;

        // Get current date and time
        time_t now = time(0);
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

        // Output to console
        std::cout << "\n--- Stock Purchase Info ---\n";
        std::cout << "Date & Time   : " << dateStr << "\n";
        std::cout << "Number of Shares: " << numShares << " shares\n";
        std::cout << boardType << "\n";
        std::cout << std::fixed << std::setprecision(4);
        std::cout << "Rounded Price : PHP " << roundedPrice << " per share\n";
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Gross Amount  : PHP " << grossAmount << "\n";
        std::cout << "Commission    : PHP " << commission << "\n";
        std::cout << "VAT (12%)     : PHP " << vat << "\n";
        std::cout << "PSE Fee       : PHP " << pseFee << "\n";
        std::cout << "SCCP Fee      : PHP " << sccpFee << "\n";
        std::cout << "Total Fees    : PHP " << totalFees << "\n";
        std::cout << "Total Cost    : PHP " << totalCost << "\n";

        // Output to file
        std::ofstream outFile("C:/Tools/Trading tools/trading books/PSE Stocks Investment Logs.txt", std::ios::app);
        if (outFile.is_open()) {
            std::ostringstream outputStream;
            outputStream << "\n--- Stock Purchase Info ---\n";
            outputStream << "Date & Time   : " << dateStr << "\n";
            outputStream << "Number of Shares: " << numShares << " shares\n";
            outputStream << boardType << "\n";
            outputStream << std::fixed << std::setprecision(4);
            outputStream << "Rounded Price : PHP " << roundedPrice << " per share\n";
            outputStream << std::fixed << std::setprecision(2);
            outputStream << "Gross Amount  : PHP " << grossAmount << "\n";
            outputStream << "Commission    : PHP " << commission << "\n";
            outputStream << "VAT (12%)     : PHP " << vat << "\n";
            outputStream << "PSE Fee       : PHP " << pseFee << "\n";
            outputStream << "SCCP Fee      : PHP " << sccpFee << "\n";
            outputStream << "Total Fees    : PHP " << totalFees << "\n";
            outputStream << "Total Cost    : PHP " << totalCost << "\n";

            outFile << outputStream.str();
            outFile.close();
        } else {
            std::cerr << "Failed to open log file.\n";
        }
    }

    return 0;
}

