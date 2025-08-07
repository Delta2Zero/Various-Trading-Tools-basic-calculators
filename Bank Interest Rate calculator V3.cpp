#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> // for std::fixed and std::setprecision
#include <algorithm> // for std::transform


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

using namespace std;

int main() {
    resizeConsoleWindow(); // set up console size
    string input;
    double interestRate = 0.0;

    // Function to convert string to lowercase
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
        if (ssRate >> interestRate) {
            break;
        } else {
    		cout << "\n******************************************************\n";
            cout << "Invalid input. Please enter a valid interest rate.\n";
    		cout << "\n******************************************************\n";
        }
    }

    // Start main loop for balance input
    while (true) {
    	cout << "\n******************************************************\n";
    	cout << "\ntype exit to quit or interest to change interest rate\n";
    	cout << "\n******************************************************\n";
        cout << "Current Balance: ";
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
                if (ssRate >> interestRate) {
                    break;
                } else {
    				cout << "\n******************************************************\n";
                    cout << "Invalid input. Please enter a valid interest rate.\n";
    				cout << "\n******************************************************\n";
                }
            }
            continue; // Go back to asking for balance
        }

        // Parse balance
        double amount = 0.0;
        stringstream ssAmount(input);
        if (!(ssAmount >> amount)) {
    		cout << "\n******************************************************\n";
            cout << "Invalid input. Please enter a valid amount.\n";
    		cout << "\n******************************************************\n";
            continue;
        }

        // Convert percentage to decimal
        double rateDecimal = interestRate / 100.0;

        // Calculate initial interest gains
        double yearlyGain = rateDecimal * amount;
        double monthlyGain = yearlyGain / 12.0;
        double monthyFinalGain = monthlyGain - (monthlyGain * 0.2); 

        cout << fixed << setprecision(2);
		cout << "\n===========================================================\n";
		cout << "                INTEREST CALCULATOR RESULT                 \n";
		cout << "===========================================================\n";
		cout << left << setw(35) << "Bank Interest Rate:" << right << setw(13) << interestRate << "%\n";
		cout << left << setw(35) << "Initial Deposit Amount:" << "PHP " << right << setw(10) << amount << "\n";
		cout << left << setw(35) << "Yearly Interest Gain:" << "PHP " << right << setw(10) << yearlyGain << "\n";
		cout << left << setw(35) << "Monthly Interest Gain:" << "PHP " << right << setw(10) << monthlyGain << "\n";
		cout << left << setw(35) << "Monthly Interest Gain After Tax:" << "PHP " << right << setw(10) << monthyFinalGain << "\n";
		cout << "-----------------------------------------------------------\n";

		double currentBalance = amount;

		cout << "\nCOMPOUNDED INTEREST (January to December)\n";
		cout << "-----------------------------------------------------------\n";
		cout << left << setw(10) << "Month" << setw(20) << "Interest Gained" << "New Balance\n";
		cout << "-----------------------------------------------------------\n";

		for (int month = 1; month <= 12; ++month) {
    		double monthlyInterest = currentBalance * (rateDecimal / 12);
    		double monthlyInterestFinal = monthlyInterest - ( monthlyInterest * 0.2);
    		currentBalance += monthlyInterestFinal;
    		cout << left << setw(10) << month
        	 << "PHP " << setw(15) << monthlyInterest
        	 << "PHP " << currentBalance << "\n";
			}

		cout << "===========================================================\n";

    }

    cout << "Exiting program. Goodbye!\n";
    return 0;
}

