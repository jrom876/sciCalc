#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "decToBinCalc.h"
#include "tempCalc.h"
#include "ustripZCalc.h"
#include "vswrCalc.h"
#include "fresnelCalc.h"
#include "linkBudgetCalc.h"
#include "antennaGainCalc.h"
#include "inrushICalc.h"
#define DATA_SIZE 1000

void getInputMain(){
  int exitFlag = 0;
	int funct;
  //float vfwd, vref, gamma, pfwd, pref;
	printf("Please choose section: \n");
	printf("0 for decToBinCalc.c\n");
	printf("1 for antennaGainCalc.c\n");
	//printf("2 for bpfCalc.c \n");
	printf("3 for fresnelCalc.c \n");
	printf("4 for linkBudgetCalc.c \n");
	printf("5 for inrushICalc.c\n");
	printf("6 for tempCalc.c \n");
	printf("7 for ustripZCalc.c\n");
	printf("8 for vswrCalc.c \n");
	//printf("9 for store data (disabled)\n");
	printf("10 for quit\n");
	scanf(" %i", &funct);
  switch(funct){
    case 0:   // decToBinCalc.c
      getUserInputDTB();
      break;
    case 1:   // antennaGainCalc.c
      getUserInputAG();
      break;
    //case 2:   // bpfCalc.c
      //getUserInputBPF();
      //break;
    case 3:   // fresnelCalc.c
      getUserInputFC();
      break;
    case 4:   // linkBudgetCalc.c
      getUserInputLB();
      break;
    case 5:   // inrushICalc.c
      setValuesIC();
      getUserInputIC();
      break;
    case 6:   // tempCalc.c
      getUserInputTC();
      break;
    case 7:   // ustripZCalc.c
      getUserInputUS();
      break;
    case 8:   // vswrCalc.c
      getUserInputVSWR();
      break;
    case 9:   // store data
      //
      break;
    case 10: // quit
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exitFlag = 1;
			break;
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
