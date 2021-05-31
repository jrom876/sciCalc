//	Package:	sciCalc
//	File:		linkBudgetCalc.c
// 	Purpose:	Calculate Link Budget (One-way and Two-way path loss)
// 	Author:		jrom876

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

// https://www.random-science-tools.com/electronics/friis.htm
// https://www.antenna-theory.com/basics/friis.php
// https://rfcafe.com/references/electrical/ew-radar-handbook/two-way-radar-equation.htm

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <check.h>
#include "linkBudgetCalc.h"
#define PI	3.14159265358979323846 // ad infinitum
#define LIGHT_SPEED		299792458 // meters per second
#define DATA_SIZE 1000
#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000
#define TERA 1000000000000 
#define TXT_FILE_LB "linkBudgetData.txt"
//
// Globals //
float freqGHz, txpwrW, distTargetKM;
float radarXSect, antGain;
float plf = 0; // Polarization Loss Factor
float tempf;
//int exitFlag = 0;


// Functions //
/**
 * Two-way Path Loss, Monostatic Radar
 * **/

float twoWayPathLossDB(float freq, float dist, float rxs){
  float result = ((20*log(freq))+(40*log(dist))+163.43-(10*log(rxs)))*(-1);
  printf("Path Loss (dB): \t%.5f\n",result); // DBPRINT
  return result;
}

/** 
 * Received Signal at Target Equations, One-way Path Loss
**/

float rxPwrWatts(float Pt, float GtdB, float GrdB, float R, float freq){
	float result = Pt*pow(10,log10(GtdB))*pow(10,log10(GrdB))
		*(pow(LIGHT_SPEED,2))/(pow((4*PI*(R*KILO)*(freq*GIGA)),2));
	printf("RX Power at antenna: \t%.8f pico-watts\n",result*TERA); // DBPRINT
	return result;
}

float rxPwrDBM(float PtdBm, float GtdB, float GrdB, float R, float freq){	
	float result = pow(10,((PtdBm-30)/10))*pow(10,log10(GtdB))
		*pow(10,log10(GrdB))*(pow(LIGHT_SPEED,2))/(pow((4*PI*(R*KILO)*(freq*GIGA)),2));
	printf("RXdBm Power at antenna: \t%.8f pico-watts\n",result*TERA); // DBPRINT
	return result;
}

void printRxDBM( float PtdBm, float GtdB, float GrdB, float R, float freq){
	printf("\n%.4f\tPtdBm\n",PtdBm);
	printf("%.4f\tGtdb\n",GtdB);
	printf("%.4f\tGrdb\n",GrdB);
	printf("%.4f\tDistance km\n",R);
	printf("%.4f\tfreq GHz\n",freq);
	getFresnelRadius(freq,R);
}

float getFresnelRadius( float freqG, float distKM ) {
	float result = (8.656*sqrt(distKM/(4*freqG)));
	printf("\nFresnel Radius = %.4f meters\n\n", result); // DBPRINT
	return result;
}

// Polarization Loss Factor
float getPLF(float phi) {
	phi = phi*(PI/180);
	float plf = pow(cos(phi),2);
	printf("\nPolarization Loss Factor = %.4f\n\n", plf); // DBPRINT
	return plf;
}

float antGainFromFreq(float eff, float ap, float fr){
  float lam = getLambda(fr);
  float antgain = ((10*log10((eff*4*PI*ap)/(lam*lam))));
  printf("Antenna Gain = %f\n\n",antgain); // DBPRINT
  return antgain;
}

float getLambda(float freq){
  float clambda = (LIGHT_SPEED/freq);
  //~ printf("\nwavelength = %.6f meters\n",clambda); // DBPRINT
  return clambda;
}
// use lambda in meters 
float getFreq(float lam){
  float freq = (LIGHT_SPEED/lam);
  //~ printf("\nfrequency = %.4f MHz\n",freq/MEGA); // DBPRINT
  return freq;
}

////////////////////////////////
////////////////////////////////
char * floatToStringLB(float f){
	char buffer[DATA_SIZE];
    return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileLB(float input){
	FILE * fPtr = fopen(TXT_FILE_LB, "a");
	char * data = floatToStringLB(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileLB(){
  	FILE * fPtr = fopen(TXT_FILE_LB, "w");
  	fclose(fPtr);
}

int getUserInputLB(){
	float f, d, xs, txp, gtdb, grdb;
  	int exitFlag = 0;
	int var;
	printf("Please choose from the following: \n");
	printf("1 to calculate twoWayPathLossDB \n");
	printf("2 to calculate rxPwrWatts \n");
	printf("3 to calculate rxPwrDBM \n");
  	printf("6 to clear all data from linkBudgetData.txt\n");
	printf("7 for store to file\n");
	printf("8 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			printf("Please enter freq in Ghz: \n");
			scanf(" %f", &f);
			printf("Please enter distance in km: \n");
			scanf(" %f", &d);
			printf("Please enter radar cross section area in m^2: \n");
			scanf(" %f", &xs);
			twoWayPathLossDB(f,d,xs);
      			break;
		case 2:
			printf("Please enter freq in Ghz: \n");
			scanf(" %f", &f);
			printf("Please enter distance in km: \n");
			scanf(" %f", &d);
			printf("Please enter TX power in Watts: \n");
			scanf(" %f", &txp);
			printf("Please enter TX Antenna Gain in dB: \n");
			scanf(" %f", &gtdb);
			printf("Please enter RX Antenna Gain in dB: \n");
			scanf(" %f", &grdb);
			rxPwrWatts(txp,gtdb,grdb,d,f);

			break;
		case 3:
			printf("Please enter freq in Ghz: \n");
			scanf(" %f", &f);
			printf("Please enter distance in km: \n");
			scanf(" %f", &d);
			printf("Please enter TX power in dBm: \n");
			scanf(" %f", &txp);
			printf("Please enter TX Antenna Gain in dB: \n");
			scanf(" %f", &gtdb);
			printf("Please enter RX Antenna Gain in dB: \n");
			scanf(" %f", &grdb);
			rxPwrDBM(txp,gtdb,grdb,d,f);
			break;
		case 6:
			clearDataFromFileLB();
			printf("linkBudgetData.txt is now cleared of all data");
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileLB(tempf);
			break;
		case 8:
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exit(0);
			break;
	}
	if(0 != exitFlag) return 0;
	getUserInputLB();
}
////////////////////////
/// Main ///
// int main(int argc, char const *argv[]) {
//   twoWayPathLossDB(24, 50, 10);
//   getUserInputLB();
//   return 0;
// }


//////////////////////////
/////// TX ANTENNA ///////
//////////////////////////
//// TX Power Density ////
//// p = Pt/(4Pi*R^2) * Gt 
//// power density = TX power * Gain

//////////////////////////
/////// RX ANTENNA ///////
//////////////////////////

//// RX Power Density ////
//	Pr = Pt/(4Pi*R^2) * Gt * Aer 
//// Pr = TX power * Gain * Effective Aperature Area of RX antenna (m^2)

//// Effective Aperatue Area ////
// Aer = (lambda^2/4Pi) * Gr 
//// Aer = (c^2/(4Pi*freq^2)) * Gr

//// Friis Transmission Formulas ////

// Pr = Pt * Gt * Gr * lambda^2 / (4Pi*R)^2
// Pr = Pt * Gt * Gr * c^2 / (4Pi*R*freq)^2
