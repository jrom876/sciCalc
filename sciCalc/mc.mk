#************************************************************************
#  Makefile file for project sciCalc
#************************************************************************

CC=gcc
DEPS = tempCalc.h \
	vswrCalc.h \
	ustripZCalc.h \
	bpf2Calc.h \
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
	bpf2Calc.o \
	fresnelCalc.o \
	antennaGainCalc.o \
	linkBudgetCalc.o \
	inrushICalc.o \
	decToBinCalc.o \
	primeNumsCalc.o \
	complexCalc.o
DEBUG=-g
#LIBS=-lm
LIBS=-lpthread -lm -lrt

#*** Targets ***

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(LIBS) 

main: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f $(OBJ)/*.o
