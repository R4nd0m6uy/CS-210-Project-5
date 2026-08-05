# CS-210-Project-5
# Airgead Banking App (CS 210)

This project models investment growth over time using compound interest, with and without monthly deposits. Built in C++ as part of SNHU's CS 210 course, this app focuses on clean, readable code that follows industry standards and handles real-world user input with grace. It's like a crystal ball for your savings — minus the smoke and mirrors.

Project Summary
The Airgead Banking App is a C++ console program that allows users to model the growth of an investment over time with or without monthly contributions. The app solves the common personal finance problem of forecasting compound interest, offering two detailed reports based on user inputs. The program reflects both good coding practices and banking standards outlined in the project requirements, including input validation, clear formatting, and modular design.

What I Did Well
I implemented a clean and modular architecture using a class-based design with InvestmentCalculator, keeping the main function minimal and focused on user interaction. I also prioritized user input validation, making sure invalid inputs (like letters or negative numbers) wouldn’t break the program. Outputs were made clear and readable using currency formatting with commas and precise rounding.

Where I Could Improve
While the program is fully functional, some improvements could boost maintainability and performance:

Refactoring Repetitive Logic: Input validation could be abstracted further to reduce redundancy.
Error Handling: Adding exception handling for edge cases or future file I/O expansion would improve robustness.
Unit Testing: Currently, there are no automated tests. Implementing basic test coverage would make future changes safer.
These changes would not only clean up the code but also make it more scalable, especially if converted to a GUI or web application in the future.

Challenges and How I Solved Them
One challenge was formatting financial output with both commas and two decimal places — C++ formatting isn’t always intuitive. I used stringstreams with locale settings to format currency properly. Another was ensuring that user input handled commas gracefully while still validating number formats; I wrote custom functions to sanitize and check input strings. Debugging these took a lot of trial and error and reviewing of standard library documentation.

Transferable Skills
Input validation and user feedback: Essential for any user-facing application.
Object-oriented design: Clean separation of logic and user interaction improves maintainability.
Precision handling in financial software: Learned to handle rounding and formatting in a way that meets real-world expectations.
Maintainability and Readability
Code is clearly structured with consistent naming conventions (camelCase for variables, m_ for private members).
The class encapsulates all business logic, so enhancements don’t clutter main().
Functions are short and focused, with comments for clarity.
Edge cases like malformed input or zero values are handled gracefully.

## CS 499 Final Project Enhancement Notes

This repository is used as the algorithms and data structures artifact for CS 499. The original artifact was the CS-210 Airgead Banking investment calculator. The enhanced version improves the project by adding structured report storage, scenario comparison, a binary search target-deposit estimator, and ranked monthly deposit scenarios.

The primary algorithm and data structure improvements include:

- Renaming the source file to `AirgeadBanking.cpp` for cleaner C++ tooling.
- Using `vector<YearEndReport>` to store generated yearly report data.
- Adding `ScenarioSummary` to compare investment outcomes.
- Adding a binary search estimator to calculate the monthly deposit needed to reach a target balance.
- Adding `vector<InvestmentScenario>` and `std::sort` to rank monthly deposit scenarios by projected final balance.

The enhanced artifact supports the CS 499 algorithms and data structures category by using structured data, vectors, sorting, repeated financial calculations, and binary search to make the original calculator more useful as a decision-support tool.

