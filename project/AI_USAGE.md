
## Week 8: Binary Search Tree Implementation

### AI Tool Used

ChatGPT (OpenAI)

### How AI Was Used

ChatGPT assisted with replacing the temporary sorted
array in store.c with a binary search tree based on
the concepts practiced in Homework 8.

AI assistance included:

- Updating store.h with the required BST interface.
- Implementing binary search tree operations in store.c.
- Preserving duplicate humidity keys as separate nodes.
- Creating tests for insertion, searching, and traversal.
- Testing tree size and height.
- Updating the project Makefile.
- Documenting the implementation in SPEC.md.
- Providing compilation and sanitizer testing commands.
- Adding brief explanatory comments to the code.

### AI Mistake and Correction

ChatGPT initially provided an incorrect expected tree
height in test_store_week4.c.

The test expected a height of 3 after inserting
humidity keys 45, 30, and 45.

The correct tree height was 2 because the root
has two children, giving a longest path of two nodes.

The expected test result was corrected, and the
updated test passed.

### Testing and Verification

The updated project was compiled using GCC with
the C11 standard and warning flags.

The full test suite passed:

- Week 4 storage: 12 tests
- Week 8 BST: 19 tests
- FSM: 16 tests
- Status word: 8 tests
- Circular buffer: 10 tests

Total: 65 tests passed, 0 failed.

The project was also rebuilt and tested using
AddressSanitizer and UndefinedBehaviorSanitizer.

No sanitizer errors were reported.

### Remaining Integration

The storage BST is implemented and unit-tested.

Inserting complete records during scenario replay
and implementing the reporting functionality are
still future project tasks.
