
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vswrCalc.h"
#define DATA_SIZE 1000
#define TXT_FILE "vswrData.txt"

// Global Variables //
float swrV;
float swrG;
float swrP;
float tempf;

// Functions //
float calcVSWRVoltage( float vf, float vr ) {
	swrV = ((vf+vr)/(vf-vr));
	printf("%.4f\n", swrV);
	tempf = swrV;
	return swrV;
}

float calcVSWRGamma( float g ) {
	float gm = fabs(g);
	swrG = ((1+gm)/(1-gm));
	printf("%.4f\n", swrG);
	tempf = swrG;
	return swrG;
}

float calcVSWRPwr( float pf, float pr ) {
	float sqrtPwr;
	sqrtPwr = sqrt(pr/pf);
	swrP = ((1+sqrtPwr)/(1-sqrtPwr));
	printf("%.4f\n", swrP);
	tempf = swrP;
	return swrP;
}

char * floatToStringVSWR(float f){
		char buffer[DATA_SIZE];
    return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileVSWR(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringVSWR(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  fclose(fPtr);
}

void clearDataFromFileVSWR(){
  FILE * fPtr = fopen(TXT_FILE, "w");
  fclose(fPtr);
}

int getUserInputVSWR(){
	int exitFlag = 0;
	float vfwd, vref, gamma, pfwd, pref;
	int var;
	printf("Please choose from the following: \n");
	printf("1 for voltage\n");
	printf("2 for gamma\n");
	printf("3 for power\n");
	printf("6 for clear\n");
	printf("7 for storeDataInFileAG\n");
	printf("8 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			printf("Please enter forward voltage: \n");
			scanf(" %f", &vfwd);
			printf("Please enter reflected voltage: \n");
			scanf("%f", &vref);
			calcVSWRVoltage(vfwd, vref);
			break;
		case 2:
			printf("Please enter gamma: \n");
			scanf("%f", &gamma);
			calcVSWRGamma(gamma);
			break;
		case 3:
			printf("Please enter forward power: \n");
			scanf("%f", &pfwd);
			printf("Please enter reflected power: \n");
			scanf("%f", &pref);
			calcVSWRPwr(pfwd, pref);
			break;
		case 6:
			clearDataFromFileVSWR();
			printf("vswrData.txt is now cleared of all data");
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileVSWR(tempf);
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
	//if(0 != exitFlag) exit(0);
	if(0 != exitFlag) return 0;
	getUserInputVSWR();
}
//
////////////////////////
/// Main ///
// int main(int argc, char *argv[]){
// 	getUserInputVSWR();
// 	return 0;
// }
