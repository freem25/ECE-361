AI Usage
Tool Used
Microsoft Copilot (GPT-5)

How It Was Used
The AI was used to:

Explain homework requirements
Explain C pointers and pointer arithmetic
Review implementations of ptr.c
Help create test cases in tests/test_ptr.c
Help create the Makefile
Explain compilation and Git commands
Verification
All code was reviewed, compiled, and tested by the student before submission.

One Thing the Model Got Wrong
The model initially suggested that the Makefile was correct, but it placed ptr.o as the first target. As a result, running make built only ptr.o instead of building all targets. The Makefile was corrected by making all the primary target.
