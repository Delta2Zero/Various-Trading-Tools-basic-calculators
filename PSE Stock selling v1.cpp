#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cctype> // for isdigit

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

bool isNumber(const std::string& s) {
    bool hasDecimal = false;
    for (size_t i = 0; i < s.length(); ++i) {
        if (!std::isdigit(s[i])) {
            if (s[i] == '.' && !hasDecimal) {
                hasDecimal = true;
            } else {
                return false;
            }
        }
    }
    return !s.empty();
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

        if (!isNumber(input)) {
            std::cout << "Invalid input. Please enter a valid number.\n";
            continue;
        }

        double marketPrice = std::atof(input.c_str());
        if (marketPrice <= 0) {
            std::cout << "Invalid price. Please enter a number above 0.\n";
            continue;
        }

        bool matched = false;
        for (size_t i = 0; i < rules.size(); ++i) {
            LotRule rule = rules[i];
            if (marketPrice >= rule.minPrice && marketPrice <= rule.maxPrice) {
                double roundedPrice = roundToTick(marketPrice, rule.tickSize);
                int lotSize = rule.lotSize;

                std::string shareInput;
                int shares = 0;

                while (true) {
                    std::cout << "Enter number of shares to sell (lot size = " << lotSize << "): ";
                    std::getline(std::cin, shareInput);

                    if (!isNumber(shareInput)) {
                        std::cout << "Invalid input - resetting to price entry.\n";
                        break; // back to market price input
                    }

                    shares = std::atoi(shareInput.c_str());

                    if (shares <= 0) {
                        std::cout << "Invalid share count - try again.\n";
                        continue;
                    }

                    if (shares % lotSize != 0) {
                        std::cout << "Odd lot - try again.\n";
                        continue;
                    }

                    // Passed all checks
                    double grossAmount = roundedPrice * shares;

                    // Seller fees
                    double commission = grossAmount * 0.0025;
                    double vat = commission * 0.12;
                    double transactionFee = grossAmount * 0.00005;
                    double sccpFee = grossAmount * 0.0001;
                    double tax = grossAmount * 0.001;

                    double totalDeductions = commission + vat + transactionFee + sccpFee + tax;
                    double netProceeds = grossAmount - totalDeductions;

                    std::cout << "\n--- Stock Selling Info ---\n";
                    std::cout << "Market Price  		: PHP " << roundedPrice << "\n";
                    std::cout << "Shares       	  	:     " << shares << "\n";
                    std::cout << std::fixed << std::setprecision(2);
                    std::cout << "Gross Amount 	 	: PHP " << grossAmount << "\n";
                    std::cout << "Commission    		: PHP " << commission << "\n";
                    std::cout << "VAT (12%)     		: PHP " << vat << "\n";
                    std::cout << "Transaction Fee		: PHP " << transactionFee << "\n";
                    std::cout << "SCCP Fee       		: PHP " << sccpFee << "\n";
                    std::cout << "Stock Tax (0.1%)	: PHP " << tax << "\n";
                    std::cout << "Fee Total		: PHP " << totalDeductions << "\n";
                    std::cout << "----------------------------\n";
                    std::cout << "Net Sale   		: PHP " << netProceeds << "\n";

                    matched = true;
                    break;
                }

                break; // only one lot rule match
            }
        }

        if (!matched) {
            std::cout << "No matching lot rule found. Please check the price range.\n";
        }
    }

    return 0;
}

