# CS-210 Airgead Banking Test Notes

## Build Test

Command used:

c++ -std=c++17 -Wall -Wextra -pedantic -o /tmp/airgead AirgeadBanking.cpp

Expected result:

- Program compiles successfully.
- Enhanced C++ code remains buildable from the command line.

## Manual Test Areas

The enhanced program should be tested for:

- Report generation with and without monthly deposits
- Scenario comparison summary output
- Ranked monthly deposit scenario output
- Target-balance monthly deposit estimator
- Restarting the calculator after one run

## Enhancement-Specific Test Expectations

The scenario comparison summary should display final balance, added balance from deposits, and interest totals.

The target-balance estimator should display an estimated monthly deposit when the user enters a target balance.

The ranked monthly deposit scenario output should show multiple monthly deposit options sorted by final projected balance.
