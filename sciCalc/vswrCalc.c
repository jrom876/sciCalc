//	Package:	sciCalc
//	File:		vswrCalc.c
//	Purpose:	Calculate VSWR using voltage, power or gamma
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
