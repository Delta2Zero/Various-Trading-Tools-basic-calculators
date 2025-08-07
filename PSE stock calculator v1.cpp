#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // for std::setprecision
#include <limits>  // for std::numeric_limits
#include <cstdlib> // for atof

#ifdef _WIN32
#include <windows.h>
#endif

void resizeConsoleWindow() {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    if (console != NULL) {
        // MoveWindow(hwnd, x, y, width_px, height_px, repaint)
        MoveWindow(console, 100, 100, 650, 500, TRUE); // Width and height in pixels
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
    
    std::vector<LotRule> rules;
    rules.push_back((LotRule){0.0001, 0.0099, 0.0001, 1000000});
    rules.push_back((LotRule){0.0100, 0.0490, 0.0010, 100000});
    rules.push_back((LotRule){0.0500, 0.2490, 0.0010, 10000});
    rules.push_back((LotRule){0.25, 0.495, 0.005, 10000});
    rules.push_back((LotRule){0.5000, 4.99, 0.01, 1000});
    rules.push_back((LotRule){5, 9.99, 0.01, 100});
    rules.push_back((LotRule){10, 19.98, 0.02, 100});
    rules.push_back((LotRule){20, 49.95, 0.05, 100});
    rules.push_back((LotRule){50, 99.95, 0.05, 10});
    rules.push_back((LotRule){100, 199.9, 0.1, 10});
    rules.push_back((LotRule){200, 499.8, 0.2, 10});
    rules.push_back((LotRule){500, 999.5, 0.5, 10});
    rules.push_back((LotRule){1000, 1999, 1, 5});
    rules.push_back((LotRule){2000, 4998, 2, 5});
    rules.push_back((LotRule){5000, 1e9, 5, 5});

    std::string input;
    while (true) {
        std::cout << "\nEnter current market price of the stock (or type 'exit' to quit): ";
        std::getline(std::cin, input);

        if (input == "exit" || input == "EXIT" || input == "Exit") {
            std::cout << "Exiting program. Goodbye!\n";
            break;
        }

        double marketPrice = std::atof(input.c_str());

        if (marketPrice <= 0) {
            std::cout << "Invalid input. Please enter a valid number or type 'exit'.\n";
            continue;
        }

        bool matched = false;
        for (size_t i = 0; i < rules.size(); ++i) {
            LotRule rule = rules[i];
            if (marketPrice >= rule.minPrice && marketPrice <= rule.maxPrice) {
                double roundedPrice = roundToTick(marketPrice, rule.tickSize);
                int shares = rule.lotSize;
                double gross = roundedPrice * shares;

                // --- Fee calculations ---
                double commission = gross * 0.0025;
                double vat = commission * 0.12;
                double pseFee = gross * 0.00005;
                double sccpFee = gross * 0.0001;

                double totalFees = commission + vat + pseFee + sccpFee;
                double totalCost = gross + totalFees;

                std::cout << "\n--- Stock Purchase Info ---\n";
                std::cout << "Tick Size     : " << rule.tickSize << "\n";
                std::cout << "Lot Size      : " << rule.lotSize << " shares\n";
                std::cout << std::fixed << std::setprecision(4);
                std::cout << "Rounded Price : PHP " << roundedPrice << " per share\n";

                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Gross Amount  : PHP " << gross << "\n";
                std::cout << "Commission    : PHP " << commission << "\n";
                std::cout << "VAT (12%)     : PHP " << vat << "\n";
                std::cout << "PSE Fee       : PHP " << pseFee << "\n";
                std::cout << "SCCP Fee      : PHP " << sccpFee << "\n";
                std::cout << "Total Fees    : PHP " << totalFees << "\n";
                std::cout << "Total Cost    : PHP " << totalCost << "\n";
                matched = true;
                break;
            }
        }

        if (!matched) {
            std::cout << "No matching lot rule found. Please check the price range.\n";
        }
    }

    return 0;
}

