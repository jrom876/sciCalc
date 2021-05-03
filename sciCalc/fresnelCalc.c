#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fresnelCalc.h"
#define DATA_SIZE 1000
#define TXT_FILE "fresnelData.txt"

// Global Variables //
float distToTargetKM;
float freqGHz;
float tempf;

// Functions //
float calcFresnelRadius( float freqG, float distKM ) {
	float result = (17.31*sqrt(distKM/(4*freqG)))/2;
	printf("%.4f\n", result);
	tempf = result;
	return result;
}

char * floatToStringFC(float f){
	char buffer[DATA_SIZE];
    	return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileFC(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringFC(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileFC(){
  FILE * fPtr = fopen(TXT_FILE, "w");
  fclose(fPtr);
}

int getUserInputFC(){
	int exitFlag = 0;
	float dkm, fg;
	int var;
	printf("Please choose from the following: \n");
	printf("1 for fresnel zone radius (m)\n");
	printf("6 to clear all data from fresnelData.txt\n");
	printf("7 for store to file\n");
	printf("8 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			printf("Please enter distance in km: \n");
			scanf(" %f", &dkm);
			printf("Please enter frequency in GHz: \n");
			scanf(" %f", &fg);
			calcFresnelRadius(fg, dkm);
			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:
			clearDataFromFileFC();
			printf("fresnelData.txt is now cleared of all data");
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileFC(tempf);
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
	getUserInputFC();
}

// int main(int argc, char const *argv[]) {
//   calcFresnelRadius( 24, 50 ) ;
//   getUserInputFC();
//   return 0;
// }
