
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

// References:
// https://e2e.ti.com/blogs_/b/powerhouse/archive/2015/03/31/powertips-how-to-limit-inrush-current-in-an-ac-dc-power-supply
// https://www.ametherm.com/blog/inrush-current/calculate-inrush-current-three-steps

// For switching large capacitor banks see:
// https://www.eaton.com/content/dam/eaton/products/medium-voltage-power-distribution-control-systems/power-capacitors/calculation-of-inrush-currents-in-single-and-multi-step-capacitor-bank-installations-td230004en.pdf

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inrushICalc.h"
#define DATA_SIZE 1000
#define EFFICIENCY 0.9
#define PI 3.14159265358979323846
#define TXT_FILE "inrushIData.txt"

/**
 * Steps for evaluating Inrush Current:
 *	1) 	Calculate Minimum Resistance, Rmin
 * 			Rmin = sqrt(2) * (Vac/I) = 1.414 * Vac/I
 * 	2) 	Calculate steady-state current, Iss
 * 			Iss = (Pout/Vac) * Efficiency(assume 90%) = 0.9*(Pout/Vac))
 * 	3) 	Estimate the peak inrush current under a no-load condition
 * 			Iip = ((1.414*Vac) - Vf) / Resr
 *  	4) 	Calculate total energy, Etot
 * 			Etot = (C*V^2) / 2
 *  	5) 	Design current limiter with these parameters
 * 				
 **/

struct ICL {
	float rmin;
	float iss;
	float imax;
	float etot;
};

// Global Variables //
float inrushMax=0, ssi=0, minRes=0, iout=0, vout=0; // ssI == steady state current
float xc=0, xi=0; // capacitive and inductive reactances
float vac=0, netCap=0, zSystem=0.000001;
float eTotal; // total Energy of cap bank
float resr=0.000001; // equivalent series resistance of capacitor(s) in bank
float irc, pki; // inrush current and peak current
float freqHz=0, freqGHz=0;

////////// Functions ///////////
////////////////////////////////

void printICL(struct ICL f1) {
	//~ printf("\nprintICL\n");
	printf("\n");
	printf("rmin\t\t%.4f Ohms\n",f1.rmin);
	printf("iss\t\t%.4f A\n",f1.iss);
	printf("imax\t\t%.4f A\n",f1.imax);
	printf("etot\t\t%.4f J\n\n",f1.etot);
}

///// Set required values //////
struct ICL setValuesIC(float va, float fr, float io, float vo, float nc, float zs, float er){
	struct ICL result;
  	//~ printf("Enter input AC Voltage:\n");
	//~ scanf(" %f", &vac);
  	//~ printf("Enter input frequency:\n");
	//~ scanf(" %f", &freqHz);
  	//~ printf("Enter output current:\n");
	//~ scanf(" %f", &iout);
  	//~ printf("Enter output voltage:\n");
	//~ scanf(" %f", &vout);
  	//~ printf("Enter net capacitance:\n");
	//~ scanf(" %f", &netCap);
  	//~ printf("Enter system impedance:\n");
	//~ scanf(" %f", &zSystem);
  	//~ printf("Enter ESR of capacitors:\n");
	//~ scanf(" %f", &resr);
	vac=va; freqHz=fr; iout=io; vout=vo; netCap=nc; zSystem=zs; resr=er;	
	result.rmin = calcMinRes();
	result.iss = calcSSI();
	result.imax = calcMaxI_noLoad();
	result.etot = calcTotEnergy();
	printICL(result);
	return result;
}
/// Calculate Minimum Resistance
float calcMinRes(void){
	minRes = ((sqrt(2)*vout)/iout);
	//~ printf("Minimum Allowable Circuit Resistance = %.4f Ohms\n", minRes);
	return minRes;
}

///////////////////////////////////////
//////// Calculate Max Inrush Current ////////
float calcMaxI(void){
	inrushMax = ((sqrt(2)*vout)/zSystem) + ssi;
	//~ printf("Maximum Inrush Current = %.4f Amps\n", inrushMax);
	return inrushMax;
}

float calcMaxI_noLoad(void){
	inrushMax = (((1.414*vac)-0.7)/resr);
	//~ printf("Maximum Inrush Current = %.4f\n", inrushMax);
	return inrushMax;
}

//////////////////////////////////////////
///// Calculate Steady State Current /////
float calcSSI(void){
	float pout = vout*iout;
  	ssi = (0.9*(pout/vac));
  	//~ printf("Steady State System Current = %.4f Amps\n", ssi);
	return ssi;
}

float setSSI(void){
	float pout = vout*iout;
  	ssi = (0.9*(pout/vac));
	return ssi;
}

float calcTotEnergy(void){
	float result = (netCap*(pow(vout,2)))/2;
	//~ printf("Total Energy in Caps = %.4f Joules\n",result);
	return result;
}

struct ICL getIC_specs(void){
	struct ICL result;
	printf("Getting inrush current specs\n");
	//~ setValuesIC();
	result.rmin = calcMinRes();
	result.iss = calcSSI();
	result.imax = calcMaxI_noLoad();
	result.etot = calcTotEnergy();
	return result;
}
//////////////////////////////////////
/// Calculate Capacitive Reactance ///
float calcXC(void){
  	xc = (1/(2*PI*freqHz*netCap));
	printf("Net Capacitive Reactance = %.4f Ohms\n", xc);
	return xc;
}

// float calcXC(float c, float f){
//   xc = (1/(2*PI*f*c));
// 	printf("Capacitive Reactance = %.4f\n", xc);
// 	return xc;
// }
////////////////////////////////////
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
	//setValuesIC();
	printf("Please choose from the following: \n");
	printf("1 to calculate minimum resistance\n");
	printf("2 to calculate steady state current\n");
	printf("3 to calculate maximum surge current\n");
	printf("4 to calculate system capacitive reactance\n");
	//~ printf("5 to reset parameters\n");
	printf("6 to get complete IC limiter specs\n");
	printf("7 to clear data from inrushIData.txt\n");
	printf("8 for store to file\n");
	printf("9 for quit\n");
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
      		//~ setValuesIC();
			break;
		case 6:
			//~ setValuesIC();
      			getIC_specs();
			break;
		case 7:
			clearDataFromFileIC();
			printf("inrushIData.txt is now cleared of all data\n");
			break;
		case 8:
			printf("Writing data to file\n");
			//~ storeDataInFileIC(tempf);
			break;
		case 9:
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exit(0);
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
