#include <iostream>
#include <iomanip>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

void resizeConsoleWindow() {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    if (console != NULL) {
        // MoveWindow(hwnd, x, y, width_px, height_px, repaint)
        MoveWindow(console, 100, 100, 680, 500, TRUE); // Width and height in pixels
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

using namespace std;

// Constants
const double CONVENIENCE_FEE_RATE_TBILL = 0.005;  // 0.50% p.a.
const double TRANSACTION_FEE_RATE = 0.0025;       // 0.25%
const double TRANSACTION_FEE_CAP = 5.0;           // PHP 5 max
const double TAX_RATE = 0.20;                     // 20%

// Function to compute gross interest
double computeGrossInterest(double capital, double rate, int maturityDays) {
    return capital * (rate / 100.0) * (maturityDays / 365.0);
}

// Function to compute fees
void computeFees(double capital, int maturityDays, double& convenienceFee, double& transactionFee) {
    // T-Bill uses time-based convenience fee
    convenienceFee = capital * CONVENIENCE_FEE_RATE_TBILL * (maturityDays / 365.0);

    // Transaction fee has a cap
    transactionFee = capital * TRANSACTION_FEE_RATE * (maturityDays / 365.0);
    if (transactionFee > TRANSACTION_FEE_CAP)
        transactionFee = TRANSACTION_FEE_CAP;
}

// Main calculation logic
void calculateTBills() {
    double capital, rate;
    int maturityDays;

    cout << "==== T-Bill Calculator ====" << endl;

    // Input
    cout << "Capital Amount (PHP): ";
    cin >> capital;

    cout << "Interest (% per annum): ";
    cin >> rate;

    cout << "Maturity Days: ";
    cin >> maturityDays;

    // Calculation
    double grossInterest = computeGrossInterest(capital, rate, maturityDays);

    double convenienceFee = 0.0, transactionFee = 0.0;
    computeFees(capital, maturityDays, convenienceFee, transactionFee);

    double totalFees = convenienceFee + transactionFee;
    double totalSpent = capital + totalFees;
    double tax = grossInterest * TAX_RATE;
    double netGain = grossInterest - tax - totalFees;
    double total = capital + netGain;

    // Output
    cout << fixed << setprecision(2);
    cout << "===================  Results  =========================" << endl;
    cout << "Capital Amount: PHP " << capital << endl;
    cout << "Convenience Fee: " << convenienceFee << endl;
    cout << "Transaction Fee: " << transactionFee << endl;
    cout << "======================================================" << endl;
    cout << "Total Fees: PHP " << totalFees << endl;
    cout << "Total Amount Spent: PHP " << totalSpent << endl;
    cout << "======================================================" << endl;
    cout << "Gross Gain on Maturity: PHP " << grossInterest << endl;
    cout << "20% Tax: PHP " << tax << endl;
    cout << "Net Gain: PHP " << netGain << endl;
    cout << "======================================================" << endl;
    cout << "Total (Capital + Net Gain): PHP " << total << endl;
    cout << "======================================================" << endl;
}

int main() {
    resizeConsoleWindow(); // set up console size
    while (true) {
        calculateTBills();

        string again;
        cout << "\nType 'clear' to clear screen, 'exit' to quit, or press Enter to calculate again.\n> ";
        cin.ignore();
        getline(cin, again);

        if (again == "exit") break;
        if (again == "clear") {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        }
    }

    return 0;
}

