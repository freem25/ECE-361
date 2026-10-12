# Homework 2 - Modular Sensor Readings

## Building the Program

This program reads sensor data and calculates the minimum and maximum and average values for temperature and humidity. It also shows the longest run above a temperature threshold and prints a histogram.

To compile the program run:

```bash
make
```

To run the program with normal input:

```bash
./readings < data/normal.txt
```

To run it with a threshold of 25:

```bash
./readings 25 < data/edge.txt
```

## Testing

To run all the unit tests and regression tests:

```bash
make test
```

To remove the compiled files:

```bash
make clean
```

There are four statistics unit tests and four regression tests. All eight passed during development.

## Module Diagram

```text
                 main.c
                   |
          +--------+---------+
          |        |         |
          v        v         v
      reading.c  stats.c  histogram.c
          |        |         |
          v        v         v
      reading.h  stats.h  histogram.h
```

## Modules

- **main.c** - Runs the program and connects everything together.
- **reading.c** - Reads the sensor data and skips invalid lines.
- **stats.c** - Calculates the minimum and maximum and mean along with the longest run above the threshold.
- **histogram.c** - Creates and prints the temperature histogram.

Each module has its own header file.

The goal was to split up the original program into smaller files while keeping the output the same.
