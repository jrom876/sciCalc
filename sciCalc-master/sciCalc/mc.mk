#************************************************************************
#	Package:	sciCalc
#	File:		mc.mk
#	Purpose:	Main Makefile for Project sciCalc
#	Author:		jrom876
#************************************************************************

CC=gcc
DEPS = tempCalc.h \
	vswrCalc.h \
	ustripZCalc.h \
	bpf1Calc.h \
	bpf2Calc.h \
	bpf3Calc.h \
	fresnelCalc.h \
	antennaGainCalc.h \
	linkBudgetCalc.h \
	inrushICalc.h \
	decToBinCalc.h \
	primeNumsCalc.h \
	complexCalc.h
OBJ = main.o \
	tempCalc.o \
	vswrCalc.o \
	ustripZCalc.o \
	bpf1Calc.o \
	bpf2Calc.o \
	bpf3Calc.o \
	fresnelCalc.o \
	antennaGainCalc.o \
	linkBudgetCalc.o \
	inrushICalc.o \
	decToBinCalc.o \
	primeNumsCalc.o \
	complexCalc.o
DEBUG=-g
LIBS=-lpthread -lm -lrt

#*** Targets ***

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LIBS) 

main: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)
	
#~ rxdbm: lbtest-test
#~ 	./lbtest-test

#~ lbtest-test: lbtest-test.o linkBudgetCalc.o
#~ 	$(CC) -o lbtest-test linkBudgetCalc.o lbtest-test.o $(LIBS)
	
clean:
	rm -f $(OBJ)/*.o
