#************************************************************************
#	Package:	sciCalc
#	File:		make-test.mk
#	Purpose:	Automated Test Batteries for Project sciCalc
#	Author:		jrom876
#************************************************************************
## https://stackoverflow.com/questions/12778430/creating-unit-testing-using-makefile
## https://www.brendanlong.com/unit-testing-c-with-check-and-autotools.html
#************************************************************************

CC=gcc
#CFLAGS=-I
DEPS = linkBudgetCalc.h \
		bpf1Calc.h \
		bpf2Calc.h \
		bpf3Calc.h \
		inrushICalc.h \
		ustripZCalc.h
		
OBJ = main-test.o \
		linkBudgetCalc.o \
		bpf1Calc.o \
		bpf2Calc.o \
		bpf3Calc.o \
		inrushICalc.o \
		ustripZCalc.o
	
DEBUG=-g
LIBS=-lcheck -lm -lpthread -lrt -lsubunit -lcheck_pic

#************************************************************************
##### AUTOMATED TEST BATTERIES ##### 
all: lbtest bpf2test inrushItest ustripZtest

## Link Budget 
lbtest.o: lbtest.c linkBudgetCalc.h
	checkmk lbtest.check >lbtest.c
	$(CC) $(CFLAGS) -c lbtest.c	
lbtest: lbtest.o 
	$(CC) -o lbtest lbtest.o $(LIBS)

## Band Pass Filter 
bpf2test.o: bpf2test.c bpf1Calc.h bpf2Calc.h bpf3Calc.h
	checkmk bpf2test.check >bpf2test.c
	$(CC) $(CFLAGS) -c bpf2test.c	
bpf2test: bpf2test.o 
	$(CC) -o bpf2test bpf2test.o $(LIBS)

## Inrush Current
inrushItest.o: inrushItest.c inrushICalc.h
	checkmk inrushItest.check >inrushItest.c
	$(CC) $(CFLAGS) -c inrushItest.c	
inrushItest: inrushItest.o 
	$(CC) -o inrushItest inrushItest.o $(LIBS)

## Microstrip Impedance
ustripZtest.o: ustripZtest.c ustripZCalc.h
	checkmk ustripZtest.check >ustripZtest.c
	$(CC) $(CFLAGS) -c ustripZtest.c	
ustripZtest: ustripZtest.o 
	$(CC) -o ustripZtest ustripZtest.o $(LIBS)

clean:
	rm -f $(OBJ)/*.o
	
