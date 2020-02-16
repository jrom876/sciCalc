# sciCalc
A simple, expandable user interface (UI) written in C for calculating various scientific values of use to the RF Hardware Engineer.

It contains: 

      executables to test each .c file 

      a main executable UI 

      2 different makefile versions

To run main in the ubuntu linux terminal, type the following:

      gcc -o main main.c -lm -lrt

      ./main

To update and recompile dependencies (make) using mc.mk, type:

      make -f mc.mk

Please do not use the bpf2Calc.h and bpf3Calc.c files until further notice.

This version is in early development and still needs verification, optimization, unit clarification, and clean-up.
