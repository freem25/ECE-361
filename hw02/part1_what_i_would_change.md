#Part 1: What I Would Change

##1. Stop Repeating Same Calculations

The program does the same thing twice when finding the min/max/avg for temp & humidity.

Instead of having the same code twice just make one function that works for both. It makes the code shorter and a lot easier to deal with.

##2. Get Rid of Global Variables

The program has global variables like `ticks`, `temps`, `hums`, `count`, `skipped`, `threshold`.

These should be local variables or put into a structure instead. That way the functions only use what they need and its easier to figure out whats going on.

##3. Make the main() Function Simpler

`main()` is doing too much. Its reading the sensor data and checking inputs and doing calculations and printing everything.

Breaking it into smaller functions makes more sense so each function has its own job. This makes the code easier to understand and easier to fix if something goes wrong.
