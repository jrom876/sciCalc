//	Package:	sciCalc
//	File:		antennaGainCalc.c 
//	Purpose:	Calculate Antenna Gain and convert freq <==> wavelength
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

#include <math.h>
#include <check.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "antennaGainCalc.h"
#define PI            3.14159265358979323846
#define LIGHT_SPEED   299792458 // meters per second
#define DATA_SIZE     1000
#define TXT_FILE "antennaGainData.txt"

#define ETA_T (eta * zmismatch)
#define LAMBDA (LIGHT_SPEED/frequency)

// References:	https://www.antenna-theory.com/basics/main.php
//				https://www.antenna-theory.com/basics/gain.php
// Antenna Efficiency = Pwr_radiated / Pwr_received

// BN: Lambda always means wavelength(m), and eta
// always means % efficiency in this program

float eta       = 0; // antenna efficiency - %
float lambda    = 0; // wavelength - meters
float aperature = 0; // antenna aperature - meters^2
float frequency = 0; // frequency - hertz
float gain      = 0; // gain = power radiated / power received
float zmismatch	= 0; // impedance mismatch between tx line and antenna
float tempf;
//int exitFlag = 0;


float calcAntGainFromFreq(float et, float ap, float fr){
  float lam = calcLambda(fr);
  float result = ((10*log10((et*4*PI*ap)/(lam*lam))));
  printf("Parabolc Antenna gain = %f\n\n",result);
  eta = et; aperature = ap; frequency = fr; lambda = lam;
  gain = result; tempf = result;
  return result;
}

float calcAntGainFromLambda(float et, float ap, float lam){
  float fr = calcFreq(lam);
  float result = ((10*log10((et*4*PI*ap)/(lam*lam))));
  printf("Parabolc Antenna gain = %.4f\n\n",result);
  eta = et; aperature = ap; frequency = fr; lambda = lam;
  gain = result; tempf = result;
  return result;
}
// use freq in MHz
float calcLambda(float freq){
  float clambda = LIGHT_SPEED/(freq*1000000);
  printf("\nwavelength = %.6f \n",clambda);
  tempf = clambda;
  return clambda;
}
// use lambda in meters 
float calcFreq(float lam){
  float freq = (LIGHT_SPEED/lam)/1000000;
  printf("\nfrequency = %.4f MHz\n",freq);
  tempf = freq;
  return freq;
}

void printLambda(){
  //lambda = LIGHT_SPEED/frequency;
  printf("wavelength = %.6f mm\n",LAMBDA*1000);
}

void printFreq(){
  //frequency = LIGHT_SPEED/lambda;
  printf("frequency = %.4f MHz\n",frequency/1000000);
}

////////////////////////////////////////////
// The following 3 functions create a .txt
// file if it does not already exist; convert an
// input float to a string and append the .txt
// file with that string on a new line; and
// clear all data from the .txt file if requested.
////////////////////////////////////////////
char * floatToStringAG(float f){
	    char buffer[DATA_SIZE];
    	return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileAG(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringAG(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileAG(){
  FILE * fPtr = fopen(TXT_FILE, "w");
  fclose(fPtr);
}
//////////////////////////////////////
//////////////////////////////////////
int getUserInputAG(){
	int exitFlag = 0;
	float e, a, f, l;
	int var;
	printf("Please choose from the following: \n");
	printf("1 for calcAntGainFromFreq(freq)\n");
	printf("2 for calcAntGainFromLambda(lam)\n");
	printf("3 for calcLambda(freq)\n");
	printf("4 for calcFreq(lam)\n");
	// printf("3 for ftoc\n");
	// printf("4 for ftok\n");
	// printf("5 for ktoc\n");
	printf("6 for clear data from file\n");
	printf("7 for store to file\n");
	printf("8 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			printf("Calculating Antenna Gain using frequency\n");
		    printf("Please enter efficiency: \n");
			scanf(" %f", &e);
		    printf("Please enter aperature area in m^2: \n");
		    scanf(" %f", &a);
		    printf("Please enter frequency in MHz: \n");
		    scanf(" %f", &f);
			calcAntGainFromFreq(e, a, f);
			break;
		case 2:
			printf("Calculating Antenna Gain using wavelength\n");
		    printf("Please enter efficiency: \n");
			scanf(" %f", &e);
		    printf("Please enter aperature area in m^2: \n");
		    scanf(" %f", &a);
		    printf("Please enter wavelength in meters: \n");
		    scanf(" %f", &l);
			calcAntGainFromLambda(e, a, l);
			break;
		case 3:
			printf("Calculating Lambda\n");
			printf("Please enter frequency: \n");
			scanf(" %f", &f);
			calcLambda(f);
			break;
		case 4:
			printf("Calculating Frequency\n");
			printf("Please enter wavelength: \n");
			scanf(" %f", &l);
			calcFreq(l);
			break;
		case 5:
			break;
		case 6:
			clearDataFromFileAG();
      		printf("antennaGainData.txt is now cleared of all data");
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileAG(tempf);
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
	getUserInputAG();
}

//
//~ int main(int argc, char const *argv[]) {
	//~ eta = 0.9;
	//~ zmismatch = 0.9;
	//~ frequency = 300000000;
	//~ printFreq();
	//~ printLambda();
	//~ printf("ETA_T: \t\t%.5f\n",ETA_T);
	//~ printf("LAMBDA: \t%.5f\n",LAMBDA);
	//~ calcAntGainFromFreq(0.92, 100, 25000000);
	//~ printLambda(2400000000);
	//~ getUserInputAG();
	//~ return 0;
//~ }
