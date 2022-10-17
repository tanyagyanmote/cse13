# Assignment 3: I Have a Little Dreidel

Simulated a centuries-old game of chance, played using a four-sided
top known as a dreidel. 

## Files included
 1. Dreidel.c: This file contains the function spin_dreidel() and play_game()
 2. Play-dreidel.c:  This file will call my functions from dreidel.c
 3. Dreidel.h : This file contains definitions for functions defined in dreidel.c and used by other code  
 4. Makefile: This file will allow the grader to type make to compile your program.
 5. README.md: This file will describe how to build and run my program and list the  command-line options it accepts and what they do.
 6. DESIGN.pdf: Describe the purpose of your program and communicate the overall design of  the program with enough detail
 7. WRITEUP.pdf: Discussion of the results of my tests.
 8. Mtrand.h: Provided in asgn3-files.tgz.
 9. Mtrand.c: Provided in asgn3-files.tgz 



## Compile / Building the program

```python
make
```

## Run Code

```
./play-dreidel -p -c -s -v
```

-p corresponds to the number of players

-c corresponds to the number of coins given to a player

-s corresponds to the seed number

-v will print a statement when a player gets eliminated
