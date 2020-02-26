#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "linkBudgetCalc.h"
#define DATA_SIZE 1000
#define TXT_FILE "linkBudgetData.txt"
//
/// Globals ///
float freqGHz, txpwrW, distTargetKM;
float radarXSect, antGain;
float hmm, wmm, tmm;
float tempf;
//int exitFlag = 0;

/// Functions ///

float twoWayPathLossDB(float freq, float dist, float rxs){
  float result = ((20*log(freq))+(40*log(dist))+163.43-(10*log(rxs)))*(-1);
  printf("Path Loss (dB): \t%.5f\n",result);
  tempf = result;
  return result;
}

////////////////////////////////
////////////////////////////////
char * floatToStringLB(float f){
	char buffer[DATA_SIZE];
    	return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileLB(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringLB(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileLB(){
  	FILE * fPtr = fopen(TXT_FILE, "w");
  	fclose(fPtr);
}

int getUserInputLB(){
	float f, d, xs;
  	int exitFlag = 0;
	int var;
	printf("Please choose from the following: \n");
	printf("1 to calculate twoWayPathLossDB \n");
  	printf("6 to clear all data from linkBudgetData.txt\n");
	printf("7 for store to file\n");
	printf("8 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			printf("Please enter freq in Ghz: \n");
			scanf(" %f", &f);
			printf("Please enter distance in km: \n");
			scanf(" %f", &d);
			printf("Please enter radar cross section area in m^2: \n");
			scanf(" %f", &xs);
			twoWayPathLossDB(f,d,xs);
      			break;
		case 2:

			break;
		case 3:

			break;
		case 6:
			clearDataFromFileLB();
			printf("linkBudgetData.txt is now cleared of all data");
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileLB(tempf);
			break;
		case 8:
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exitFlag = 1;
			break;
	}
	if(0 != exitFlag) return 0;
	getUserInputLB();
}
////////////////////////
/// Main ///
// int main(int argc, char const *argv[]) {
//   twoWayPathLossDB(24, 50, 10);
//   getUserInputLB();
//   return 0;
// }
