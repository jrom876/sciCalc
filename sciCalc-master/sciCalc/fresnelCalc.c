//	Package:	sciCalc
//	File:		fresnelCalc.c 
//	Purpose:	Calculate Fresnel Zone diameter and radius
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
#include "fresnelCalc.h"
#define DATA_SIZE 1000
#define TXT_FILE "fresnelData.txt"

// Global Variables //
float distToTargetKM;
float freqGHz;
float tempf;

// Functions //
float calcFresnelDiameter( float freqG, float distKM ) {
	float result = (17.31*sqrt(distKM/(4*freqG)));
	printf("\nFresnel Diameter = %.4f meters\n\n", result);
	tempf = result;
	return result;
}

float calcFresnelRadius( float freqG, float distKM ) {
	float result = (8.656*sqrt(distKM/(4*freqG)));
	printf("\nFresnel Radius = %.4f meters\n\n", result);
	tempf = result;
	return result;
}

//~ float calcFresnelInterference( float freqG, float distRKM, float distDKM ) {
	//~ float result = (8.656*sqrt(distKM/(4*freqG)));
	//~ printf("\nFresnel Radius = %.4f meters\n\n", result);
	//~ tempf = result;
	//~ return result;
//~ }

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
	printf("1 for fresnel zone diameter (m)\n");
	printf("2 for fresnel zone radius (m)\n");
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
			calcFresnelDiameter(fg, dkm);
			break;
		case 2:
			printf("Please enter distance in km: \n");
			scanf(" %f", &dkm);
			printf("Please enter frequency in GHz: \n");
			scanf(" %f", &fg);
			calcFresnelRadius(fg, dkm);
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
			exit(0);
			exitFlag = 1;
			break;
	}
	if(0 != exitFlag) return 0;
	getUserInputFC();
}

//~ int main(int argc, char const *argv[]) {
  //~ calcFresnelRadius( 24, 50 ) ;
  //~ getUserInputFC();
   //~ return 0;
//~ }
