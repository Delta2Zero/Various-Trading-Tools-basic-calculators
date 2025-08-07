#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>

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

struct LotRule {
    double minPrice;
    double maxPrice;
    double tickSize;
    int lotSize;
};

int getLotSize(double price, const std::vector<LotRule>& rules) {
    for (const auto& rule : rules) {
        if (price >= rule.minPrice && price <= rule.maxPrice) {
            return rule.lotSize;
        }
    }
    return 1; // default fallback
}

int main() {
    resizeConsoleWindow(); // set up console size
    
    std::vector<LotRule> rules = {
        {0.0001, 0.0099, 0.0001, 1000000},
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
        std::cout << "\nEnter stock price (or 'exit'): ";
        std::getline(std::cin, input);
        if (input == "exit") break;

        double price = atof(input.c_str());
        if (price <= 0) {
            std::cout << "Invalid stock price. Try again.\n";
            continue;
        }

        int lotSize = getLotSize(price, rules);

        std::cout << "Enter dividend per share: ";
        std::getline(std::cin, input);
        double dividend = atof(input.c_str());
        if (dividend < 0) {
            std::cout << "Invalid dividend. Try again.\n";
            continue;
        }

        double totalCost = price * lotSize;
        double totalDividend = dividend * lotSize;
        double yield = (totalDividend / totalCost) * 100.0;

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\nStock Price: PHP " << price << "\n";
        std::cout << "Board Lot: " << lotSize << " shares\n";
        std::cout << "Dividend per Share: PHP " << dividend << "\n";
        std::cout << "Total Cost: PHP " << totalCost << "\n";
        std::cout << "Total Dividend: PHP " << totalDividend << "\n";
        std::cout << "Dividend Yield: " << yield << "%\n";
    }

    return 0;
}

