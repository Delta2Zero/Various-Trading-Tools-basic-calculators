#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <stdexcept>   // for invalid_argument, out_of_range

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

using namespace std;

// Fee constants
const double PSE_FEE = 0.00005;
const double SCCP_FEE = 0.0001;
const double BROKER_COMMISSION_RATE = 0.0025;
const double VAT_RATE = 0.12;
const double SALES_TAX = 0.001;

void print_line() {
    cout << "\n***************************************************************************\n" << endl;
}

int main() {
    resizeConsoleWindow(); // set up console size
    
    cout << fixed << setprecision(2);
    string input;
    double capital;

    cout << R"(

***************************************************************************
***************************************************************************

 Written by Chatgpt, Prompt and code checking and testing by DeltaTwoZero

 Gstocks PSE Trading Calculator Version 5

 Enter your total capital or type 'exit' to quit

***************************************************************************
***************************************************************************
)" ;

    while (true) {
        cout << "Capital: ";
        cin >> input;
        if (input == "exit") break;

        try {
            capital = stod(input);
        } catch (invalid_argument& e) {
            cout << "Invalid capital input. Please enter a valid number or 'exit' to quit.\n";
            continue;
        } catch (out_of_range& e) {
            cout << "Capital input out of range. Please enter a valid number.\n";
            continue;
        }

        double market_price;
        int stocks_bought;
        double custom_exit_price;
        int stocks_to_sell;

        try {
            print_line();
            cout << "Enter the market price: ";
            cin >> market_price;
            if(cin.fail()) throw invalid_argument("Invalid market price");

            cout << "Enter the amount of stocks you bought: ";
            cin >> stocks_bought;
            if(cin.fail()) throw invalid_argument("Invalid stocks bought");

            cout << "Enter your planned custom exit price: ";
            cin >> custom_exit_price;
            if(cin.fail()) throw invalid_argument("Invalid custom exit price");

            cout << "Enter how many stocks you plan to sell (can be less than bought): ";
            cin >> stocks_to_sell;
            if(cin.fail()) throw invalid_argument("Invalid stocks to sell");
        }
        catch (invalid_argument& e) {
            cout << "Input error detected: " << e.what() << ". Returning to capital input.\n";
            // Clear error flags and discard invalid input from buffer
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;  // Go back to capital input
        }

        // Clear input buffer before continuing just in case
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // --- Buy Info ---
        double gross_buy = market_price * stocks_bought;
        double buy_pse_fee = gross_buy * PSE_FEE;
        double buy_sccp_fee = gross_buy * SCCP_FEE;
        double buy_commission = gross_buy * BROKER_COMMISSION_RATE;
        double buy_vat = buy_commission * VAT_RATE;
        double buyFeeTotal = buy_pse_fee + buy_sccp_fee + buy_commission + buy_vat;
        double total_buy_cost = gross_buy + buy_pse_fee + buy_sccp_fee + buy_commission + buy_vat;

        print_line();
        
        cout << "--- Buy Information ---" << endl;
        cout << "Market Price: PHP " << market_price << endl;
        cout << "Stocks Bought: " << stocks_bought << endl;
        cout << "Gross Buy: PHP " << gross_buy << endl;
        cout << "---------- Fees ------------" << endl;
        cout << "PSE Fee: PHP " << buy_pse_fee << endl;
        cout << "SCCP Fee: PHP " << buy_sccp_fee << endl;
        cout << "Broker's Commission: PHP " << buy_commission << endl;
        cout << "VAT on Commission: PHP " << buy_vat << endl;
        cout << "Buy Fee Total: PHP " << buyFeeTotal << endl;
        cout << "---------------------------" << endl;
        cout << "Total Buy Cost: PHP " << total_buy_cost << endl;
        cout << "---------------------------" << endl;

        print_line();

        // --- Brute Force Break Even ---
        double break_even_price = market_price;
        double net_sell, gross_sell, sell_sales_tax, sell_pse, sell_sccp, sell_comm, sell_vat;
        int loop_steps = 0;
        double adjusted_net_sell;  // Declare adjusted net sell

        while (true) {
            // Calculate gross sell and fees
            gross_sell = break_even_price * stocks_to_sell;
            sell_sales_tax = gross_sell * SALES_TAX;
            sell_pse = gross_sell * PSE_FEE;
            sell_sccp = gross_sell * SCCP_FEE;
            sell_comm = gross_sell * BROKER_COMMISSION_RATE;
            sell_vat = sell_comm * VAT_RATE;

            // Calculate net sell after all fees
            net_sell = gross_sell - (sell_sales_tax + sell_pse + sell_sccp + sell_comm + sell_vat);

            // Calculate adjusted net sell by subtracting the total buy cost from net sell
            adjusted_net_sell = net_sell - total_buy_cost;

            // If the adjusted net sell is greater than or equal to the proportional buy cost, break the loop
            if (adjusted_net_sell >= 0) break;
            
           //debug cout << "Net Sell: PHP " << adjusted_net_sell << " vs Profit: PHP 0" << endl;

            // Increment the break-even price for the next loop iteration
            break_even_price += 0.01;
            loop_steps++;

            // Prevent infinite loop by breaking after 10,000 iterations
            if (loop_steps > 10000) break;
        }

        cout << "--- Brute-Force Break Even ---" << endl;
        cout << "Break Even Sell Price: PHP " << break_even_price << endl;
        cout << "Gross Sell: PHP " << gross_sell << endl;
        cout << "---------- Fees ------------" << endl;
        cout << "Sales Tax: PHP " << sell_sales_tax << endl;
        cout << "PSE Fee: PHP " << sell_pse << endl;
        cout << "SCCP Fee: PHP " << sell_sccp << endl;
        cout << "Broker's Commission: PHP " << sell_comm << endl;
        cout << "VAT on Commission: PHP " << sell_vat << endl;
        cout << "---------------------------" << endl;
        cout << "Net Sale: PHP " << adjusted_net_sell << endl;
        cout << "Total Loop Steps Taken: " << loop_steps << endl;
        cout << "---------------------------" << endl;

        print_line();

        // --- Custom Exit ---
        double custom_gross_sell = custom_exit_price * stocks_to_sell;
        double custom_sales_tax = custom_gross_sell * SALES_TAX;
        double custom_sell_pse = custom_gross_sell * PSE_FEE;
        double custom_sell_sccp = custom_gross_sell * SCCP_FEE;
        double custom_comm = custom_gross_sell * BROKER_COMMISSION_RATE;
        double custom_vat = custom_comm * VAT_RATE;
        double custom_net_sell = custom_gross_sell - (custom_sales_tax + custom_sell_pse + custom_sell_sccp + custom_comm + custom_vat);
        double customFeeSellTotal = custom_sales_tax + custom_sell_pse + custom_sell_sccp + custom_comm + custom_vat;

        double proportional_buy_cost = (total_buy_cost / stocks_bought) * stocks_to_sell;
        double net_profit = custom_net_sell - proportional_buy_cost;
        double gain_percent = (net_profit / proportional_buy_cost) * 100;
        double market_move_percent = ((custom_exit_price - market_price) / market_price) * 100;
        double updated_capital = capital + net_profit;

        cout << "--- Custom Exit Simulation ---" << endl;
        cout << "Custom Exit Price: PHP " << custom_exit_price << endl;
        cout << "Stocks to Sell: " << stocks_to_sell << endl;
        cout << "Gross Sale: PHP " << custom_gross_sell << endl;
        cout << "---------- Fees ------------" << endl;
        cout << "Sales Tax: PHP " << custom_sales_tax << endl;
        cout << "PSE Fee: PHP " << custom_sell_pse << endl;
        cout << "SCCP Fee: PHP " << custom_sell_sccp << endl;
        cout << "Broker's Commission: PHP " << custom_comm << endl;
        cout << "VAT on Commission: PHP " << custom_vat << endl;
        cout << "Sell Fee Total: PHP " << customFeeSellTotal << endl;
        cout << "---------------------------" << endl;
        cout << "Net Sale: PHP " << custom_net_sell << endl;
        cout << "Proportional Buy Cost: PHP " << proportional_buy_cost << endl;
        cout << "Net Profit: PHP " << net_profit << endl;
        cout << "---------------------------" << endl;
        cout << "Custom Exit is a " << gain_percent << "% gain from initial capital buy amount." << endl;
        cout << "Custom Exit is a " << market_move_percent << "% Market movement." << endl;
        cout << "---------------------------" << endl;
        cout << "Updated Total Capital: PHP " << updated_capital << endl;
        cout << "---------------------------" << endl;

        print_line();
    }

    return 0;
}

