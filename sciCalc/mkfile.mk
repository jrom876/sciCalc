#************************************************************************
#  Makefile file for project sciCalc
#************************************************************************

CC=gcc
CFLAGS=-I
DEPS = tempCalc.h vswrCalc.h ustripZCalc.h fresnelCalc.h linkBudgetCalc.h antennaGainCalc.h
OBJ = main.o tempCalc.o vswrCalc.o ustripZCalc.o fresnelCalc.o linkBudgetCalc.o antennaGainCalc.o
DEBUG=-g
LIBS=-lm -lrt -lpthread

 main : \
 	main.o \
	tempCalc.o \
	vswrCalc.o \
	ustripZCalc.o \
	fresnelCalc.o \
	antennaGainCalc.o \
	linkBudgetCalc.o

	$(CC) -o main main.o tempCalc.o vswrCalc.o ustripZCalc.o fresnelCalc.o linkBudgetCalc.o antennaGainCalc.o $(LIBS)

#*** Targets ***

main.o : main.c tempCalc.h \
	vswrCalc.h ustripZCalc.h \
	fresnelCalc.h linkBudgetCalc.h
	$(CC) -c main.c $(LIBS)

tempCalc.o : tempCalc.c tempCalc.h
	$(CC) -c tempCalc.c $(LIBS)

vswrCalc.o : vswrCalc.c vswrCalc.h
	$(CC) -c vswrCalc.c $(LIBS)

ustripZCalc.o : ustripZCalc.c ustripZCalc.h
	$(CC) -c ustripZCalc.c $(LIBS)

fresnelCalc.o : fresnelCalc.c fresnelCalc.h
	$(CC) -c fresnelCalc.c $(LIBS)

linkBudgetCalc.o : linkBudgetCalc.c linkBudgetCalc.h
	$(CC) -c linkBudgetCalc.c $(LIBS)

antennaGainCalc.o : antennaGainCalc.c linkBudgetCalc.h
	$(CC) -c antennaGainCalc.c $(LIBS)

clean:
	rm main main.o tempCalc.o vswrCalc.o ustripZCalc.o fresnelCalc.o linkBudgetCalc.o antennaGainCalc.o
