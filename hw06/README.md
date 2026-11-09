# ECE 361 — Homework 6

## Overview

Homework 6 implements a simulated sensor, switch and LED controls, scenario loading, and a replay program using the iom361_r4 hardware emulator.

The program reads temperature, relative humidity, and switches through the simulated hardware registers. Each newly pressed switch toggles its corresponding LED.

The replay program prints one timestamped trace line for every tick.

## Files

- `sensor.h`, `sensor.c` — Read temperature and humidity, convert raw 20-bit values, and round measurements to one decimal place.
- `panel.h`, `panel.c` — Detect switch presses/releases and control LEDs using read-modify-write operations.
- `scenario.h`, `scenario.c` — Load and validate scenario files, store up to 1,000 entries, and preserve values between changes.
- `main.c` — Replay scenarios, process switches, toggle LEDs, and print timestamped output.
- `iom361_r4.h`, `iom361_r4.c` — Version 4.0 simulated hardware library.
- `tests/test_hw06.c` — Unit tests for the sensor, panel, and scenario modules.
- `tests/fixtures/` — Input files for error-handling and boundary testing.
- `scenarios/` — Valid replay scenarios.
- `expected/` — Expected output traces.
- `regress.sh` — Regression testing script.
- `Makefile` — Build, test, and cleanup rules.

## Building

Run `make` from the `hw06` directory.

This compiles the required C modules using GCC and creates the `replay` executable.

## Running

Run:

`./replay scenarios/hold_values.txt`

Or:

`./replay scenarios/toggle_sequence.txt`

The program prints one trace line per tick containing the UTC timestamp, temperature, humidity, switches, newly pressed switches, and LED states.

## Scenario File Format

The first meaningful line must contain `start` followed by a Unix timestamp.

Each subsequent data line contains:

`tick temperature humidity switches`

Example:

    start 1791273600
    0 20.0 40.0 0x0
    2 21.5 40.0 0x1
    5 21.5 45.0 0x3

Switch values use hexadecimal notation. Comments begin with `#`, blank lines are ignored, and ticks must increase strictly.

Values remain unchanged between scenario entries. Scenarios can contain at most 1,000 data entries.

The current replay implementation expects an initial entry at tick 0.

## Testing

Run:

`make test`

This executes the unit tests first, then runs `regress.sh` to compare both replay scenarios with their expected traces.

At the time of testing:

- 24 unit tests passed.
- 2 regression scenarios passed.
- 0 test failures were reported.

## Cleaning

Run:

`make clean`

This removes generated executables and object files without deleting the source code, test fixtures, or scenario files.

## Error Handling

The scenario loader reports the physical line number of the first invalid scenario entry.

The program returns a nonzero exit status when scenario loading or required hardware operations fail.
