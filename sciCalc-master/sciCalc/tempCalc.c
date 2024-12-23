//	Package:	sciCalc
//	File:		tempCalc.c
//	Purpose:	Convert Degrees Celsius <==> Fahrenheit
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
#include "tempCalc.h"
#define DATA_SIZE 1000
#define TXT_FILE "tempData.txt"

float tempf;
//int exitFlag = 0;

float ctof(float c){
  float result = (c*(1.8))+32;
  printf("Conversion: %f C = %f F\n",c,result);
  tempf = result;
  return result;
}
//
float ctok(float c){
  float result = c+273;
  printf("Conversion: %f C = %f K\n",c,result);
  tempf = result;
  return result;
}

float ftoc(float f){
  float result = (f-32)/1.8;
  printf("Conversion: %f C = %f F\n",f,result);
  tempf = result;
  return result;
}

float ftok(float f){
  float result = ftoc(f) + 273;
  printf("Conversion: %f F = %f K\n",f,result);
  tempf = result;
  return result;
}

float ktoc(float k){

}
float ktof(float k){

}

char * floatToStringTC(float f){
	char buffer[DATA_SIZE];
    	return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileTC(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringTC(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileTC(){
  FILE * fPtr = fopen(TXT_FILE, "w");
  fclose(fPtr);
}

int getUserInputTC(){
	int exitFlag = 0;
	float c, f, k;
	int var;
	printf("Please choose from the following: \n");
	printf("1 for ctof\n");
	printf("2 for ctok\n");
	printf("3 for ftoc\n");
	printf("4 for ftok\n");
	printf("5 for ktoc\n");
	printf("6 for ktof\n");
	printf("7 to clear all data from tempData.txt\n");
	printf("8 for store to file\n");
	printf("9 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			printf("Please enter C temp: \n");
			scanf(" %f", &c);
			ctof(c);
			break;
		case 2:
			printf("Please enter C temp: \n");
			scanf(" %f", &c);
			ctok(c);
			break;
		case 3:
			printf("Please enter F temp: \n");
			scanf(" %f", &c);
			ftoc(c);
			break;
		case 4:
			printf("Please enter F temp: \n");
			scanf(" %f", &c);
			ftok(c);
			break;
		case 5:
			printf("Please enter K temp: \n");
			scanf(" %f", &c);
			ktoc(c);
			break;
		case 6:
			printf("Please enter K temp: \n");
			scanf(" %f", &c);
			ktof(c);
			break;
    		case 7:
			clearDataFromFileTC();
      			printf("tempData.txt is now cleared of all data");
			break;
		case 8:
			printf("Writing data to file\n");
			storeDataInFileTC(tempf);
			break;
		case 9:
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exitFlag = 1;
			break;
	}
	if(0 != exitFlag) return 0;
	getUserInputTC();
}
//
// int main(int argc, char const *argv[]) {
//   ctof(85);
//   ctok(85);
//   ftoc(185);
//   ftok(185);
//   getUserInputTC();
//   return 0;
// }
