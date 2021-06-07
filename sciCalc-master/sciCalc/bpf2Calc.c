
//	Package:	sciCalc
//	File:		bpf2Calc.c 
//	Purpose:	Calculate Band Pass RLC Filter component values for 
//			Butterworth, Chebychev, and Bessel filters with 
//			2 resonant circuits using the K-Q Method
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
// https://en.wikipedia.org/wiki/Equivalent_impedance_transforms
// https://www.electricaltechnology.org/2020/11/rlc-circuits-series-parallel-equations.html
// https://www.electrical4u.com/lc-circuit-analysis/
// https://www.gnu.org/software/gsl/

// Jackpot!! See Below....
// https://www.electricaltechnology.org/2020/11/active-passive-frequency-filters-formulas.html 

////////////////////////////////////
//// Band-Pass Filter Equations ////
////////////////////////////////////
//~ Bandwidth:
//~ The total range of the allowable frequency is known as bandwidth, 
//~ from lower cutoff to upper cutoff frequency.
//~ β = ωc2 – ωc1
//~ β = R/L             Series RLC
//~ β = 1/RC            Parallel RLC

//~ Center Frequency:
//~ the center of the allowable band of frequency fc is given by:
//~ (same for series and parallel resonant circuits)
//~ ωo = sqrt(1/LC) = sqrt((wc1)*(wc2))
//~ ωo = sqrt(1/2PI*LC) parallel resonant

//~ Quality Factor:
//~ Q = β/ωo 
//~ Q = sqrt(L/(CR^2))	series resonant
//~ Q = sqrt((CR^2)/L)	parallel resonant

//~ Cutoff Frequency:
//~ There are two cutoff frequencies in band pass filters 
//~ i.e. Lower cutoff ωc1 & upper cutoff ωc2 , any frequency 
//~ before ωc1 and after ωc2 is being blocked by the filter.

//~ ωc1 = -(R/2L) + sqrt((R/2L)^2 + (1/LC))	series resonant circuit
//~ ωc2 =  (R/2L) + sqrt((R/2L)^2 + (1/LC))

//~ ωc1 = -(1/2RC) + sqrt((1/2RC))^2 + (1/LC))	parallel resonant circuit
//~ ωc2 =  (1/2RC) + sqrt((1/2RC))^2 + (1/LC))

//~ ωc1 = -(β/2) + sqrt((β/2))^2 + (ωo)^2)	generalized equation
//~ ωc2 =  (β/2) + sqrt((β/2))^2 + (ωo)^2)

//////////////////////////////////////////
//// End of Bandpass Filter Equations ////
//////////////////////////////////////////

//~ Magnitude Scaling
//~ if you only want to scale the magnitude of the filter.
//~ R’ = km R
//~ L’ = km L
//~ C’ = C / km

//~ Frequency Scaling
//~ When you only want to scale the frequency of the filter
//~ R’ = R
//~ L’ = L / kf
//~ C’ = C / kf

//~ Simultaneous Scaling
//~ When you want to scale the both frequency & magnitude of the filter
//~ R’ = km R
//~ L’ = (km/kf) L
//~ C’ = (1/kmkf) C
//~ R’ = scaled resistance
//~ L’ = scaled inductance
//~ C’ = scaled capacitance
//~ km = Magnitude scaling factor
//~ kf = frequency scaling factor
//// End of active Filters ////

/// Parallel RLC ///

// Impedance:		Z = 1 / (sqrt((1/R)^2 + ((1/Xl)-(1/Xc)))) in Ohms
// Power Factor: 	cos(theta) = Z/R

// Resonant Freq:	fr = 1 / (2PI *sqrt(LC)) in Hz
// 					omega = 1 / sqrt(LC) in rad/sec
// Neper Frequency:	alpha = fn = 1/(2RC)

// Overdamped:		omega^2 < alpha^2	roots are real and distinct
// Underdamped:		omega^2 > alpha^2	roots are complex and conjugate of each other
// Crit Damped:		omega^2 = alpha^2	roots are real and equal
// 
// Quality Factor:	Q = R/Xl = R/Xc = R * sqrt(C/L)
// Bandwidth:		BW = fr/Q = R/L (in rad/s) = R/(2PI*L) (in Hz)
// 

/// Series RLC ///

// Impedance:		Z = 1 / sqrt(R^2 + (Xl-Xc)^2) in Ohms
// Power Factor: 	cos(theta) = Z/R

// Resonant Freq:	fr = 1 / (2PI *sqrt(LC)) in Hz
// 					omega = 1 / sqrt(LC) in rad/sec
// Neper Frequency:	alpha = fn = 1/(2RC)

// Overdamped:		omega^2 < alpha^2	roots are real and distinct
// Underdamped:		omega^2 > alpha^2	roots are complex and conjugate of each other
// Crit Damped:		omega^2 = alpha^2	roots are real and equal
// 
// Quality Factor:	Q = omega * (L/R) = 1 / (omega*RC) = (1/R) * sqrt(L/C)
// Bandwidth:		BW = fr/Q = R/L (in rad/s) = R/(2PI*L) (in Hz)

// Upper and Lower Cutoff frequencies:
// 					fh = fr + ½ B.W
// 					fl = fr – ½ B.W

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <check.h>
#include "bpf1Calc.h"
#include "bpf2Calc.h"
#include "bpf3Calc.h"
#include "ustripZCalc.h"
#include "inrushICalc.h"
#include "complexCalc.h"
#define QBP (cntrFreqHz/bw3dB_Hz)
#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000
#define TERA 1000000000000 
#define TXT_FILE "bpf2Data.txt"

float cntrFreqHz = 0;
float bw3dB_Hz = 0;
float inductorValue_H = 0;
float Qbp = 0;
int choice = 0;
float km = 0; // magnitude scaling factor
float kf = 0; // frequency scaling factor
float tempf;

struct comp_2 {
	float R1;
	float R2;
	float C1;
	float C2;
	float C12;
	float L1;	
	float L2;	
};

/// variable declarations for 2 resonant circuits ///
float l, c12, c1, c2, r1, r2; 
float q1, q2, k12, cnode_2;

//  inductor    Q1          Q2          K12BW       CNode
float lBW,      q1BW,       q2BW,       k12BW,      cnodeBW;    // Butterworth
float lCH_01,   q1CH_01,    q2CH_01,    k12CH_01,   cnodeCH_01; // Chebychev 0.1dB
float lCH_05,   q1CH_05,    q2CH_05,    k12CH_05,   cnodeCH_05; // Chebychev 0.5dB
float lCH_1,    q1CH_1,     q2CH_1,     k12CH_1,    cnodeCH_1;  // Chebychev 1.0dB
float lBES,     q1BES,      q2BES,      k12BES,     cnodeBES;   // Bessel

//	Qbp = Fo / BW 3dB
float calcQbp(float cf, float bw){
	Qbp = (cf/bw);
	//~ printf("Qbp = %f\n",Qbp); // DBPRINT
	return Qbp;
}

void setKQValues(int ch){
	choice = ch;
	switch(choice){
		case 1:
		  q1 = Q1BW;
		  k12 = K12BW;
		  q2 = Q2BW;
		  break;
		case 2:
		  q1 = Q1CH_01DB;
		  k12 = K12CH_01DB;
		  q2 = Q2CH_01DB;
		  break;
		case 3:
		  q1 = Q1CH_05DB;
		  k12 = K12CH_05DB;
		  q2 = Q2CH_05DB;
		  break;
		case 4:
		  q1 = Q1CH_1DB;
		  k12 = K12CH_1DB;
		  q2 = Q2CH_1DB;
		  break;
		case 5:
		  q1 = Q1BES;
		  k12 = K12BES;
		  q2 = Q2BES;
		  break;
		default:
		  break;
	}
}

/**
 * Steps for 2 element k-q method:
 *	1) 	Choose filter type
 * 	2) 	Calculate the pass band “Q”, Qbp
 * 			Qbp = Fo / BW 3dB
 *  	3) 	Calculate the loaded Q for input and output resonator, Q1 and Q2
 * 			Q1 = Qbp * q1
 * 			Q2 = Qbp * q2
 *  	4) 	Calculate the coupling coefficient K12 for this filter
 * 			K12 = k12 / Qbp
 *  	5) 	Choose Inductor value
 * 	6)	Calculate Resistor values, R1 and R2
 *  	7) 	Calculate the nodal capacitance, Cnode, which resonates with the inductor at Fo
 * 			Cnode = 1 / ((2Pi*Fo)^2 * L)
 *  	8) 	Calculate the coupling capacitor C12
 * 			C12 = K12 * Cnode
 *	9)	Finally the values of C1 and C2 can be calculated. 
 *		These are the capacitors that resonate with the inductors at Fo, 
 *		less the value of the coupling capacitor C12
 * 			C1 = Cnode – C12
 * 			C2 = Cnode – C12
 **/

float calcQ1(void){
	float result = Qbp*q1;
	//~ printf("Q1 = %f\n",result); // DBPRINT
	return result;
}

float calcQ2(void){
	float result = Qbp*q2;
	//~ printf("Q2 = %f\n",result); // DBPRINT
	// tempf = result;
	return result;
}

float calcK12(void){
	float result = k12/Qbp;
	//~ printf("K12 = %f\n",result); // DBPRINT
	return result;
}

float calcCNode2(float cf, float ind){
	float result = 1/((pow((2*PI*cntrFreqHz),2))*inductorValue_H);
	cnode_2 = result;
	//~ printf("CNode = %f\n",result*GIGA); // DBPRINT
	return result;
}

float calcC12(void){
	float result = (calcK12()*calcCNode2(cntrFreqHz,inductorValue_H));
	//~ printf("C12 = %f nF\n",result*GIGA); // DBPRINT
	return result;
}

float calcC1(void){
	float result = (calcCNode2(cntrFreqHz,inductorValue_H) - calcC12());
	//~ printf("C1 = %f nF\n",result*GIGA); // DBPRINT
	return result;
}

float calcC2(void){
	float result = (calcCNode2(cntrFreqHz,inductorValue_H) - calcC12());
	//~ printf("C2 = %f nF\n",result*GIGA); // DBPRINT
	return result;
}

float calcR1(void){
	float result = ((2*PI*cntrFreqHz*inductorValue_H*(calcQ1())));
	//~ printf("R1 = %f Ohms\n",result); // DBPRINT
	return result;
}

float calcR2(void){
	float result = ((2*PI*cntrFreqHz*inductorValue_H*(calcQ2())));
	//~ printf("R2 = %f Ohms\n",result); // DBPRINT
	return result;
}

struct comp_2 computeValues_2(int ch){
	struct comp_2 result;
	float cf, bw, ind;	
	// First we initialize globals with user input	
	printf("Please enter Center Frequency in MHz: \n");
	scanf(" %f", &cf);		
	cntrFreqHz = cf*MEGA;
	printf("Please enter 3dB bandwidth in kHz: \n");
	scanf(" %f", &bw);
	bw3dB_Hz = bw*KILO;
	printf("Please enter inductor value in uH: \n");
	scanf(" %f", &ind);	
	inductorValue_H = ind/MEGA;
	// Now we calculate values in the proper order
	setKQValues(ch);
	calcQbp(cntrFreqHz, bw3dB_Hz);
	calcQ1(); calcQ2(); calcK12();
	printf("\n");
	calcCNode2(cntrFreqHz, inductorValue_H);
	result.C12 = calcC12();
	result.C1 = calcC1();
	result.C2 = calcC2();
	result.R1 = calcR1();
	result.R2 = calcR2();
	result.L1 = inductorValue_H; 
	result.L2 = inductorValue_H;
	printf("\n");
	return result;
}

struct comp_2 setValues_2(float cf, float bw, float ind, int ch){
	struct comp_2 result;
// First we initialize globals with user input	
	cntrFreqHz = cf*MEGA;
	//~ printf("cntrFreqHz: %.4f MHz\n",cntrFreqHz); // DBPRINT
	bw3dB_Hz = bw*KILO;
	//~ printf("bw3dB_Hz: %.4f kHz\n", bw3dB_Hz); // DBPRINT
	inductorValue_H = ind/MEGA;
// Now we calculate values in the proper order
	setKQValues(ch);
	calcQbp(cntrFreqHz, bw3dB_Hz);
	calcQ1(); calcQ2(); calcK12();
	printf("\n");
	calcCNode2(cntrFreqHz, inductorValue_H);
	result.C12 = calcC12();
	result.C1 = calcC1();
	result.C2 = calcC2();
	result.R1 = calcR1();
	result.R2 = calcR2();
	result.L1 = inductorValue_H;
	result.L2 = inductorValue_H;
	printBPF2(result);
	//printf("\n");
	return result;
}

//// Bartlett's Bisection Theorem for scaling impedance
struct comp_2 BBT_2 (struct comp_2 fin, float scale) {
	struct comp_2 result;
	printf("\n******* Begin Bartlett's Transform *******\n"); // DBPRINT
	printf("\nRunning Bartlett's Bisection Theorem on this BPF2, which allows");
	printf("\nthe user to modify the Impedance of the output filter stage."); // DBPRINT
	printf("\nNOTE: Use only on 2-resonant circuit filters with SYMMETRICAL CIRCUIT DIAGRAMS\n"); // DBPRINT
	printf("Scaling impedance by:\t%.6f times\n",scale); // DBPRINT
	printf("Component values before transform:"); // DBPRINT
	printBPF2(fin); // DBPRINT
	
/// Begin Transforming Impedance 
/// Definie line of symmetry as the C12 coupling cap, and double it
/// into two identical virtual caps, C12a and C12b
	float C12a = fin.C12*2;
	float C12b = fin.C12*2;
/// Series capacitor addition equation: 1/Cs = 1/C1 + 1/C2 + .... + 1/Cn
	fin.C12 = 1/((1/C12a) + (1/(C12b/scale)));
	//~ printf("BBT fin.C12 = %.6f nF",fin.C12*GIGA); // DBPRINT
	fin.C2 /= scale;
	fin.R2 *= scale;
	fin.L2 *= scale;
/// End Transforming Impedance 

	result = fin;
	printf("\nComponent values after transform:"); // DBPRINT
	printBPF2(result); // DBPRINT
	printf("\n******* End Bartlett's Transform *******\n"); // DBPRINT
	return result;
}

float calcBW_2 (float r, float c) {
	//float result = 1/(r*c);
	float result = 1/(r*c*PI);	// (1/RC)/(PI/2) = 2/(RC*PI)
	printf("Calculated BW (1/(PI*RC)):\t%.6f kHz\n",result/KILO); // DBPRINT
	return result;
}

void printBPF2(struct comp_2 f1) {
	//~ printf("\nprintBPF2\n");
	printf("\n%.6f nF\tC1\n",f1.C1*GIGA);
	printf("%.6f nF\tC2\n",f1.C2*GIGA);
	printf("%.6f nF\tC12\n",f1.C12*GIGA);
	printf("%.4f uH\tL1\n",f1.L1*MEGA);
	printf("%.4f uH\tL2\n",f1.L2*MEGA);
	printf("%.4f ohms\tR1\n",f1.R1);
	printf("%.4f ohms\tR2\n",f1.R2);
	//calcBW_2(f1.R1,f1.C1);
	//calcBW_2(f1.R2,f1.C2);
	//~ float cbw1 = 1/(f1.R1*f1.C1*PI);
	float cbw2 = 1/(f1.R2*f1.C2*PI);
	//calcBW_3(f1.R1,f1.C1);
	//~ printf("\nTarget BW:\t\t\t%.3f kHz",bw3dB_Hz/KILO);
	//~ printf("\nCalculated BW (1/(PI*RC)):\t%.3f kHz\n",(bw3dB_Hz/KILO));
	//~ printf("Difference between the two:\t %.3f kHz\n",((bw3dB_Hz-cbw1)/KILO));
	//~ printf("Deviation from Target BW:\t%.3f percent\n",(((bw3dB_Hz-cbw1)/KILO)/(bw3dB_Hz/KILO))*100);
	//calcBW_3(f1.R2,f1.C3);
	printf("\nTarget BW:\t\t\t%.3f kHz",bw3dB_Hz/KILO);
	printf("\nCalculated BW (1/(PI*RC)):\t%.3f kHz\n",(cbw2/KILO));
	printf("Difference between the two:\t %.3f kHz\n",((bw3dB_Hz-cbw2)/KILO));
	printf("Deviation from Target BW:\t%.3f percent\n",(((bw3dB_Hz-cbw2)/KILO)/(bw3dB_Hz/KILO))*100);
}

/////////////////////////////////////////////
//////////// File and IO Handling ///////////
/////////////////////////////////////////////
/// The following 4 functions create a .txt
/// file if it does not already exist; convert an
/// input float to a string and append the .txt
/// file with that string on a new line;
// clear all data from the .txt file if requested;
// and allow users to select menu choices.
////////////////////////////////////////////
char * floatToStringBPF2(float f){
	char buffer[DATA_SIZE];
	return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileBPF2(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringBPF2(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileBPF2(){
  	FILE * fPtr = fopen(TXT_FILE, "w");
  	fclose(fPtr);
}

int getUserInputBPF2(){
	int exitFlag = 0;
	//float q1,q2,k12,cnode;
	
	int ch;
	printf("Please choose from the following: \n");
	printf("1 for Butterworth Filter with 2 tuned circuits\n");
	printf("2 for Chebychev 0.1dB with 2 tuned circuits\n");
	printf("3 for Chebychev 0.5dB with 2 tuned circuits\n");
	printf("4 for Chebychev 1.0dB with 2 tuned circuits\n");
	printf("5 for Bessel with 2 tuned circuits\n");
	printf("10 for quit\n");
  	printf("11 to clear bpfData.txt\n");
	printf("12 for store to file\n");
	scanf(" %i", &ch);
	
	switch (ch) {
		case 1:
			computeValues_2(1);
			break;
		case 2:
			computeValues_2(2);
			break;
		case 3:
			computeValues_2(3);
			break;
		case 4:
			computeValues_2(4);
			break;
		case 5:
			computeValues_2(5);
			break;		
		case 11:
			clearDataFromFileBPF2();
			printf("bpfData.txt is now cleared of all data");
			break;
		case 12:
			printf("Writing data to file\n");
			storeDataInFileBPF2(tempf);
			break;
		case 10:
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
	else getUserInputBPF2();
}
//
	//~ int main(int argc, char const *argv[]) {
	//~ getUserInputBPF2();
	//~ return 0;
 //~ }
