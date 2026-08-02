#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
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

// Struct to compare the two calculated investment paths
struct ScenarioSummary {
    double finalBalanceWithoutDeposits;
    double finalBalanceWithDeposits;
    double totalInterestWithoutDeposits;
    double totalInterestWithDeposits;
};


// Sum the interest earned across all years in a report
double calculateTotalInterest(const vector<YearEndReport>& report) {
    double totalInterest = 0.0;

    for (const auto& entry : report) {
        totalInterest += entry.interestEarned;
    }

    return totalInterest;
}

// Print a concise comparison between the two generated investment paths
void printScenarioComparison(const vector<YearEndReport>& withoutDeposits,
                             const vector<YearEndReport>& withDeposits) {
    if (withoutDeposits.empty() || withDeposits.empty()) {
        cout << "\nNo scenario comparison available." << endl;
        return;
    }

    ScenarioSummary summary {
        withoutDeposits.back().yearEndBalance,
        withDeposits.back().yearEndBalance,
        calculateTotalInterest(withoutDeposits),
        calculateTotalInterest(withDeposits)
    };

    double depositAdvantage = summary.finalBalanceWithDeposits - summary.finalBalanceWithoutDeposits;

    cout << "\n   Investment Scenario Comparison" << endl;
    cout << "==============================================================" << endl;
    cout << "Final Balance Without Deposits: $" << formatCurrency(roundTo2(summary.finalBalanceWithoutDeposits)) << endl;
    cout << "Final Balance With Deposits:    $" << formatCurrency(roundTo2(summary.finalBalanceWithDeposits)) << endl;
    cout << "Additional Ending Balance:      $" << formatCurrency(roundTo2(depositAdvantage)) << endl;
    cout << "Interest Without Deposits:      $" << formatCurrency(roundTo2(summary.totalInterestWithoutDeposits)) << endl;
    cout << "Interest With Deposits:         $" << formatCurrency(roundTo2(summary.totalInterestWithDeposits)) << endl;
    cout << "==============================================================" << endl;
}


// Calculate final balance for a proposed monthly deposit.
// This supports the binary search target-deposit estimator.
double calculateFinalBalanceWithMonthlyDeposit(double initialInvestment,
                                               double monthlyDeposit,
                                               double annualInterest,
                                               int years) {
    double balance = initialInvestment;
    double interestRate = annualInterest / 100.0;

    for (int year = 1; year <= years; ++year) {
        for (int month = 1; month <= 12; ++month) {
            balance += monthlyDeposit;
            double interest = balance * (interestRate / 12.0);
            balance += interest;
        }
    }

    return balance;
}

// Estimate the minimum monthly deposit needed to reach a target balance.
// This uses binary search instead of forcing the user to guess repeatedly.
double estimateMonthlyDepositForTarget(double initialInvestment,
                                       double annualInterest,
                                       int years,
                                       double targetBalance) {
    double balanceWithoutDeposits = calculateFinalBalanceWithMonthlyDeposit(
        initialInvestment,
        0.0,
        annualInterest,
        years
    );

    if (balanceWithoutDeposits >= targetBalance) {
        return 0.0;
    }

    double lowDeposit = 0.0;
    double highDeposit = 100.0;

    while (calculateFinalBalanceWithMonthlyDeposit(initialInvestment, highDeposit, annualInterest, years) < targetBalance) {
        highDeposit *= 2.0;

        if (highDeposit > 1000000.0) {
            return -1.0;
        }
    }

    while ((highDeposit - lowDeposit) > 0.01) {
        double midDeposit = (lowDeposit + highDeposit) / 2.0;
        double projectedBalance = calculateFinalBalanceWithMonthlyDeposit(
            initialInvestment,
            midDeposit,
            annualInterest,
            years
        );

        if (projectedBalance >= targetBalance) {
            highDeposit = midDeposit;
        } else {
            lowDeposit = midDeposit;
        }
    }

    return highDeposit;
}

// Display target-balance planning output for the user.
void printTargetDepositEstimate(double initialInvestment,
                                double annualInterest,
                                int years,
                                double targetBalance) {
    double estimatedDeposit = estimateMonthlyDepositForTarget(
        initialInvestment,
        annualInterest,
        years,
        targetBalance
    );

    cout << "\n   Target Balance Planning" << endl;
    cout << "==============================================================" << endl;
    cout << "Target Balance: $" << formatCurrency(roundTo2(targetBalance)) << endl;

    if (estimatedDeposit < 0) {
        cout << "A reasonable monthly deposit estimate could not be calculated." << endl;
    } else {
        cout << "Estimated Minimum Monthly Deposit: $"
             << formatCurrency(roundTo2(estimatedDeposit)) << endl;
    }

    cout << "==============================================================" << endl;
}


// Stores one possible monthly-deposit scenario for comparison and ranking
struct InvestmentScenario {
    string label;
    double monthlyDeposit;
    double finalBalance;
};

// Build and rank common monthly-deposit scenarios using vector storage and std::sort
void printRankedMonthlyDepositScenarios(double initialInvestment,
                                        double baseMonthlyDeposit,
                                        double annualInterest,
                                        int years) {
    vector<InvestmentScenario> scenarios = {
        {"No Monthly Deposit", 0.0, 0.0},
        {"Current Monthly Deposit", baseMonthlyDeposit, 0.0},
        {"Current + $50 Monthly", baseMonthlyDeposit + 50.0, 0.0},
        {"Current + $100 Monthly", baseMonthlyDeposit + 100.0, 0.0}
    };

    for (auto& scenario : scenarios) {
        scenario.finalBalance = calculateFinalBalanceWithMonthlyDeposit(
            initialInvestment,
            scenario.monthlyDeposit,
            annualInterest,
            years
        );
    }

    sort(scenarios.begin(), scenarios.end(), [](const InvestmentScenario& first,
                                                const InvestmentScenario& second) {
        return first.finalBalance > second.finalBalance;
    });

    cout << "\n   Ranked Monthly Deposit Scenarios" << endl;
    cout << "==============================================================" << endl;
    cout << left << setw(30) << "Scenario"
         << right << setw(18) << "Monthly"
         << setw(18) << "Final Balance" << endl;
    cout << "--------------------------------------------------------------" << endl;

    for (const auto& scenario : scenarios) {
        cout << left << setw(30) << scenario.label
             << right << setw(18) << ("$" + formatCurrency(roundTo2(scenario.monthlyDeposit)))
             << setw(18) << ("$" + formatCurrency(roundTo2(scenario.finalBalance))) << endl;
    }

    cout << "==============================================================" << endl;
}

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

        // --- Generate, store, and display the investment reports ---
        vector<YearEndReport> withoutDeposits = calculator.calculateWithoutMonthlyDeposits();
        vector<YearEndReport> withDeposits = calculator.calculateWithMonthlyDeposits();

        calculator.printReport(withoutDeposits, false);
        calculator.printReport(withDeposits, true);
        printScenarioComparison(withoutDeposits, withDeposits);
        printRankedMonthlyDepositScenarios(initial, deposit, interest, years);

        // --- Optional target-balance planning using binary search ---
        char estimateTarget = 'n';
        cout << "\nWould you like to estimate the monthly deposit needed for a target balance? (y/n): ";
        cin >> estimateTarget;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (estimateTarget == 'y' || estimateTarget == 'Y') {
            double targetBalance = getValidatedDouble("Enter Target Balance: $");
            printTargetDepositEstimate(initial, interest, years, targetBalance);
        }

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
