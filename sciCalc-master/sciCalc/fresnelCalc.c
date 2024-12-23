//	Package:	sciCalc
//	File:		fresnelCalc.c 
//	Purpose:	Calculate Fresnel Zone diameter and radius
//	Author:		jrom876

/**
	Copyright (C) 2019, 2021 
	Jacob Romero, Creative Engineering Solutions, LLC
	cesllc876@gmail.com

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

/// STANDARD DEFINITIONS FOR PROJECT SCICALC 
#define PI	3.14159265358979323846 	// ad infinitum sine repeto
#define LIGHT_SPEED		299792458.0 	// meters per second
#define STACK_OVERFLOW	2147483648		// Hex 0x80000000
#define DATA_SIZE 1000
#define DELTA 1.0e-6
#define MILLI 1.0e-3
#define MICRO 1.0e-6
#define NANO 1.0e-9
#define PICO 1.0e-12
#define FEMTO 1.0e-15
#define ATTO 1.0e-18
#define KILO 1.0e3
#define MEGA 1.0e6
#define GIGA 1.0e9
#define TERA 1.0e12 
#define true 1
#define false 0

/// STANDARD DEFINITIONS FOR LIGHT INTENSITY AND ELECTRIC FIELD CALCULATIONS
#define AIR_REFRACTIVE_INDEX 1.00027717
#define E0 8.8541878128*PICO				// Permittivity of Free Space in Farads per meter
#define MU0 1.25663706212*MICRO				// Permeability of Free Space in Newtons per square meter
#define EPSILON_0 1/(MU0*(LIGHT_SPEED**2))	// Permittivity of Free Space Equation
#define E_CONSTANT 1/(4*PI*EPSILON_0)
#define ELECTRON_CHARGE 1.6e-19 			// Charge of an electron in Coulombs
#define RADIUS_HELIUM_ATOM 26.5e-12			// Radius of a Helium atom in meters
#define LED_ARRAY_RADIUS 0.35 				// meters from LED array to sample plate
#define ETA_T (eta * zmismatch)
#define LAMBDA (LIGHT_SPEED/frequency)
#define FREQUENCY (LIGHT_SPEED/lambda)

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
