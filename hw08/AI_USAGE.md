
# ECE 361 - Homework 8
## AI Usage Documentation

### AI Tool Used
- ChatGPT (OpenAI)

### How AI Was Used

ChatGPT was used to assist with the development,
debugging, testing, and documentation of Homework 8.

#### Part 1: BST ADT
- Assisted in adapting the professor-provided
  BinarySearchTree.c program into bst.h and bst.c.
- Helped implement the required BST functions.
- Added brief comments explaining the code.
- Provided GCC commands for compilation checks.

#### Part 2: Testing and Debugging
- Assisted in creating tests/test_bst.c.
- Helped develop the Makefile.
- Helped reproduce the original search bug using
  AddressSanitizer and UndefinedBehaviorSanitizer.
- Assisted in fixing the NULL pointer error
  in bst_contains().
- Helped verify insertion, searching, duplicates,
  tree height, and memory cleanup.

#### Part 3: Tree Statistics
- Assisted in developing treestats.c.
- Helped update the Makefile.
- Provided commands for testing increasing and
  shuffled insertion orders.
- Assisted in documenting the experiment results.

### Verification and Corrections

- Compiled the C programs using GCC in Ubuntu WSL.
- Ran unit tests and memory checks.
- Reproduced and documented the original search
  crash before correcting it.
- Verified the required treestats outputs.
- Confirmed that inserting 1023 increasing keys
  produced height 1023.
- Confirmed that shuffled insertion produced
  height 25 in the recorded experiment.
- Used Git to commit and push completed work
  to the ECE-361 repository.

### Summary

AI was used as a programming and debugging
assistant. The generated code was compiled and
tested against the homework requirements.
