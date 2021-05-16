//	Package:	sciCalc
//	File:		inrushCalc.c 
//	Purpose:	Calculate Power Supply Inrush Current
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

// https://e2e.ti.com/blogs_/b/powerhouse/archive/2015/03/31/
// powertips-how-to-limit-inrush-current-in-an-ac-dc-power-supply
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inrushICalc.h"
#define DATA_SIZE 1000
#define PI 3.14159265358979323846
#define TXT_FILE "inrushIData.txt"

// Global Variables //
float inrushMax=0, ssi=0, minRes=0, iout=0, vout=0; // ssI == steady state current
float xc=0, xi=0; // capacitive and inductive reactances
float vac=0, netCap=0, zSystem=0;
float eTotal; // Total Energy of cap bank
float irc, pki; // inrush current and peak current
float freqHz=0, freqGHz=0;
float tempf;

////////// Functions ///////////
////////////////////////////////
///// Set required values //////
void setValuesIC(void){
  	printf("Enter input AC Voltage:\n");
	scanf(" %f", &vac);
  	printf("Enter input frequency:\n");
	scanf(" %f", &freqHz);
  	printf("Enter output current:\n");
	scanf(" %f", &iout);
  	printf("Enter output voltage:\n");
	scanf(" %f", &vout);
  	printf("Enter net capacitance:\n");
	scanf(" %f", &netCap);
  	printf("Enter system impedance:\n");
	scanf(" %f", &zSystem);
}
//////////////////////////////////////
/// Calculate Capacitive Reactance ///
float calcXC(){
  	xc = (1/(2*PI*freqHz*netCap));
	printf("Net Capacitive Reactance = %.4f Ohms\n", xc);
	tempf = xc;
	return xc;
}

// float calcXC(float c, float f){
//   xc = (1/(2*PI*f*c));
// 	printf("Capacitive Reactance = %.4f\n", xc);
// 	tempf = xc;
// 	return xc;
// }
////////////////////////////////
/// Calculate Minimum Resistance
float calcMinRes() {
	minRes = ((1.414*vout)/iout);
	printf("Minimum Allowable Circuit Resistance = %.4f Ohms\n", minRes);
	tempf = minRes;
	return minRes;
}

// float calcMinRes( float v, float i ) {
// 	minRes = ((1.414*v)/i);
// 	printf("Minimum Resistance = %.4f\n", minRes);
// 	tempf = minRes;
// 	return minRes;
// }
///////////////////////////////////////
//////// Calculate Max Inrush Current ////////
float calcMaxI() {
	inrushMax = ((1.414*vout)/zSystem) + ssi;
	printf("Maximum Inrush Current = %.4f Amps\n", inrushMax);
	tempf = inrushMax;
	return inrushMax;
}

// float calcMaxI( float v, float r ) {
// 	inrushMax = ((1.414*v)/r);
// 	printf("Maximum Current = %.4f\n", inrushMax);
// 	tempf = inrushMax;
// 	return inrushMax;
// }

//////////////////////////////////////////
///// Calculate Steady State Current /////
float calcSSI(){
	float pout = vout*iout;
  	//ssi = (0.9*(pout/vac));
  	setSSI();
  	printf("Steady State System Current = %.4f Amps\n", ssi);
	//printf("%.4f\n", ssi);
	tempf = ssi;
	return ssi;
}

float setSSI(){
	float pout = vout*iout;
  	ssi = (0.9*(pout/vac));
	tempf = ssi;
	return ssi;
}
////////////////////////////////////
char * floatToStringIC(float f){
	char buffer[DATA_SIZE];
    	return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileIC(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringIC(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileIC(){
  FILE * fPtr = fopen(TXT_FILE, "w");
  fclose(fPtr);
}

int getUserInputIC(){
	int exitFlag = 0;
	float dkm, fg;
	float v, i;
	int var;
  // printf("Setting required values\n");
  // setValuesIC();
	printf("Please choose from the following: \n");
	printf("1 to calculate minimum resistance\n");
	printf("2 to calculate steady state current\n");
	printf("3 to calculate maximum surge current\n");
	printf("4 to calculate system capacitive reactance\n");
	printf("5 to reset parameters\n");
	printf("6 to clear data from inrushIData.txt\n");
	printf("7 for store to file\n");
	printf("8 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			calcMinRes();
			break;
		case 2:
      			calcSSI();
			break;
		case 3:
      			calcMaxI();
			break;
		case 4:
      			calcXC();
			break;
		case 5:
      			setValuesIC();
			break;
		case 6:
			clearDataFromFileIC();
			printf("inrushIData.txt is now cleared of all data\n");
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileIC(tempf);
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
	getUserInputIC();
}

// int main(int argc, char const *argv[]) {
//   printf("Setting required values\n");
//   setValuesIC();
//   getUserInputIC();
//   return 0;
// }

// float calcSSI(float i, float v, float v_ac){
// 	float pout = v*i;
//   	ssi = (0.9*(pout/v_ac));
// 	printf("Steady State Current = %.4f Amps\n", ssi);
// 	tempf = ssi;
// 	return ssi;
// }
/// Calculate Total Energy of Capacitor Bank ///
// float calcETotal(float c, float v){
//   	eTotal = (c*(v*v))/2;
// 	printf("%.4f\n", eTotal);
// 	tempf = eTotal;
// 	return eTotal;
// }
//////////////////////////////////////////
//https://electronics.stackexchange.com/questions/272000/
// inrush-current-calculation-in-capacitive-circuite
// float calcIRC(float vac, float c2){
//   	irc = 2*PI*vac*c2;
// 	printf("%.4f\n", irc);
// 	tempf = irc;
// 	return irc;
// }
//
// float calcPeakI(float Vripple, float tcharge, float c2){
//   	pki = 1.5*(Vripple/tcharge)*c2;
// 	printf("%.4f\n", pki);
// 	tempf = pki;
// 	return pki;
// }

/////////////////////////////////////
// IR Current for a capacitor bank //
// float findirc(float vac, float vfwd, float esr){
//   	float irp = (((1.414*vac)-vfwd)/esr);
// 	printf("%.4f\n", irp);
// 	tempf = irp;
// 	return irp;
// }
