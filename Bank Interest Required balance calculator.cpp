#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

void resizeConsoleWindow() {
#ifdef _WIN32
    HWND console = GetConsoleWindow();
    if (console != NULL) {
        MoveWindow(console, 100, 100, 580, 500, TRUE);
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

int main() {
    resizeConsoleWindow();
    string input;
    double interestRate = 0.0;

    auto toLower = [](string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    };

    // Ask for initial interest rate
    while (true) {
        cout << "\n******************************************************\n";
        cout << "\n type exit to exit the program \n";
        cout << "\n Written by: ChatGPT \n";
        cout << "\n Prompt code checking and testing by : DeltaTwoZero \n";
        cout << "\n******************************************************\n";
        cout << "Enter bank interest rate: ";
        getline(cin, input);
        if (toLower(input) == "exit") {
            cout << "\n******************************************************\n";
            cout << "Exiting program. Goodbye!\n";
            cout << "\n******************************************************\n";
            return 0;
        }

        stringstream ssRate(input);
        if (ssRate >> interestRate && interestRate > 0) {
            break;
        } else {
            cout << "\n******************************************************\n";
            cout << "Invalid input. Please enter a valid interest rate.\n";
            cout << "\n******************************************************\n";
        }
    }

    // Start main loop for desired monthly income input
    while (true) {
        cout << "\n******************************************************\n";
        cout << "\ntype exit to quit or interest to change interest rate\n";
        cout << "\n******************************************************\n";
        cout << "Desired Monthly Interest Income: ";
        getline(cin, input);
        string loweredInput = toLower(input);

        if (loweredInput == "exit") {
            break;
        } else if (loweredInput == "interest") {
            // Ask for new interest rate
            while (true) {
                cout << "\n******************************************************\n";
                cout << "\n type exit to exit the program \n";
                cout << "\n Written by: ChatGPT \n";
                cout << "\n Prompt code checking and testing by : DeltaTwoZero \n";
                cout << "\n******************************************************\n";
                cout << "Enter new bank interest rate: ";
                getline(cin, input);
                if (toLower(input) == "exit") {
                    cout << "\n******************************************************\n";
                    cout << "Exiting program. Goodbye!\n";
                    cout << "\n******************************************************\n";
                    return 0;
                }
                stringstream ssRate(input);
                if (ssRate >> interestRate && interestRate > 0) {
                    break;
                } else {
                    cout << "\n******************************************************\n";
                    cout << "Invalid input. Please enter a valid interest rate.\n";
                    cout << "\n******************************************************\n";
                }
            }
            continue;
        }

        double desiredMonthlyIncome = 0.0;
        stringstream ssAmount(input);
        if (!(ssAmount >> desiredMonthlyIncome) || desiredMonthlyIncome <= 0) {
            cout << "\n******************************************************\n";
            cout << "Invalid input. Please enter a valid monthly income.\n";
            cout << "\n******************************************************\n";
            continue;
        }

        double rateDecimal = interestRate / 100.0;
        double requiredBalance = (desiredMonthlyIncome * 12) / rateDecimal;
        double yearlyGain = desiredMonthlyIncome * 12;

        cout << fixed << setprecision(2);
        cout << "\n===========================================================\n";
        cout << "                REQUIRED BALANCE CALCULATOR                \n";
        cout << "===========================================================\n";
        cout << left << setw(35) << "Bank Interest Rate:" << right << setw(13) << interestRate << "%\n";
        cout << left << setw(35) << "Desired Monthly Income:" << "PHP " << right << setw(10) << desiredMonthlyIncome << "\n";
        cout << left << setw(35) << "Required Deposit Amount:" << "PHP " << right << setw(10) << requiredBalance << "\n";
        cout << left << setw(35) << "Yearly Interest Gain:" << "PHP " << right << setw(10) << yearlyGain << "\n";
        cout << "-----------------------------------------------------------\n";

        double currentBalance = requiredBalance;

        cout << "\nCOMPOUNDED INTEREST (January to December)\n";
        cout << "-----------------------------------------------------------\n";
        cout << left << setw(10) << "Month" << setw(20) << "Interest Gained" << "New Balance\n";
        cout << "-----------------------------------------------------------\n";

        for (int month = 1; month <= 12; ++month) {
            double monthlyInterest = currentBalance * (rateDecimal / 12);
            currentBalance += monthlyInterest;
            cout << left << setw(10) << month
                 << "PHP " << setw(15) << monthlyInterest
                 << "PHP " << currentBalance << "\n";
        }

        cout << "===========================================================\n";
    }

    cout << "Exiting program. Goodbye!\n";
    return 0;
}

