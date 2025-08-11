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
        MoveWindow(console, 100, 100, 680, 500, TRUE);
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

const double CONVENIENCE_FEE_RATE_TBILL = 0.005;  
const double TRANSACTION_FEE_RATE = 0.0025;       
const double TRANSACTION_FEE_CAP = 5.0;           
const double TAX_RATE = 0.20;                     

double computeGrossInterest(double capital, double rate, int maturityDays) {
    return capital * (rate / 100.0) * (maturityDays / 365.0);
}

void computeFees(double capital, int maturityDays, double& convenienceFee, double& transactionFee) {
    convenienceFee = capital * CONVENIENCE_FEE_RATE_TBILL * (maturityDays / 365.0);
    transactionFee = capital * TRANSACTION_FEE_RATE * (maturityDays / 365.0);
    if (transactionFee > TRANSACTION_FEE_CAP)
        transactionFee = TRANSACTION_FEE_CAP;
}

void calculateTBills() {
    double capital, rate;
    int maturityDays;

    cout << "==== T-Bill Calculator ====" << endl;

    // Input validation for capital
    cout << "Capital Amount (PHP): ";
    if (!(cin >> capital)) {
        cout << "Invalid input. Please enter a numeric value for capital: ";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    // Input validation for rate
    cout << "Interest (% per annum): ";
    if (!(cin >> rate)) {
        cout << "Invalid input. Please enter a numeric value for interest rate: ";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    // Input validation for maturity days
    cout << "Maturity Days: ";
    if (!(cin >> maturityDays)) {
        cout << "Invalid input. Please enter a numeric value for maturity days: ";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    double grossInterest = computeGrossInterest(capital, rate, maturityDays);

    double convenienceFee = 0.0, transactionFee = 0.0;
    computeFees(capital, maturityDays, convenienceFee, transactionFee);

    double totalFees = convenienceFee + transactionFee;
    double totalSpent = capital + totalFees;
    double tax = grossInterest * TAX_RATE;
    double netGain = grossInterest - tax - totalFees;
    double total = capital + netGain;

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
    resizeConsoleWindow(); 
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
