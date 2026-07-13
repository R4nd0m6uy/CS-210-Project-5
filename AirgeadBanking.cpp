#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <limits>
#include <cctype>
#include <cmath>  // For std::round
#include <locale> // For formatted output with commas

using namespace std;

// Helper to round to 2 decimal places for display
double roundTo2(double value) {
    return std::round(value * 100.0) / 100.0;
}

// Format a double as currency with commas and 2 decimals (e.g., 1,234,567.89)
string formatCurrency(double value) {
    stringstream ss;
    ss.imbue(locale(""));
    ss << fixed << setprecision(2) << value;
    return ss.str();
}

// Struct to hold year-end summary data
struct YearEndReport {
    int year;
    double yearEndBalance;
    double interestEarned;
};

// InvestmentCalculator handles all logic for the investment reports
class InvestmentCalculator {
public:
    InvestmentCalculator();

    // Setters with validation
    bool setInitialInvestment(double t_amount);
    bool setMonthlyDeposit(double t_amount);
    bool setAnnualInterest(double t_rate);
    bool setNumYears(int t_years);

    void clearData();

    // Main report generators
    vector<YearEndReport> calculateWithoutMonthlyDeposits() const;
    vector<YearEndReport> calculateWithMonthlyDeposits() const;

    // Display functions
    void printReport(const vector<YearEndReport>& report, bool withDeposits) const;
    void printInputs() const;

private:
    double m_initialInvestment;
    double m_monthlyDeposit;
    double m_annualInterest;
    int m_numYears;
};

// Constructor initializes member variables
InvestmentCalculator::InvestmentCalculator()
    : m_initialInvestment(0), m_monthlyDeposit(0), m_annualInterest(0), m_numYears(0) {}

// Validate and set the initial investment
bool InvestmentCalculator::setInitialInvestment(double t_amount) {
    if (t_amount < 0) return false;
    m_initialInvestment = t_amount;
    return true;
}

// Validate and set the monthly deposit
bool InvestmentCalculator::setMonthlyDeposit(double t_amount) {
    if (t_amount < 0) return false;
    m_monthlyDeposit = t_amount;
    return true;
}

// Validate and set the annual interest rate
bool InvestmentCalculator::setAnnualInterest(double t_rate) {
    if (t_rate < 0) return false;
    m_annualInterest = t_rate;
    return true;
}

// Validate and set the number of years
bool InvestmentCalculator::setNumYears(int t_years) {
    if (t_years <= 0) return false;
    m_numYears = t_years;
    return true;
}

// Reset all member variables to zero
void InvestmentCalculator::clearData() {
    m_initialInvestment = 0;
    m_monthlyDeposit = 0;
    m_annualInterest = 0;
    m_numYears = 0;
}

// Generate a report with no monthly deposits
vector<YearEndReport> InvestmentCalculator::calculateWithoutMonthlyDeposits() const {
    vector<YearEndReport> report;
    double balance = m_initialInvestment;
    double interestRate = m_annualInterest / 100.0;

    for (int year = 1; year <= m_numYears; ++year) {
        double yearInterest = 0.0;
        for (int month = 1; month <= 12; ++month) {
            double interest = balance * (interestRate / 12.0);
            yearInterest += interest;
            balance += interest;
        }
        report.push_back({year, balance, yearInterest});
    }
    return report;
}

// Generate a report including monthly deposits
vector<YearEndReport> InvestmentCalculator::calculateWithMonthlyDeposits() const {
    vector<YearEndReport> report;
    double balance = m_initialInvestment;
    double interestRate = m_annualInterest / 100.0;

    for (int year = 1; year <= m_numYears; ++year) {
        double yearInterest = 0.0;
        for (int month = 1; month <= 12; ++month) {
            balance += m_monthlyDeposit;
            double interest = balance * (interestRate / 12.0);
            yearInterest += interest;
            balance += interest;
        }
        report.push_back({year, balance, yearInterest});
    }
    return report;
}

// Nicely print a yearly investment report (rounded to 2 decimals for display, formatted with commas)
void InvestmentCalculator::printReport(const vector<YearEndReport>& report, bool withDeposits) const {
    cout << "\n" << (withDeposits ? "   Balance and Interest With Additional Monthly Deposits"
                                   : "   Balance and Interest Without Additional Monthly Deposits") << endl;
    cout << "==============================================================" << endl;
    cout << "  Year\tYear End Balance\tYear End Earned Interest" << endl;
    cout << "--------------------------------------------------------------" << endl;
    for (const auto& entry : report) {
        cout << "  " << entry.year
             << "\t$" << formatCurrency(roundTo2(entry.yearEndBalance))
             << "\t\t$" << formatCurrency(roundTo2(entry.interestEarned)) << endl;
    }
    cout << "==============================================================" << endl;
}

// Print the user's entered investment details (rounded for display, formatted with commas)
void InvestmentCalculator::printInputs() const {
    cout << "\nYour Data:" << endl;
    cout << "Initial Investment Amount: $" << formatCurrency(roundTo2(m_initialInvestment)) << endl;
    cout << "Monthly Deposit:           $" << formatCurrency(roundTo2(m_monthlyDeposit)) << endl;
    cout << "Annual Interest:           " << fixed << setprecision(2) << roundTo2(m_annualInterest) << "%" << endl;
    cout << "Number of Years:           " << m_numYears << endl;
}

// ----------- Input Handling Helpers -------------

// Remove commas from a string for number parsing
string removeCommas(const string& str) {
    string result;
    for (char c : str)
        if (c != ',') result += c;
    return result;
}

// Helper function to check if a string is a valid double (after commas removed)
bool isValidDouble(const string& input) {
    bool seenDecimal = false, seenDigit = false;
    size_t i = 0;
    // Skip leading whitespace
    while (i < input.length() && isspace(input[i])) i++;
    // Optional sign
    if (i < input.length() && (input[i] == '-' || input[i] == '+')) i++;
    // Main part
    for (; i < input.length(); ++i) {
        if (isdigit(input[i])) {
            seenDigit = true;
        } else if (input[i] == '.') {
            if (seenDecimal) return false;
            seenDecimal = true;
        } else if (!isspace(input[i])) {
            // Any other character is invalid
            return false;
        }
    }
    return seenDigit;
}

// Helper function to check if a string is a valid int (after commas removed)
bool isValidInt(const string& input) {
    size_t i = 0;
    while (i < input.length() && isspace(input[i])) i++;
    if (i < input.length() && (input[i] == '-' || input[i] == '+')) i++;
    bool seenDigit = false;
    for (; i < input.length(); ++i) {
        if (isdigit(input[i])) {
            seenDigit = true;
        } else if (!isspace(input[i])) {
            return false;
        }
    }
    return seenDigit;
}

// Prompt the user and return a validated double (supports commas)
double getValidatedDouble(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        string cleanInput = removeCommas(input);
        if (isValidDouble(cleanInput)) {
            try {
                double value = stod(cleanInput);
                if (value >= 0)
                    return value;
                else
                    cout << "Value must be non-negative." << endl;
            } catch (...) {
                cout << "Invalid number format. Try again." << endl;
            }
        } else {
            cout << "Error: Input must be a valid non-negative number (digits, optional decimal, and commas only, no other symbols or letters)." << endl;
        }
    }
}

// Prompt the user and return a validated positive integer (supports commas)
int getValidatedInt(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        string cleanInput = removeCommas(input);
        if (isValidInt(cleanInput)) {
            try {
                int value = stoi(cleanInput);
                if (value > 0)
                    return value;
                else
                    cout << "Value must be a positive integer." << endl;
            } catch (...) {
                cout << "Invalid number format. Try again." << endl;
            }
        } else {
            cout << "Error: Input must be a positive whole number (digits and commas only, no other symbols or letters)." << endl;
        }
    }
}

// ------------------- MAIN PROGRAM -------------------
int main() {
    cout << "----------------------------------------------" << endl;
    cout << "  Welcome to the Airgead Banking Calculator!" << endl;
    cout << "----------------------------------------------" << endl;

    InvestmentCalculator calculator;
    char tryAgain = 'n';

    do {
        // --- Input collection with validation and error messaging ---
        double initial, deposit, interest;
        int years;

        initial = getValidatedDouble("Enter Initial Investment Amount: $");
        while (!calculator.setInitialInvestment(initial)) {
            cout << "Amount must be non-negative." << endl;
            initial = getValidatedDouble("Enter Initial Investment Amount: $");
        }

        deposit = getValidatedDouble("Enter Monthly Deposit: $");
        while (!calculator.setMonthlyDeposit(deposit)) {
            cout << "Amount must be non-negative." << endl;
            deposit = getValidatedDouble("Enter Monthly Deposit: $");
        }

        interest = getValidatedDouble("Enter Annual Interest Rate (in %): ");
        while (!calculator.setAnnualInterest(interest)) {
            cout << "Rate must be non-negative." << endl;
            interest = getValidatedDouble("Enter Annual Interest Rate (in %): ");
        }

        years = getValidatedInt("Enter Number of Years: ");
        while (!calculator.setNumYears(years)) {
            cout << "Years must be a positive integer." << endl;
            years = getValidatedInt("Enter Number of Years: ");
        }

        // --- Show the entered data and wait for user to continue ---
        calculator.printInputs();
        cout << "Press Enter to continue...";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- Generate and display the two required investment reports ---
        calculator.printReport(calculator.calculateWithoutMonthlyDeposits(), false);
        calculator.printReport(calculator.calculateWithMonthlyDeposits(), true);

        // --- Option to restart ---
        cout << "\nWould you like to run another calculation? (y/n): ";
        cin >> tryAgain;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        calculator.clearData();
        cout << endl;

    } while (tryAgain == 'y' || tryAgain == 'Y');

    cout << "Thank you for using Airgead Banking Calculator. Goodbye!" << endl;
    return 0;
}
