# sciCalc
A simple, expandable user interface (UI) written in C for calculating various scientific values of use to the RF Hardware Engineer.

It contains: 

      executables to test each .c file 

      a main executable UI 

      2 different makefile versions

To run main on the ubuntu linux terminal, type the following:

      gcc -o main main.c -lm -lrt

      ./main

To make (update and recompile dependencies) using mc.mk, type:

      make -f mc.mk

Please do not use the bpf2Calc and bpf3Calc files until further notice.

This version is in early development and still needs documentation, verification, optimization, unit clarification, and general clean-up. 
