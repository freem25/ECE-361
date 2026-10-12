# AI Usage - Homework 2

## AI Tools Used

I used ChatGPT to help me work through Homework 2.

## What I Used It For

ChatGPT helped with splitting the original C program into separate modules and writing the header files. It also helped with making the Makefile and setting up the unit tests.

I also used it to explain GCC errors and figure out the Ubuntu commands needed to compile and test everything and upload the files to GitHub.

## Mistakes and Fixes

One problem happened when compiling `reading.c`. GCC gave an error saying it expected a declaration or statement at the end of the file. The function was missing a closing curly bracket.

I found the problem from the compiler error message and checked the bottom of the file. After adding the missing `}` the code compiled correctly.

Another problem happened in the Sauna Controller project. ChatGPT originally gave me `iom361_initialize(3, 3)` but the actual function required a third argument.

GCC showed the error and pointed to the function declaration in `iom361_r4.h`. The code was corrected to pass `&result` as the third argument and then it compiled.

## Verification

After fixing the errors I ran the unit tests and regression tests using `make test`. All eight Homework 2 tests passed. The iom361 library also passed all 36 of its self tests.

The final code was compiled and tested using Ubuntu WSL 2.
