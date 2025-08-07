#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

/*
GCrypto Trading Calculator

this is a simple trading calculator that is used only for Gcrypto with some help from data in coinsph
the fee calculation is based on the padded buy price from what is shown on coinsph market price

includes break-even calculation and what the Gcrypto market sell price shown on ui is supposed to look like when selling...

Written by Chatgpt with prompt and testing by-DeltaTwoZero

this version is 1.2 since i have no idea how to name version 

for personal use only use at your own risk
*/

void resizeConsoleWindow() {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    if (console != NULL) {
        MoveWindow(console, 100, 100, 580, 500, TRUE);
    }
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        COORD newSize = {120, 500};
        SetConsoleScreenBufferSize(hOut, newSize);
    }
#endif
}

void clearScreen() {
    system("cls");
}

bool isExitCommand(const string& input) {
    string lowerInput = input;
    for (char& c : lowerInput) c = tolower(c);
    return lowerInput == "exit";
}

bool isClearCommand(const string& input) {
    string lowerInput = input;
    for (char& c : lowerInput) c = tolower(c);
    return lowerInput == "clear";
}

double getValidatedDouble(const string& prompt) {
    double value;
    string input;
    while (true) {
        cout << prompt;
        cin >> input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (isExitCommand(input)) exit(0);

        try {
            value = stod(input);
            return value;
        } catch (...) {
            cout << "Invalid input. Please enter a valid number." << endl;
        }
    }
}

int main() {
    resizeConsoleWindow();

    while (true) {
        string cmd;
        cout << "\n================== GCrypto Trading Calculator ==================" << endl;
        cout << "\nType 'clear' to clear screen, Type 'exit' to quit.\nPress Enter to continue...\n: ";
        getline(cin, cmd);
        if (isExitCommand(cmd)) break;
        if (isClearCommand(cmd)) clearScreen();

        // ---------- INPUTS ----------
        double actualMarketPrice = getValidatedDouble("Enter Actual Market Price (from Coins.ph): ");
        double gcryptoBuyPrice = getValidatedDouble("Enter Gcrypto Buy Price: ");
        double gcryptoSellPrice = getValidatedDouble("Enter Gcrypto Sell Price: ");
        double amountToSpend = getValidatedDouble("Enter Amount to Spend (PHP): ");
        double exitMarketPrice = getValidatedDouble("Enter Exit Market Price (from Coins.ph): ");
        double sellPercent = getValidatedDouble("Enter Crypto Sell Percentage (0 - 100): ");

        while (sellPercent < 0.0 || sellPercent > 100.0) {
            cout << "Sell percentage must be between 0 and 100." << endl;
            sellPercent = getValidatedDouble("Enter Crypto Sell Percentage (0 - 100): ");
        }

        // ---------- CALCULATIONS ----------
        double buyMarkupPercent = ((gcryptoBuyPrice - actualMarketPrice) / actualMarketPrice) * 100.0;
        double totalFee = buyMarkupPercent * 2.0;
        double cryptoReceived = amountToSpend / gcryptoBuyPrice;

        double gcryptoSpread = gcryptoBuyPrice - gcryptoSellPrice;
        double gcryptoSpreadPercent = (gcryptoSpread / gcryptoBuyPrice) * 100.0;
        double gcryptoSellMarkupPercent = ((actualMarketPrice - gcryptoSellPrice) / actualMarketPrice) * 100.0;

        double cryptoToSell = cryptoReceived * (sellPercent / 100.0);
        double capitalUsed = amountToSpend * (sellPercent / 100.0);

        // Sell Info 1: Based on Buy Mark-up
        double gcryptoSellPrice1 = exitMarketPrice - ((buyMarkupPercent / 100.0) * exitMarketPrice);
        double grossSale1 = cryptoToSell * gcryptoSellPrice1;
        double netProfit1 = grossSale1 - capitalUsed;

        // Sell Info 2: Based on Gcrypto Sell Mark-up
        double gcryptoSellPrice2 = exitMarketPrice - ((gcryptoSellMarkupPercent / 100.0) * exitMarketPrice);
        double grossSale2 = cryptoToSell * gcryptoSellPrice2;
        double netProfit2 = grossSale2 - capitalUsed;

        // ---------- OUTPUT ----------
        cout << fixed << setprecision(6);

        cout << "\n*************************************************************" << endl;
        cout << "***************** - Market Information - ********************" << endl;
        cout << "*************************************************************" << endl;
        cout << "Actual Market Price:               " << actualMarketPrice << endl;
        cout << "Market Price Buy (Gcrypto):        " << gcryptoBuyPrice << endl;
        cout << "Market Price Sell (Gcrypto):       " << gcryptoSellPrice << endl;
        cout << "Gcrypto Buy Mark-up %:             " << buyMarkupPercent << " %" << endl;
        cout << "Assumed Total Fee (Buy and Sell):  " << totalFee << " %" << endl;
        cout << "Crypto Coin to be received:        " << cryptoReceived << endl;

        cout << "\n*************************************************************" << endl;
        cout << "*************** - Additional Information - ******************" << endl;
        cout << "*************************************************************" << endl;
        cout << "Gcrypto Total Spread:             " << gcryptoSpread << endl;
        cout << "Gcrypto Spread %:                 " << gcryptoSpreadPercent << " %" << endl;
        cout << "Gcrypto Sell Mark-up %:           " << gcryptoSellMarkupPercent << " %" << endl;

        cout << "\n*************************************************************" << endl;
        cout << "***************** -   Sell Information - ********************" << endl;
        cout << "*************************************************************" << endl;
        cout << "Actual Market Price on Exit:       " << exitMarketPrice << endl;
        cout << "Market Price (Gcrypto on Exit):    " << gcryptoSellPrice1 << endl;
        cout << "Crypto Coin to be Sold in %:       " << sellPercent << " %" << endl;
        cout << "Net Profit Gained:                 " << netProfit1 << " PHP" << endl;

        cout << "\n*************************************************************" << endl;
        cout << "**-Sell Information Based on Mark-up on Coinsph vs Gcrypto-**" << endl;
        cout << "*************************************************************" << endl;
        cout << "Actual Market Price on Exit:       " << exitMarketPrice << endl;
        cout << "Market Price (Gcrypto on Exit):    " << gcryptoSellPrice2 << endl;
        cout << "Crypto Coin to be Sold in %:       " << sellPercent << " %" << endl;
        cout << "Net Profit Gained:                 " << netProfit2 << " PHP" << endl;
        cout << "*************************************************************" << endl;
        cout << "*************************************************************" << endl;
    }

    return 0;
}

