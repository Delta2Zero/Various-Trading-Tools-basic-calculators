#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>
#include <cmath>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

void resizeConsoleWindow() {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    if (console != NULL) {
        MoveWindow(console, 100, 100, 650, 500, TRUE);
    }
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        COORD newSize = {120, 500};
        SetConsoleScreenBufferSize(hOut, newSize);
    }
#endif
}

double calculateFee(double amount) {
    double fee = amount * 0.0025;
    return (fee < 0.1) ? 0.1 : fee;
}

int main() {
    resizeConsoleWindow();
    std::string input;

    while (true) {
        std::cout << "***************************************************************************\n";
        std::cout << "\n Written by Chatgpt, Prompt and code checking and testing by DeltaTwoZero \n";
        std::cout << "\n GoTrade Stocks Trading Calculator Version 3.5 (USD Mode + Piecemeal Check) \n";
        std::cout << "\n Enter your choice or type 'exit' to quit \n";
        std::cout << "***************************************************************************\n";
        std::cout << "1 - Enter by USD spent (auto-calculate stock)\n";
        std::cout << "2 - Enter by Stock amount (manual input like Gotrade UI)\n";
        std::cout << "Choice: ";

        std::getline(std::cin, input);
        if (input == "exit") break;

        int mode = 0;
        std::stringstream(input) >> mode;
        if (mode != 1 && mode != 2) {
            std::cout << "Invalid choice. Try again.\n";
            continue;
        }

        double marketPrice = 0.0, stockAmount = 0.0, rawBuyCost = 0.0;
        double buyFee = 0.0, totalBuyCost = 0.0, totalCapital = 0.0;

        std::cout << "Enter your total capital (USD): ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        std::stringstream(input) >> totalCapital;

        std::cout << "Enter market price of stock: ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        std::stringstream(input) >> marketPrice;
        if (marketPrice <= 0) {
            std::cout << "Invalid market price.\n";
            continue;
        }

        if (mode == 1) {
            double usdAmount = 0.0;
            std::cout << "Enter USD amount to spend (gross): ";
            std::getline(std::cin, input);
            if (input == "exit") break;
            std::stringstream(input) >> usdAmount;
            if (usdAmount <= 0) {
                std::cout << "Invalid USD amount.\n";
                continue;
            }
            buyFee = calculateFee(usdAmount);
            if (usdAmount <= buyFee) {
                std::cout << "Fee is equal or greater than amount. Cannot proceed.\n";
                continue;
            }
            rawBuyCost = usdAmount - buyFee;
            stockAmount = rawBuyCost / marketPrice;
            totalBuyCost = usdAmount;

        } else {
            std::cout << "Enter stock amount (as shown in Gotrade): ";
            std::getline(std::cin, input);
            if (input == "exit") break;
            std::stringstream(input) >> stockAmount;
            if (stockAmount <= 0) {
                std::cout << "Invalid stock amount.\n";
                continue;
            }
            rawBuyCost = stockAmount * marketPrice;
            buyFee = calculateFee(rawBuyCost);
            totalBuyCost = rawBuyCost + buyFee;
        }

        std::cout << std::fixed << std::setprecision(12);
        std::cout << "\n--------------------------= Buy Information =------------------------------\n";
        std::cout << "Market Price:                           USD " << marketPrice << "\n";
        std::cout << "Stock/s Amount Bought:                  Shares " << stockAmount << "\n";
        std::cout << "Raw Buy Cost (Capital):                 USD " << rawBuyCost << "\n";
        std::cout << "Buy Fee (0.25% or $0.1 min):            USD " << buyFee << "\n";
        std::cout << "Total Buy Cost:                         USD " << totalBuyCost << "\n";
        std::cout << "***************************************************************************\n";

        std::cout << "Enter your planned custom exit price: ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        double customExitPrice;
        std::stringstream(input) >> customExitPrice;

        std::cout << "Enter how many stock/s you plan to sell: ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        double sellStockAmount;
        std::stringstream(input) >> sellStockAmount;

        double testSellGross = totalBuyCost;
        double sellFee, netAfterSell;
        double step = 0.000001;
        int loops = 0;
        while (true) {
            sellFee = calculateFee(testSellGross);
            netAfterSell = testSellGross - sellFee;
            if (netAfterSell >= totalBuyCost) {
                testSellGross = std::ceil(testSellGross * 1000000000000.0) / 1000000000000.0;
                break;
            }
            testSellGross += step;
            loops++;
            if (loops > 10000000) break;
        }
        double breakEvenPrice = testSellGross / stockAmount;
        double breakEvenNetProfit = testSellGross - totalBuyCost - sellFee;

        std::cout << "\n----------------------= Brute-Force Break Even =---------------------------\n";
        std::cout << "Brute Force Break Even Sell Price:              USD " << breakEvenPrice << "\n";
        std::cout << "Gross Sell at Break Even:                       USD " << testSellGross << "\n";
        std::cout << "Sell Fee (0.25% or $0.1 min):                   USD " << sellFee << "\n";
        std::cout << "Buy Cost vs Net Profit:                         USD " << breakEvenNetProfit << "\n";
        std::cout << "Total Loop Steps Taken:                         " << loops << "\n";

        double customGrossSale = customExitPrice * sellStockAmount;
        double customSellFee = calculateFee(customGrossSale);
        double proportionalBuyCost = (sellStockAmount <= stockAmount) ? (totalBuyCost / stockAmount) * sellStockAmount : totalBuyCost;
        double customNetSale = customGrossSale - customSellFee;
        double customNetProfit = customNetSale - proportionalBuyCost;

        std::cout << "\n----------------------= Custom Exit Simulation =---------------------------\n";
        std::cout << "Custom Exit Price:                             USD " << customExitPrice << "\n";
        std::cout << "Stock/s Amount to Sell:                        Shares " << sellStockAmount << "\n";
        std::cout << "Gross Sale:                                    USD " << customGrossSale << "\n";
        std::cout << "Sell Fee (0.25% or $0.1 min):                  USD " << customSellFee << "\n";
        std::cout << "Proportional Buy Cost:                         USD " << proportionalBuyCost << "\n";
        std::cout << "Net Sale (After Sell Fee):                     USD " << customNetSale << "\n";
        std::cout << "Net Profit (After Total Buy Cost + Sell Fees): USD " << customNetProfit << "\n";

        double percentGain = (customNetProfit / rawBuyCost) * 100.0;
        double percentMovement = ((customExitPrice - marketPrice) / marketPrice) * 100.0;

        std::cout << "\n-------------------------= Market Movement =-------------------------------\n";
        std::cout << "Custom Exit is a " << percentGain << "% gain from initial capital buy amount.\n";
        std::cout << "Custom Exit is a " << percentMovement << "% Market movement\n";

        double updatedTotalCapital = totalCapital + customNetProfit;
        std::cout << "--------------------------= Capital Update =-------------------------------\n";
        std::cout << "Total Capital:                                 USD " << totalCapital << "\n";
        std::cout << "Updated Total Capital:                         USD " << updatedTotalCapital << "\n";
        std::cout << "***************************************************************************\n";

        std::cout << "\n------------------= Piecemeal Sell Fee Trap Simulation =--------------------\n";
        if (sellStockAmount < 1.0) {
            std::cout << "Selling less than 1 share - no piecemeal sell fee trap applies.\n";
        } else {
            int shares_to_sell = static_cast<int>(std::floor(sellStockAmount));
            double remaining_fraction = sellStockAmount - shares_to_sell;

            double piecemeal_gross = 0.0;
            double piecemeal_fees = 0.0;
            double piecemeal_net = 0.0;

            for (int i = 0; i < shares_to_sell; ++i) {
                double gross = customExitPrice * 1.0;
                double fee = calculateFee(gross);
                double net = gross - fee;

                piecemeal_gross += gross;
                piecemeal_fees += fee;
                piecemeal_net += net;
            }

            if (remaining_fraction > 0.0) {
                double gross = customExitPrice * remaining_fraction;
                double fee = calculateFee(gross);
                double net = gross - fee;

                piecemeal_gross += gross;
                piecemeal_fees += fee;
                piecemeal_net += net;
            }

            double piecemealNetProfit = piecemeal_net - proportionalBuyCost;

            std::cout << std::fixed << std::setprecision(12);
            std::cout << "Piecemeal Gross Sale:                         USD " << piecemeal_gross << "\n";
            std::cout << "Piecemeal Total Fees:                         USD " << piecemeal_fees << "\n";
            std::cout << "Piecemeal Net Sale (After All Fees):          USD " << piecemeal_net << "\n";
            std::cout << "Piecemeal Net Profit:                         USD " << piecemealNetProfit << "\n";

            if (piecemeal_net < customNetSale) {
                std::cout << "\nWARNING: Piecemeal selling resulted in LOWER net sale.\n";
                std::cout << "You lose USD " << (customNetSale - piecemeal_net) << " compared to bulk sell.\n";
                std::cout << "This is likely due to minimum $0.10 fee per tiny sale.\n";
                std::cout << "Avoid low-quantity piecemeal selling near breakeven.\n";
            }
        }
    }
    return 0;
}
