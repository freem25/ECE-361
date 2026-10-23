# Homework 4 — AI Usage

## Tools Used

I used ChatGPT to help me understand the homework requirements, explain C programming concepts, and assist with writing and testing my code.

## How I Used AI

- **Part 1:** ChatGPT helped me design and implement the tally abstract data type, including the opaque structure, static memory allocation, alphabetical insertion, and the tally functions.
- **Part 2:** ChatGPT helped me write the events program, create test cases, and set up the Makefile.
- **Part 3:** ChatGPT helped explain opaque types, demonstrate the compiler error caused by directly accessing a hidden structure member, and document the result.
- **Testing and debugging:** I used ChatGPT to help interpret GCC output, test results, and Git commands while working in Ubuntu.

## One Thing the AI Got Wrong

ChatGPT originally told me that my comprehensive test program should run 12 tests. However, when I compiled and ran the program, it reported 13 tests. After reviewing the test code, we determined that there were actually 13 test checks, so the expected number of tests was incorrect. The correct result was 13 tests passed and 0 failed.

## Verification

I compiled and ran my code in Ubuntu using GCC, checked the program output, ran the unit tests, and used AddressSanitizer and UndefinedBehaviorSanitizer during testing. I also committed and pushed the work to my GitHub repository.
