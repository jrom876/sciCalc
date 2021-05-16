//	Package:	sciCalc
//	File:		main.c
//	Purpose:	Main, runs the primary user interface
//	Author:		jrom876

/**
	Copyright (C) 2019, 2021 
	Jacob Romero, Creative Engineering Solutions, LLC
	cesllc876@gmail.com
	admin@jrom.io 

	This program is free software; you can redistribute it
	and/or modify it under the terms of the GNU General Public  
	License as published by the Free Software Foundation, version 2.

	This program is distributed in the hope that it will be
	useful, but WITHOUT ANY WARRANTY; without even the implied 
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	
	See the GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public
	License along with this program; if not, write to:
	The Free Software Foundation, Inc.
	59 Temple Place, Suite 330
	Boston, MA 02111-1307 USA

**/

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "decToBinCalc.h"
#include "tempCalc.h"
#include "bpf2Calc.h"
#include "ustripZCalc.h"
#include "vswrCalc.h"
#include "fresnelCalc.h"
#include "primeNumsCalc.h"
#include "linkBudgetCalc.h"
#include "antennaGainCalc.h"
#include "inrushICalc.h"
#include "complexCalc.h"
#define DATA_SIZE 1000

void getInputMain(){
  int exitFlag = 0;
	int funct;
  //float vfwd, vref, gamma, pfwd, pref;
	printf("Please choose a section: \n");
	printf("\nNumber Calculators: \n");
	printf("\t0 for complexCalc.c\n");
	printf("\t1 for decToBinCalc.c\n");
	printf("\t2 for primeNumsCalc.c\n");
	
	printf("\nRF Value Calculators: \n");	
	printf("\t3 for linkBudgetCalc.c \n");
	printf("\t4 for antennaGainCalc.c\n");
	printf("\t5 for fresnelCalc.c \n");	
	printf("\t6 for vswrCalc.c \n");	
	
	printf("\nPCB Circuit Design Calculators: \n");	
	printf("\t7 for ustripZCalc.c\n");
	printf("\t8 for inrushICalc.c\n");
	printf("\t9 for bpf2Calc.c \n");

	printf("\nScientific Value Calculators: \n");	
	printf("\t10 for tempCalc.c \n");
	printf("\n\t11 for quit\n");
	scanf(" %i", &funct);
  switch(funct){
    case 0:   // complexCalc.c
      getUserInputCC();
      break;
    case 1:   // decToBinCalc.c
      getUserInputDTB();
      break;
    case 2:   // primeNumsCalc.c
      genPrimeNums();
      break;
    case 3:   // linkBudgetCalc.c
      getUserInputLB();
      break;      
    case 4:   // antennaGainCalc.c
      getUserInputAG();
      break;
    case 5:   // fresnelCalc.c
      getUserInputFC();
      break;
    case 6:   // vswrCalc.c
      getUserInputVSWR();
      break;
    case 7:   // ustripZCalc.c
      getUserInputUS();
      break;
    case 8:   // inrushICalc.c
      setValuesIC();
      getUserInputIC();
      break;      
    case 9:   // bpfCalc.c
      getUserInputBPF2();
      break;
    case 10:   // tempCalc.c
      getUserInputTC();
      break;
    case 11: // quit
      printf("Quitting\n");
      exitFlag = 1;
      break;
    default:
      printf("Wrong choice. Now exiting.\n");
      exitFlag = 1;
      exit(0);
    }
  if(0 != exitFlag) exit(0);
  getInputMain();
}
////////////////////////

int main(int argc, char const *argv[]) {
  //printf("Test before\n");
  getInputMain();
  //printf("Test after\n");
  return 0;
}
