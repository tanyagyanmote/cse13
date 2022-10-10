# Assignment 2 - A Small Numerical Library

This program uses mathlib.c, includes functions of sin,cos,arcsin,arccos,arctan, and log to compute Taylor series commands' of these functions.

## Files included
```
1. mathlib.h: Function prototypes for the math functions 
2. mathlib.c: Contains math function implementations
3. mathlib-test.c: This file will contain the main() program and act as a math library test harness. 
4. Makefile: This file will allow grader to type make to compile your program.
5. README.md: This file will describe how to build and run the program and list the command-line options it accepts and what they do.
6. DESIGN.pdf: Describe the purpose of the program and communicate the overall design of the program with enough detail
7. WRITEUP.pdf: Discussion of the results

```
## Compile / Building the program

```bash
$make
```

## Run Code
To run certain function type -(x), x replacing the corresponding letter. For example (s = sin).

```python
$./mathlib-test
./mathlib-test -a
./mathlib-test -s
./mathlib-test -c
./mathlib-test -S
./mathlib-test -C
./mathlib-test -T
./mathlib-test -l
```

## Sources
``` python
https://www.youtube.com/watch?v=tjd8fQw5HTA&ab_channel=BroCode
https://www.youtube.com/watch?v=ugfnD0xDDb4
https://stackoverflow.com/questions/10792015/gnuplot-plotting-multiple-line-graphs
```
