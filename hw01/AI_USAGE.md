#AI Usage

#AI I used:
- ChatGPT

#What I used it for:
I used ChatGPT to help with:

- setting up WSL 2 and Ubuntu on Windows 11
- installing and checking GCC, Make, Git, and Bash
- cloning and organizing the GitHub repository
- understanding the required bit-manipulation functions
- writing and debugging `bits.h`, `bits.c`, `status.h`, and `status.c`
- creating test cases
- creating the Makefile
- understanding compiler and Git error messages

#a few things that went wrong:
During setup, a command was entered as `1s` using the number `1` instead of `ls` using a lowercase letter `l`. The AI initially treated this as if the Linux `ls` command might be missing and suggested troubleshooting the `coreutils` package.
The actual problem was only the typo. I discovered this by looking carefully at the command and noticing that `1s` had been entered instead of `ls`. After using the correct `ls` command, it worked normally.
I also had syntax errors in `bits.h` because two function declarations ended with `:` instead of `;`. GCC reported the exact file and line numbers. I opened `bits.h`, corrected the punctuation, recompiled, and verified that the program built successfully.
