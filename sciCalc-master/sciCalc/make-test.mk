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
		
OBJ = lbtest.o \
		bpf2test.o \
		bpf3test.o \
		inrushItest.o \
		ustripZtest.o
	
DEBUG=-g
LIBS=-lcheck -lm -lpthread -lrt -lsubunit -lcheck_pic

#************************************************************************
##### AUTOMATED TEST BATTERIES ##### 
all: lbtest bpf2test bpf3test inrushItest ustripZtest

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

## Band Pass Filter 
bpf3test.o: bpf3test.c bpf1Calc.h bpf2Calc.h bpf3Calc.h
	checkmk bpf3test.check >bpf3test.c
	$(CC) $(CFLAGS) -c bpf3test.c	
bpf3test: bpf3test.o 
	$(CC) -o bpf3test bpf3test.o $(LIBS)

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
	rm -f $(OBJ)
	
#************************************************************************
#*** Targets ***

#~ main: main.o
#~ 	./main

#~ main.o : main.c \
#~ 		tempCalc.h \
#~ 		vswrCalc.h \
#~ 		ustripZCalc.h \
#~ 		fresnelCalc.h \
#~ 		linkBudgetCalc.h
#~ 	$(CC) -c main.c $(LIBS)

#~ tempCalc.o : tempCalc.c tempCalc.h
#~ 	$(CC) -c tempCalc.c $(LIBS)

#~ vswrCalc.o : vswrCalc.c vswrCalc.h
#~ 	$(CC) -c vswrCalc.c $(LIBS)

#~ ustripZCalc.o : ustripZCalc.c ustripZCalc.h
#~ 	$(CC) -c ustripZCalc.c $(LIBS)

#~ fresnelCalc.o : fresnelCalc.c fresnelCalc.h
#~ 	$(CC) -c fresnelCalc.c $(LIBS)

#~ linkBudgetCalc.o : linkBudgetCalc.c linkBudgetCalc.h
#~ 	$(CC) -c linkBudgetCalc.c $(LIBS)

#~ antennaGainCalc.o : antennaGainCalc.c linkBudgetCalc.h
#~ 		$(CC) -c antennaGainCalc.c $(LIBS)

#~ clean:
#~ 	rm main main.o tempCalc.o vswrCalc.o ustripZCalc.o fresnelCalc.o linkBudgetCalc.o antennaGainCalc.o

#~ lbtest: lbtest.o
#~ 	./lbtest
#~ bpf2test: bpf2test.o
#~ 	./bpf2test
