//	Package:	sciCalc
//	File:		ustripCalc.c
//	Purpose:	Calculate PCB Microstrip Impedance using mils or mm
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
#include "ustripZCalc.h"
#define DATA_SIZE 1000
#define TXT_FILE "ustripZData.txt"

/// Globals ///
float hmils, wmils, tmils;
float hmm, wmm, tmm;
float tempf;
//int exitFlag = 0;
//
/// Functions ///
float convertMilsToMM(float mil){
  float result = ((mil*25.4)*0.001);
  printf("Conversion: \t%.5f mils = %.5f mm\n",mil, result);
  tempf = result;
  return result;
}

float convertMMToMils(float mm){
  float result = ((mm*39.37));
  printf("Conversion: \t%.5f mm = %.5f mils\n",mm, result);
  tempf = result;
  return result;
}

float calcZoutMils(float h, float w, float t, float er){
  float result = ((87/(sqrt(er+1.41))) * log(((5.98*h)/((0.8*w)+t))));
  printf("%.4f Ohms\n", result);
  tempf = result;
  return result;
}

float calcZoutMM(float hm, float wm, float tm, float er){
  float result = ((87/(sqrt(er+1.41))) * log(((5.98*hm*0.03937)/((0.8*wm*0.03937)+(tm*0.03937)))));
  printf("%.4f Ohms\n", result);
  tempf = result;
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
			exitFlag = 1;
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
