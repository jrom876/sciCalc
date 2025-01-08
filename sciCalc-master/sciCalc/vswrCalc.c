//	Package:	sciCalc
//	File:		vswrCalc.c
//	Purpose:	Calculate VSWR using voltage, power or gamma
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
#include "vswrCalc.h"

/// STANDARD DEFINITIONS FOR PROJECT SCICALC 
#define PI		3.14159265358979323846 	// ad infinitum sine repeto
#define LIGHT_SPEED	299792458.0 		// meters per second
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
#define E0 8.8541878128*PICO			// Permittivity of Free Space in Farads per meter
#define MU0 1.25663706212*MICRO			// Permeability of Free Space in Newtons per square meter
#define EPSILON_0 1/(MU0*(LIGHT_SPEED**2))	// Permittivity of Free Space Equation
#define E_CONSTANT 1/(4*PI*EPSILON_0)
#define ELECTRON_CHARGE 1.6e-19 		// Charge of an electron in Coulombs
#define RADIUS_HELIUM_ATOM 26.5e-12		// Radius of a Helium atom in meters
#define LED_ARRAY_RADIUS 0.35 			// meters from LED array to sample plate
#define ETA_T (eta * zmismatch)
#define LAMBDA (LIGHT_SPEED/frequency)
#define FREQUENCY (LIGHT_SPEED/lambda)

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
