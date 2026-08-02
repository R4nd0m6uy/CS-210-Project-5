# CS 499 Algorithms and Data Structures Enhancement Summary

## Artifact

CS-210 Airgead Banking investment calculator.

## Enhancement Category

Algorithms and Data Structures.

## Enhancements Completed

The enhanced version improves the original calculator by adding:

- A clearer C++ source filename: AirgeadBanking.cpp
- Stored yearly report results using vector<YearEndReport>
- A scenario comparison summary
- A binary search target-deposit estimator
- Ranked monthly deposit scenarios using vector<InvestmentScenario> and std::sort

## Data Structures Used

The enhancement uses structured data to store and compare investment results:

- YearEndReport stores yearly investment results.
- ScenarioSummary stores final comparison values.
- InvestmentScenario stores ranked monthly deposit scenarios.
- vector<YearEndReport> stores generated report rows.
- vector<InvestmentScenario> stores multiple investment scenarios for sorting.

## Algorithms Used

The binary search estimator is the main algorithmic enhancement. Instead of requiring the user to manually guess monthly deposit amounts, the program estimates the minimum monthly deposit needed to reach a target balance.

The ranked scenario feature also uses sorting to organize different monthly deposit strategies by final balance. This makes the program more useful as a decision-support tool instead of only showing one calculation at a time.

## Course Outcome Connection

This enhancement supports the algorithms and data structures outcome by using structured data, vectors, sorting, repeated calculations, and binary search to solve a practical financial planning problem.
