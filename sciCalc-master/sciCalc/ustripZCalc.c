//	Package:	sciCalc
//	File:		ustripCalc.c
//	Purpose:	Calculate PCB Microstrip Impedance using mils or mm
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

// References:
// https://www.microwaves101.com/encyclopedias/microstrip
// https://www.rfwireless-world.com/calculators/Embedded-Microstrip-Impedance-Calculator.html
// https://chemandy.com/calculators/microstrip-transmission-line-calculator-hartley27.htm

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ustripZCalc.h"

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

#define TXT_FILE "ustripZData.txt"

/// Globals ///
float hmils, wmils, tmils;
float hmm, wmm, tmm;
float eglobal, wglobal, thglobal, hglobal;
float zeff = 0.00001, eeff = 0.00001;
float zo = 0.00001;
float tempf = 0;
//int exitFlag = 0;
// (1 oz/ft2 = 0.035 mm)


/// Functions ///
float convertMilsToMM(float mil){
  float result = ((mil*25.4)*0.001);
  printf("Conversion: \t%.5f mils = %.5f mm\n",mil, result); // DBPRINT
  return result;
}

float convertMMToMils(float mm){
  float result = ((mm*39.37));
  printf("Conversion: \t%.5f mm = %.5f mils\n",mm, result); // DBPRINT
  return result;
}

//********************************************************************//
/// Embedded Microstrip Impedance 
// https://chemandy.com/calculators/microstrip-transmission-line-calculator-hartley27.htm

float calcZEff(float er, float w, float h){
	float result;
	if((w/h)<1){
		zeff = ((er+1.0)/2) + (((er-1.0)/2) * ((1/sqrt(1+((12*h)/w)))+((0.04)*pow((1-(w/h)),2)))); 
	}
	else if ((w/h)>=1){
		result = ((er+1.0)/2) + (((er-1.0)/2) * ((1/sqrt(1+((12*h)/w)))));
	}	
	zeff = result;
	return result;
}

float zoutMM(float w, float h){
	float result;
	if((w/h)<1){
		result = (60*pow(zeff,-0.5)) * log(((8*h)/w)+(w/(4*h)));
	}
	else if ((w/h)>=1){
		result = ((120*PI)*pow(zeff,-0.5)) * 1/((w/h)+1.393+(0.677*log((w/h)+1.444)));
	}
	return result;
}

void printUS(struct USZ f1){
	//~ printf("\nprintUS\n");
	//~ printf("\n");
	//~ printf("\nEmbedded Microstrip Impedance Calculator");
	printf("\nEffective Dielectric Constant\t\t%.4f\n",f1.Eeff);
	printf("Characteristic System Impedance\t\t%.4f Ohms\n",f1.Zout);
}

///// Set required values //////
struct USZ setValuesUS(float er, float width, float height){
	struct USZ result;
	printf("\nSet Values");
	eglobal=er; wglobal=width; hglobal=height;	
	result.Eeff = calcZEff(er,width,height);
	result.Zout = zoutMM(width,height);
	printUS(result);
	return result;
}
//********************************************************************//
/// Transmission Strip Impedance 
// https://ncalculators.com/electronics/microstrip-impedance-calculator.htm

struct USZ putValuesUS(float er, float w, float th, float h){
	struct USZ result;
	result.Eeff = calcZEff(er, w, h);
	result.Zout = calcZoutMM(er, w, th, h);
	printf("\nPut Values\n"); // DBPRINT
	printf("Eeff\t%.4f\nZout\t%.4f\n",result.Eeff,result.Zout); // DBPRINT
	return result;
}

float calcZoutMils(float er,float w, float t, float h){
  float result = ((87/(sqrt(er+1.41))) * log(((5.98*h)/((0.8*w)+t))));
  //~ printf("Zout\t%.4f Ohms\n", result); // DBPRINT
  return result;
}

float calcZoutMM(float er,float wm, float tm, float hm){
  float result = ((87/(sqrt(er+1.41))) * log(((5.98*hm*0.03937)/((0.8*wm*0.03937)+(tm*0.03937)))));
  //~ printf("Zout\t%.4f Ohms\n", result); // DBPRINT
  return result;
}
////////////////////////////////
////////////////////////////////
char * floatToStringUS(float f){
	char buffer[DATA_SIZE];
    	return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileUS(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringUS(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileUS(){
  FILE * fPtr = fopen(TXT_FILE, "w");
  fclose(fPtr);
}

int getUserInputUS(){
	int exitFlag = 0;
	float h, w, t, er, mm, mils;
	int var;
	printf("Please choose from the following: \n");
	printf("1 to calculate output impedance(mils), Z \n");
	printf("2 to calculate output impedance(mm), Z \n");
	printf("3 to convert mils to millimeters \n");
	printf("4 to convert millieters to mils \n");
	printf("5 to clear all data from ustripZData.txt\n");
	printf("6 for store to file\n");
	printf("7 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			printf("Please enter height in mils: \n");
			scanf(" %f", &hmils);
			printf("Please enter width in mils: \n");
			scanf(" %f", &wmils);
			printf("Please enter thickness in mils: \n");
			scanf(" %f", &tmils);
			printf("Please enter dielectric constant: \n");
			scanf(" %f", &er);
			calcZoutMils(hmils,wmils,tmils,er);
      			break;
		case 2:
			printf("Please enter height in mm: \n");
			scanf(" %f", &hmm);
			printf("Please enter width in mm: \n");
			scanf(" %f", &wmm);
			printf("Please enter thickness in mm: \n");
			scanf(" %f", &tmm);
			printf("Please enter dielectric constant: \n");
			scanf(" %f", &er);
			calcZoutMM(hmm,wmm,tmm,er);
			break;
		case 3:
			printf("Please enter value in mils: \n");
			scanf(" %f", &mils);
			convertMilsToMM(mils);
			break;
		case 4:
			printf("Please enter value in mm: \n");
			scanf(" %f", &mm);
			convertMMToMils(mm);
			break;
	    	case 5:
	      		clearDataFromFileUS();
	      		printf("ustripZData.txt is now cleared of all data\n");
	      		break;
		case 6:
			printf("Writing data to file\n");
			storeDataInFileUS(tempf);
			break;
		case 7:
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exit(0);
			break;
	}
	if(0 != exitFlag) return 0;
	getUserInputUS();
}
////////////////////////
/// Main ///
// int main(int argc, char const *argv[]) {
//   calcZoutMils(20,40,1.378,3.66);
//   calcZoutMM(0.508,1.016,0.035,3.66);
//   convertMilsToMM(20);
//   convertMMToMils(0.43);
//   getUserInputUS();
//   return 0;
// }
