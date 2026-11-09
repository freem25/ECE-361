# AI Usage — ECE 361 Homework 6

## AI Tool Used

OpenAI ChatGPT was used as an assistance tool throughout Homework 6.

## How AI Was Used

AI assisted with the development, explanation, debugging, and testing of the homework's C modules.

**Part 1 — Sensor**

- Assisted with implementing `sensor.h` and `sensor.c`.
- Explained the sensor register interface, 20-bit masking, temperature and humidity conversion formulas, rounding, and error handling.
- Helped develop tests for sensor readings, timestamps, and failed register operations.

**Part 2 — Switches and LEDs**

- Assisted with implementing `panel.h` and `panel.c`.
- Explained bitwise AND, OR, XOR, NOT, and left-shift operations.
- Explained read-modify-write and why other LED states must be preserved.
- Helped develop tests for switch presses, releases, held switches, LED toggling, and invalid LED numbers.

**Part 3 — Scenario and Replay**

- Assisted with designing `scenario.h`, `scenario.c`, and `main.c`.
- Explained file reading, scenario parsing, timestamps, arrays, pointers, and error handling.
- Helped create valid and invalid scenario files.
- Assisted with testing held values, invalid input lines, scenario limits, and switch-controlled LED behavior.

**Part 4 — Testing and Build System**

- Assisted with writing and extending `tests/test_hw06.c`.
- Helped create the Makefile and regression-testing script.
- Assisted with preparing scenario files and expected trace outputs.
- Helped prepare the README and AI usage documentation.

## Errors and Corrections

Several issues were identified and corrected during development:

1. Added a missing `#include "panel.h"` declaration to the test file after GCC reported implicit function declaration errors.
2. Corrected the placement of the Part 3 tests inside `main()` after compilation errors.
3. Corrected test fixtures containing unintended leading blank lines that changed the expected physical line numbers.
4. Removed temporary debugging statements after identifying and resolving test failures.
5. Corrected the placement of the test summary so that it reports the total number of completed tests.

Compiler errors, test output, and terminal results were used to diagnose these issues.

## Verification

The implementation was compiled using GCC with the C11 standard and strict warning flags.

The completed test commands included:

- `make`
- `make test`
- `make clean`
- `./replay scenarios/hold_values.txt`
- `./replay scenarios/toggle_sequence.txt`

At the latest successful verification:

- 24 unit tests passed.
- 2 regression scenarios passed.
- No test failures were reported.
- The build completed without compiler warnings or errors.

## Review and Responsibility

AI-generated suggestions and code were entered into the project files and tested using Ubuntu WSL and GCC.

The implementation was checked against the Homework 6 instructions and the ECE 361 project specification. The debugging process included reviewing compiler diagnostics and correcting code or test data as necessary.

AI was used as a development and learning aid. The submitted files and their behavior remain my responsibility.
