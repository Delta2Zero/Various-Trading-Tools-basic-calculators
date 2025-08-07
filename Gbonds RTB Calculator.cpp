#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

#ifdef _WIN32
#include <cstdlib>
#define CLEAR_COMMAND "cls"
#else
#include <cstdlib>
#define CLEAR_COMMAND "clear"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

void resizeConsoleWindow() {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    if (console != NULL) {
        MoveWindow(console, 100, 100, 500, 500, TRUE);
    }

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hOut, &csbi);
        COORD newSize = {120, 500};
        SetConsoleScreenBufferSize(hOut, newSize);
    }
#endif
}

using namespace std;

void clearConsole() {
    system(CLEAR_COMMAND);
}

bool isNumber(const string& input) {
    try {
        stod(input);
        return true;
    } catch (...) {
        return false;
    }
}

int main() {
    resizeConsoleWindow();
    while (true) {
        string input;
        double capital = 0.0;
        double annualRate = 0.0;
        int durationDays = 0;

        // Capital
        cout << "Enter Capital Amount: ";
        getline(cin, input);
        if (input == "clear") {
            clearConsole();
            continue;
        }
        if (!isNumber(input) || stod(input) <= 0) {
            cout << "Invalid input. Restarting...\n\n";
            continue;
        }
        capital = stod(input);

        // Interest Rate
        cout << "Enter Annual Interest Rate (e.g. 6.25): ";
        getline(cin, input);
        if (input == "clear") {
            clearConsole();
            continue;
        }
        if (!isNumber(input) || stod(input) <= 0) {
            cout << "Invalid input. Restarting...\n\n";
            continue;
        }
        annualRate = stod(input);

        // Duration (Days)
        cout << "Enter Duration in Days: ";
        getline(cin, input);
        if (input == "clear") {
            clearConsole();
            continue;
        }
        if (!isNumber(input) || stoi(input) <= 0) {
            cout << "Invalid input. Restarting...\n\n";
            continue;
        }
        durationDays = stoi(input);

        // --- Computation ---
        double convenienceFee = capital * 0.002;     // 0.2%
        double transactionFee = 20.0;                // Fixed ?20
        double totalFees = convenienceFee + transactionFee;

        int numQuarters = durationDays / 91;         // Approximate 1 quarter as 91 days
        double quarterlyRate = annualRate / 100.0 * (3.0 / 12.0);
        double grossQuarterlyInterest = capital * quarterlyRate;
        double netQuarterlyInterest = grossQuarterlyInterest * 0.80;

        double totalNetInterest = netQuarterlyInterest * numQuarters;
        double netGainAfterFees = totalNetInterest - totalFees;
        double finalMaturityPayout = capital + netQuarterlyInterest;
        double finalAmount = capital + netGainAfterFees;

        // --- Output ---
        cout << fixed << setprecision(2);
        cout << "\n===== RTB Bond Planning Summary =====\n";
        cout << "Capital Amount:           PHP " << capital << endl;
        cout << "Annual Rate:                  " << annualRate << "%\n";
        cout << "Bond Duration:                " << durationDays << " days (" << numQuarters << " quarters)\n";
        cout << "-----------------------------\n";
        cout << "Net Quarterly Interest:   PHP " << netQuarterlyInterest << endl;
        cout << "Number of Payouts:            " << numQuarters << endl;
        cout << "Total Net Interest:       PHP " << totalNetInterest << endl;
        cout << "Final Payout (last + cap):PHP " << finalMaturityPayout << endl;
        cout << "-----------------------------\n";
        cout << "Convenience Fee (0.2%):   PHP " << convenienceFee << endl;
        cout << "Transaction Fee (fixed):  PHP " << transactionFee << endl;
        cout << "Total Fees:               PHP " << totalFees << endl;
        cout << "Net Gain After Fees:      PHP " << netGainAfterFees << endl;
        cout << "Total Received at End:    PHP " << finalAmount << endl;
        cout << "===============================\n\n";

        cout << "Press Enter to calculate again or type 'exit' to quit.\n";
        getline(cin, input);
        if (input == "exit") break;
        clearConsole();
    }

    return 0;
}

