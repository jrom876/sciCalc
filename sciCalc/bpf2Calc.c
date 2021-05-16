
//	Package:	sciCalc
//	File:		bpf2Calc.c 
//	Purpose:	Calculate Band Pass RLC Filter component values for 
//			Butterworth, Chebychev, and Bessel filters with 
//			2 or 3 resonant circuits using the K-Q Method
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
#include "bpf2Calc.h"
#define TXT_FILE "bpf2Data.txt"

float cntrFreqHz = 0;
float bw3dB_Hz = 0;
float inductorValue_H = 0;
float Qbp = 0;
int   choice = 0;
float tempf;

/// variable declarations for 2 resonant circuits ///
float l, c12, c1, c2, r1, r2; 
float q1, q2, k12, cnode_2;

//  inductor    Q1          Q2          K12BW       CNode
float lBW,      q1BW,       q2BW,       k12BW,      cnodeBW;    // Butterworth
float lCH_01,   q1CH_01,    q2CH_01,    k12CH_01,   cnodeCH_01; // Chebychev 0.1dB
float lCH_05,   q1CH_05,    q2CH_05,    k12CH_05,   cnodeCH_05; // Chebychev 0.5dB
float lCH_1,    q1CH_1,     q2CH_1,     k12CH_1,    cnodeCH_1;  // Chebychev 1.0dB
float lBES,     q1BES,      q2BES,      k12BES,     cnodeBES;   // Bessel

/// variable declarations for 3 resonant circuits ///
float l_3, c12_3, c23_3, c1_3, c2_3, c3_3, r1_3, r2_3; 
float q1_3, q2_3, q3_3, k12_3, k23_3, cnode_3;

//  inductor    Q1          Q3          K12BW       K23BW	CNode
float lBW_3,    q1BW_3,     q3BW_3,     k12BW_3,    k23BW_3,	cnodeBW_3;    // Butterworth
float lCH_01_3, q1CH_01_3,  q3CH_01_3,  k12CH_01_3, k23CH_01_3,	cnodeCH_01_3; // Chebychev 0.1dB
float lCH_05_3, q1CH_05_3,  q3CH_05_3,  k12CH_05_3, k23CH_05_3,	cnodeCH_05_3; // Chebychev 0.5dB
float lCH_1_3,  q1CH_1_3,   q3CH_1_3,   k12CH_1_3,  k23CH_1_3,	cnodeCH_1_3;  // Chebychev 1.0dB
float lBES_3,   q1BES_3,    q3BES_3,    k12BES_3,   k23BES_3,	cnodeBES_3;   // Bessel

// Qbp = Fo / BW 3dB
float calcQbp(float cf, float bw){
	Qbp = (cf/bw);
	//~ printf("Qbp = %f\n",Qbp);
	return Qbp;
}

void setKQValues(int ch){
	choice = ch;
	switch(choice){
// 2 resonant circuit filters
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
// 3 resonant circuit filters
		case 6:
		  q1_3 = Q1BW_3;
		  k12_3 = K12BW_3;
		  k23_3 = K23BW_3;
		  q3_3 = Q3BW_3;
		  break;
		case 7:
		  q1_3 = Q1CH_01DB_3;
		  k12_3 = K12CH_01DB_3;
		  k23_3 = K23CH_01DB_3;
		  q3_3 = Q3CH_01DB_3;
		  break;
		case 8:
		  q1_3 = Q1CH_05DB_3;
		  k12_3 = K12CH_05DB_3;
		  k23_3 = K23CH_05DB_3;
		  q3_3 = Q3CH_05DB_3;
		  break;
		case 9:
		  q1_3 = Q1CH_1DB_3;
		  k12_3 = K12CH_1DB_3;
		  k23_3 = K23CH_1DB_3;
		  q3_3 = Q3CH_1DB_3;
		  break;
		case 10:
		  q1_3 = Q1BES_3;
		  k12_3 = K12BES_3;
		  k23_3 = K23BES_3;
		  q3_3 = Q3BES_3;
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
	//~ printf("Q1 = %f\n",result);
	return result;
}

float calcQ2(void){
	float result = Qbp*q2;
	//~ printf("Q2 = %f\n",result);
	// tempf = result;
	return result;
}

float calcK12(void){
	float result = k12/Qbp;
	//~ printf("K12 = %f\n",result);
	return result;
}

float calcCNode2(float cf, float ind){
	float result = 1/(((2*PI*cntrFreqHz)*(2*PI*cntrFreqHz))*inductorValue_H);
	cnode_2 = result;
	//~ printf("CNode = %f\n",result*1000000000);
	return result;
}

float calcC12(void){
	float result = (calcK12()*calcCNode2(cntrFreqHz,inductorValue_H));
	//~ printf("C12 = %f nF\n",result*1000000000);
	return result;
}

float calcC1(void){
	float result = (calcCNode2(cntrFreqHz,inductorValue_H) - calcC12());
	printf("C1 = %f nF\n",result*1000000000);
	return result;
}

float calcC2(void){
	float result = (calcCNode2(cntrFreqHz,inductorValue_H) - calcC12());
	printf("C2 = %f nF\n",result*1000000000);
	return result;
}

float calcR1(void){
	float result = ((2*PI*cntrFreqHz*inductorValue_H*(calcQ1())));
	printf("R1 = %f Ohms\n",result);
	return result;
}

float calcR2(void){
	float result = ((2*PI*cntrFreqHz*inductorValue_H*(calcQ2())));
	printf("R2 = %f Ohms\n",result);
	return result;
}

void computeValues_2(int ch){
	float cf, bw, ind;	
	// First we initialize globals with user input	
	printf("Please enter Center Frequency in MHz: \n");
	scanf(" %f", &cf);		
	cntrFreqHz = cf*1000000;
	printf("Please enter 3dB bandwidth in kHz: \n");
	scanf(" %f", &bw);
	bw3dB_Hz = bw*1000;
	printf("Please enter inductor value in uH: \n");
	scanf(" %f", &ind);	
	inductorValue_H = ind/1000000;
	// Now we calculate values in the proper order
	setKQValues(ch);
	calcQbp(cntrFreqHz, bw3dB_Hz);
	calcQ1();
	calcQ2();
	calcK12();
	printf("\n");
	calcR1();
	calcR2();
	calcCNode2(cntrFreqHz, inductorValue_H);
	calcC12();
	calcC1();
	calcC2();
	printInductorValue();
	printf("\n");
}

/**
 * Steps for 3 element k-q method:
 *	1) 	Choose filter type
 * 	2) 	Calculate the pass band “Q”, Qbp
 * 			Qbp = Fo / BW 3dB
 *  	3) 	Calculate the loaded Q for input and output resonator, Q1 and Q3
 * 			Q1_3 = Qbp * q1_3
 * 			Q3_3 = Qbp * q3_3
 *  	4) 	Calculate the coupling coefficients, K12 and K23, for this filter
 * 			K12_3 = k12_3 / Qbp
 * 			K23_3 = k23_3 / Qbp
 *  	5) 	Choose Inductor value
 * 	6)	Calculate Resistor values, R1 and R2
 *  	7) 	Calculate the nodal capacitance, Cnode3, which resonates with the inductor at Fo
 * 			Cnode_3 = 1 / ((2Pi*Fo)^2 * L)
 *  	8) 	Calculate the coupling capacitors, C12 and C23
 * 			C12_3 = K12_3 * Cnode3
 * 			C23_3 = K23_3 * Cnode3
 *	9)	Finally the values of C1, C2 and C3 can be calculated. 
 *		These are the capacitors that resonate with the inductors at Fo, 
 *		less the value of the coupling capacitor C12
 * 			C1_3 = Cnode3 – C12_3
 * 			C2_3 = Cnode3 – C12_3 – C23_3
 * 			C3_3 = Cnode3 – C12_3
 **/

float calcQ1_3(void){
	float result = Qbp*q1_3;
	//~ printf("Q1 = %f\n",result);
	return result;
}

float calcQ3_3(void){
	float result = Qbp*q3_3;
	//~ printf("Q3 = %f\n",result);
	return result;
}

float calcK12_3(void){
	float result = k12_3/Qbp;
	//~ printf("K12 = %f\n",result);
	return result;
}

float calcK23_3(void){
	float result = k23_3/Qbp;
	//~ printf("K23 = %f\n",result);
	return result;
}

float calcCNode3(float cf, float ind){
	float result = 1/(((2*PI*cntrFreqHz)*(2*PI*cntrFreqHz))*inductorValue_H);
	cnode_3 = result;
	//~ printf("CNode3 = %f\n",result*1000000000);
	return result;
}

float calcC12_3(void){
	float result = (calcK12_3()*calcCNode3(cntrFreqHz,inductorValue_H));
	//~ printf("C12 = %f nF\n",result*1000000000);
	return result;
}

float calcC23_3(void){
	float result = (calcK23_3()*calcCNode3(cntrFreqHz,inductorValue_H));
	//~ printf("C23 = %f nF\n",result*1000000000);
	return result;
}

float calcC1_3(void){
	float result = (calcCNode3(cntrFreqHz,inductorValue_H) - calcC12_3());
	printf("C1 = %f nF\n",result*1000000000);
	return result;
}

float calcC2_3(void){
	float result = (calcCNode3(cntrFreqHz,inductorValue_H)  - calcC12_3() - calcC23_3());
	printf("C2 = %f nF\n",result*1000000000);
	return result;
}

float calcC3_3(void){
	float result = (calcCNode3(cntrFreqHz,inductorValue_H) - calcC12_3());
	printf("C3 = %f nF\n",result*1000000000);
	return result;
}

float calcR1_3(void){
	float result = ((2*PI*cntrFreqHz*inductorValue_H*(calcQ1_3())));
	printf("R1 = %f Ohms\n",result);
	return result;
}

float calcR2_3(void){
	float result = ((2*PI*cntrFreqHz*inductorValue_H*(calcQ3_3())));
	printf("R2 = %f Ohms\n",result);
	return result;
}

void computeValues_3(int ch){
	float cf, bw, ind;	
	// First we initialize globals with user input	
	printf("Please enter Center Frequency in MHz: \n");
	scanf(" %f", &cf);		
	cntrFreqHz = cf*1000000;
	printf("Please enter 3dB bandwidth in kHz: \n");
	scanf(" %f", &bw);
	bw3dB_Hz = bw*1000;
	printf("Please enter inductor value in uH: \n");
	scanf(" %f", &ind);	
	inductorValue_H = ind/1000000;
	// Now we calculate values in the proper order
	setKQValues(ch);
	calcQbp(cntrFreqHz, bw3dB_Hz);
	calcQ1_3();
	calcQ3_3();
	calcK12_3();
	calcK23_3();
	printf("\n");
	calcR1_3();
	calcR2_3();
	calcCNode3(cntrFreqHz, inductorValue_H);
	calcC12_3();
	calcC23_3();
	calcC1_3();
	calcC2_3();
	calcC3_3();
	printInductorValue();
	printf("\n");
}
/////////////////////////////////////////////

float printInductorValue(void) {
	printf("Ind = %f uH\n",inductorValue_H*1000000);
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
	printf("6 for Butterworth Filter with 3 tuned circuits\n");
	printf("7 for Chebychev 0.1dB with 3 tuned circuits\n");
	printf("8 for Chebychev 0.5dB with 3 tuned circuits\n");
	printf("9 for Chebychev 1.0dB with 3 tuned circuits\n");
	printf("10 for Bessel with 3 tuned circuits\n");
  	printf("11 to clear bpfData.txt\n");
	printf("12 for store to file\n");
	printf("13 for quit\n");
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
		case 6:
			computeValues_3(6);
			break;
		case 7:
			computeValues_3(7);
			break;
		case 8:
			computeValues_3(8);
			break;
		case 9:
			computeValues_3(9);
			break;
		case 10:
			computeValues_3(10);
			break;
		case 11:
			clearDataFromFileBPF2();
			printf("bpfData.txt is now cleared of all data");
			break;
		case 12:
			printf("Writing data to file\n");
			storeDataInFileBPF2(tempf);
			break;
		case 13:
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exitFlag = 1;
			break;
	}
	if(0 != exitFlag) return 0;
	getUserInputBPF2();
}
//
	//~ int main(int argc, char const *argv[]) {
	//~ getUserInputBPF2();
	//~ return 0;
 //~ }
