//	Package:	sciCalc
//	File:		bpf3Calc.c 
//	Purpose:	Calculate Band Pass Filter component values for 
//				Butterworth, Chebychev, and Bessel filters with
//				3 resonant circuits using the K-Q Method
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
#include <stdarg.h>
#include <math.h>
#include <check.h>
#include "bpf1Calc.h"
#include "bpf2Calc.h"
#include "bpf3Calc.h"
#include "ustripZCalc.h"
#include "inrushICalc.h"
#include "complexCalc.h"
#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000
#define TERA 1000000000000 
#define TXT_FILE "bpf2Data.txt"

float cntrFreqHz_3 = 0;
float bw3dB_kHz_3 = 0.000001;
float inductorValue_H_3 = 0;
float Qbp_3 = 0;
int choice_3 = 0;
float km_3 = 0; // magnitude scaling factor
float kf_3 = 0; // frequency scaling factor
//~ float tempf;

/**
 * Steps for 3 element k-q method:
 *		1) 	Choose filter type
 * 		2) 	Calculate the pass band “Q”, Qbp
 * 				Qbp = Fo / BW 3dB
 *  	3) 	Calculate the loaded Q for input and output resonator, Q1 and Q3
 * 				Q1_3 = Qbp * q1_3
 * 				Q3_3 = Qbp * q3_3
 *  	4) 	Calculate the coupling coefficients, K12 and K23, for this filter
 * 				K12_3 = k12_3 / Qbp
 * 				K23_3 = k23_3 / Qbp
 *  	5) 	Choose Inductor value
 * 		6)	Calculate Resistor values, R1 and R2
 *  	7) 	Calculate the nodal capacitance, Cnode3, which resonates with the inductor at Fo
 * 				Cnode_3 = 1 / ((2Pi*Fo)^2 * L)
 *  	8) 	Calculate the coupling capacitors, C12 and C23
 * 				C12_3 = K12_3 * Cnode3
 * 				C23_3 = K23_3 * Cnode3
 *		9)	Finally the values of C1, C2 and C3 can be calculated. 
 *			These are the capacitors that resonate with the inductors at Fo, 
 *			less the value of the coupling capacitor C12
 * 				C1_3 = Cnode3 – C12_3
 * 				C2_3 = Cnode3 – C12_3 – C23_3
 * 				C3_3 = Cnode3 – C12_3
 **/

struct comp_3 {
	float R1;
	float R2;
	float C1;
	float C2;
	float C3;
	float C12;
	float C23;
	float L1;	
	float L2;	
	float L3;	
};

/// variable declarations for 3 resonant circuits ///
float l_3, c12_3, c23_3, c1_3, c2_3, c3_3, r1_3, r2_3; 
float q1_3, q2_3, q3_3, k12_3, k23_3, cnode_3;
float f3dB2rbw;

//  inductor    Q1          Q3          K12BW       K23BW		CNode
float lBW_3,    q1BW_3,     q3BW_3,     k12BW_3,    k23BW_3,	cnodeBW_3;    // Butterworth
float lCH_01_3, q1CH_01_3,  q3CH_01_3,  k12CH_01_3, k23CH_01_3,	cnodeCH_01_3; // Chebychev 0.1dB
float lCH_05_3, q1CH_05_3,  q3CH_05_3,  k12CH_05_3, k23CH_05_3,	cnodeCH_05_3; // Chebychev 0.5dB
float lCH_1_3,  q1CH_1_3,   q3CH_1_3,   k12CH_1_3,  k23CH_1_3,	cnodeCH_1_3;  // Chebychev 1.0dB
float lBES_3,   q1BES_3,    q3BES_3,    k12BES_3,   k23BES_3,	cnodeBES_3;   // Bessel

//	Qbp = Fo / BW 3dB
float calcQbp_3(float cf, float bw){
	Qbp_3 = (cf/bw);
	//~ printf("Qbp = %f\n",Qbp); // DBPRINT
	return Qbp_3;
}
void setKQValues_3(int ch){	
	f3dB2rbw = f3dB2rbw_3;
	choice_3 = ch;
	switch(choice_3){
// 3 resonant circuit filters
		// 3 resonant circuit filters
		case 1:
		  q1_3 = Q1BW_3;
		  k12_3 = K12BW_3;
		  k23_3 = K23BW_3;
		  q3_3 = Q3BW_3;
		  break;
		case 2:
		  q1_3 = Q1CH_01DB_3;
		  k12_3 = K12CH_01DB_3;
		  k23_3 = K23CH_01DB_3;
		  q3_3 = Q3CH_01DB_3;
		  break;
		case 3:
		  q1_3 = Q1CH_05DB_3;
		  k12_3 = K12CH_05DB_3;
		  k23_3 = K23CH_05DB_3;
		  q3_3 = Q3CH_05DB_3;
		  break;
		case 4:
		  q1_3 = Q1CH_1DB_3;
		  k12_3 = K12CH_1DB_3;
		  k23_3 = K23CH_1DB_3;
		  q3_3 = Q3CH_1DB_3;
		  break;
		case 5:
		  q1_3 = Q1BES_3;
		  k12_3 = K12BES_3;
		  k23_3 = K23BES_3;
		  q3_3 = Q3BES_3;
		  break;
		default:
		  break;
	}
}
float calcQ1_3(void){
	float result = Qbp_3*q1_3;
	//~ printf("Q1 = %f\n",result);
	return result;
}
float calcQ3_3(void){
	float result = Qbp_3*q3_3;
	//~ printf("Q3 = %f\n",result);
	return result;
}
float calcK12_3(void){
	float result = k12_3/Qbp_3;
	//~ printf("K12 = %f\n",result);
	return result;
}
float calcK23_3(void){
	float result = k23_3/Qbp_3;
	//~ printf("K23 = %f\n",result);
	return result;
}
float calcCNode3(float cf, float ind){
	float result = 1/(pow((2*PI*cntrFreqHz_3),2)*inductorValue_H_3);
	cnode_3 = result;
	//~ printf("CNode3 = %f\n",result*GIGA);
	return result;
}
float calcC12_3(void){
	float result = (calcK12_3()*calcCNode3(cntrFreqHz_3,inductorValue_H_3));
	//~ printf("C12 = %f nF\n",result*GIGA);
	return result;
}
float calcC23_3(void){
	float result = (calcK23_3()*calcCNode3(cntrFreqHz_3,inductorValue_H_3));
	//~ printf("C23 = %f nF\n",result*GIGA);
	return result;
}
float calcC1_3(void){
	float result = (calcCNode3(cntrFreqHz_3,inductorValue_H_3) - calcC12_3());
	//~ printf("C1 = %f nF\n",result*GIGA);
	return result;
}
float calcC2_3(void){
	float result = (calcCNode3(cntrFreqHz_3,inductorValue_H_3)  - calcC12_3() - calcC23_3());
	//~ printf("C2 = %f nF\n",result*GIGA);
	return result;
}
float calcC3_3(void){
	float result = (calcCNode3(cntrFreqHz_3,inductorValue_H_3) - calcC12_3());
	//~ printf("C3 = %f nF\n",result*GIGA);
	return result;
}
float calcR1_3(void){
	float result = ((2*PI*cntrFreqHz_3*inductorValue_H_3*(calcQ1_3())));
	//~ printf("R1 = %f Ohms\n",result);
	return result;
}
float calcR2_3(void){
	float result = ((2*PI*cntrFreqHz_3*inductorValue_H_3*(calcQ3_3())));
	//~ printf("R2 = %f Ohms\n",result);
	return result;
}
float calcBW_3 (float r, float c) {
	float result = 1/(r*c*PI);	// (1/RC)/(PI/2) = 2/(RC*PI)
	printf("\nCalculated BW (1/(PI*RC)):\t%.3f kHz\n",result/KILO);
	return result;
}
void printBPF3(struct comp_3 f1) {
	//~ printf("\nprintBPF3\n");
	printf("\n");
	printf("%.6f nF\tC1\n",f1.C1*GIGA);
	printf("%.6f nF\tC2\n",f1.C2*GIGA);
	printf("%.6f nF\tC3\n",f1.C3*GIGA);
	printf("%.6f nF\tC12\n",f1.C12*GIGA);
	printf("%.6f nF\tC23\n",f1.C23*GIGA);
	printf("%.6f uH\tL1\n",f1.L1*MEGA);
	printf("%.6f uH\tL2\n",f1.L2*MEGA);
	printf("%.6f uH\tL3\n",f1.L3*MEGA);
	printf("%.3f ohms\tR1\n",f1.R1);
	printf("%.3f ohms\tR2\n",f1.R2);
	float cbw1 = (1/(f1.R1*f1.C1*PI));
	float cbw2 = (1/(f1.R2*f1.C3*PI)); // *f3dB2rbw
	//calcBW_3(f1.R1,f1.C1);
	printf("\nTarget BW:\t\t\t%.3f kHz\t\t%.6f MHz",bw3dB_kHz_3/KILO,bw3dB_kHz_3/MEGA);
	printf("\nCalculated BW:\t\t\t%.3f kHz\t\t%.6f MHz\n",(cbw1/KILO),(cbw1/MEGA));
	printf("Difference between the two:\t %.3f kHz\t\t%.6f MHz\n",((bw3dB_kHz_3-cbw1)/KILO),((bw3dB_kHz_3-cbw1)/MEGA));
	printf("Deviation from Target BW:\t%.3f percent\n",(((bw3dB_kHz_3-cbw1)/KILO)/(bw3dB_kHz_3/KILO))*100);
	//calcBW_3(f1.R2,f1.C3);
	printf("\nTarget BW:\t\t\t%.3f kHz\t\t%.6f MHz",bw3dB_kHz_3/KILO,bw3dB_kHz_3/MEGA);
	printf("\nCalculated BW (1/(PI*RC)):\t%.3f kHz\t\t%.6f MHz\n",(cbw2/KILO),(cbw2/MEGA));
	//~ printf("Calculated BW * f3dB2rbw_3:\t%.3f kHz\t\t%.6f MHz\n",(cbw2/KILO)*f3dB2rbw,(cbw2/MEGA)*f3dB2rbw);
	printf("Difference between the two:\t %.3f kHz\t\t%.6f MHz\n",((bw3dB_kHz_3-cbw2)/KILO),((bw3dB_kHz_3-cbw2)/MEGA));
	printf("Deviation from Target BW:\t%.3f percent\n",(((bw3dB_kHz_3-cbw2)/KILO)/(bw3dB_kHz_3/KILO))*100);
}
struct comp_3 computeValues_3(int ch){
	struct comp_3 result;
	float cf, bw, ind;	
	// First we initialize globals with user input	
	printf("Please enter Center Frequency in MHz: \n");
	scanf(" %f", &cf);		
	cntrFreqHz_3 = cf*MEGA;
	printf("Please enter 3dB bandwidth in kHz: \n");
	scanf(" %f", &bw);
	bw3dB_kHz_3 = bw*KILO;
	printf("Please enter inductor value in uH: \n");
	scanf(" %f", &ind);	
	inductorValue_H_3 = ind/MEGA;
	// Now we calculate values in the proper order
	setKQValues_3(ch);
	calcQbp_3(cntrFreqHz_3, bw3dB_kHz_3);
	calcQ1_3(); calcQ3_3(); calcK12_3(); calcK23_3();
	printf("\n");
	calcCNode3(cntrFreqHz_3, inductorValue_H_3);
	result.C12 = calcC12_3();
	result.C23 = calcC23_3();
	result.C1 = calcC1_3();
	result.C2 = calcC2_3();
	result.C3 = calcC3_3();
	result.R1 = calcR1_3();
	result.R2 = calcR2_3();
	result.L1 = ind/MEGA;
	result.L2 = ind/MEGA;
	result.L3 = ind/MEGA;
	printBPF3(result);
	printf("\n");
	return result;
}

/** 
	setValues_3(float cf, float bw, float ind, int ch)
	Sets values and runs all the calculations to create and return a BPF3 
	with the following user-defined parameters:
		cf = center freq (fo)
		bw = 3dB bandwidth we want 
		ind = inductor we want 
		ch = choice, type of filter we are creating 
**/
struct comp_3 setValues_3 (float cf, float bw, float ind, int ch){
	struct comp_3 result;
// First we initialize globals	
	cntrFreqHz_3 = cf*MEGA;
	bw3dB_kHz_3 = bw*KILO;
	inductorValue_H_3 = ind/MEGA;
	//~ printf("cntrFreqHz: %.4f MHz\n",cntrFreqHz_3); // DBPRINT
	//~ printf("bw3dB_Hz: %.4f kHz\n", bw3dB_kHz_3); // DBPRINT
// Now we calculate values in the proper order
	setKQValues_3(ch);
	calcQbp_3(cntrFreqHz_3, bw3dB_kHz_3);
	calcQ1_3(); calcQ3_3(); calcK12_3(); calcK23_3();
	//~ printf("\n");
	calcCNode3(cntrFreqHz_3, inductorValue_H_3);
	result.C12 = calcC12_3();
	result.C23 = calcC23_3();
	result.C1 = calcC1_3();
	result.C2 = calcC2_3();
	result.C3 = calcC3_3();
	result.R1 = calcR1_3();
	result.R2 = calcR2_3();
	result.L1 = inductorValue_H_3;
	result.L2 = inductorValue_H_3;
	result.L3 = inductorValue_H_3;
	printBPF3(result);
	printf("\n");
	return result;
}

/** 
	modBPF_C_3(float wl, float r, float frq)
	Maintains the frq and res of our BPF3, but changes the ind to a 
	new value (wl), and returns the cap required to achieve these reqs
		wl = the inductor we want to use 
		r = resistor we are using 
		frq = center freq of our resonator (fo)
**/
float modBPF_C_3(float wl, float r, float frq){
	printf("\nBPF at Center Freq:\t\t%.6f MHz\n",frq/MEGA);
	float newCapVal = 1/(pow((2*PI*frq),2)*wl);
	//~ printf("Orig Center Freq:\t\t\t%.3f kHZ\t\t%.6f MHz\n",frq/KILO,frq/MEGA); // DBPRINT = 1/(pow((2*PI*frq),2)*wl);
	float cbw = 1/(r*newCapVal*PI);
	//~ float mbw = 1/(r*newCapVal*PI);
	float ncf = 1/((2*PI)*sqrt(wl*newCapVal));
	//~ printf("Old Cap Value:\t\t\t%.6f nF\n",newCapVal*GIGA); // DBPRINT
	printf("Resistor: \t\t\t%.3f Ohms\nInductor:\t\t\t%.6f uH\nCapacitor:\t\t\t%.6f nF\n",r,wl*MEGA,newCapVal*GIGA); // DBPRINT
	//~ printf("New Cap:\t\t\t%.6f nF\n",newCapVal*GIGA); // DBPRINT
	//~ printf("Orig Center Freq:\t\t%.3f kHZ\t\t%.6f MHz\n",frq/KILO,frq/MEGA); // DBPRINT
	//~ printf("New Center Freq:\t\t%.3f kHZ\t\t%.6f MHz\n",ncf/KILO,ncf/MEGA); // DBPRINT
	printf("Calculated BW (1/(PI*RC)):\t%.3f kHz\t\t%.6f MHz\n",(cbw/KILO),(cbw/MEGA)); // DBPRINT
	return newCapVal;
}

/** 
	NT_3 (struct comp_3 fin, float scale)
	Norton's Transform for scaling impedance on 3-circuit BPFs (BPF3). 
	
	Generally we want to scale to a convenient inductor value, or we
	may need to scale to a 50 Ohm or 75 Ohm output resistor. 
	
		fin = the BPF3 whose output stage we are modifying. We will
				use setValues_3() to create and test our BPF3 for now.
				
		scale = the transform coefficient to which we are scaling. 
	
	Examples:
		NT_3(holder3, (2.2/1.0)); 			Scaling 1.0 uH to 2.2uH inductor
		NT_3(holder3, (2.2/holder3.L1)); 	Scaling output to 2.2uH inductor
		NT_3(holder3, (50/holder3.R2)); 	Scaling output to 50 Ohm resistor
		NT_3(holder3, (75/holder3.R2)); 	Scaling output to 75 Ohm resistor
**/
struct comp_3 NT_3 (struct comp_3 fin, float scale) {
	struct comp_3 result;
	//~ printf("\n******* Begin Norton's Transform *******\n"); // DBPRINT
	//~ printf("\nRunning Norton's Transform on this BPF"); // DBPRINT
	//~ printf("\nNOTE: Use only on 3-circuit BPFs for now\n"); // DBPRINT
	//~ printf("Scaling impedance by:\t%.6f times\n",scale); // DBPRINT
	//~ printf("\nComponent values before Norton's transform:"); // DBPRINT
	//~ printBPF3(fin); // DBPRINT
	
/// Begin Transforming Impedance of our input filter (fin)
/// First we scale components of the output resonant circuit
	fin.C3 /= scale;
	fin.R2 *= scale;
	fin.L3 *= scale;
	//printf("\n******************************************\n"); // DBPRINT
	//~ printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"); // DBPRINT
	//~ printf("\nFirst we scale all components in the last resonant circuit"); // DBPRINT
	//~ printf("\nR2 = %f * %f ohms = %f ohms\n",fin.R1,scale,fin.R2); // DBPRINT
	//~ printf("C3 = %f / %f nF = %f nF\n",fin.C1*GIGA,scale,fin.C3*GIGA); // DBPRINT
	//~ printf("L3 = %f * %f uH = %f uH\n",fin.L1*MEGA,scale,fin.L3*MEGA); // DBPRINT
/// Transformers change impedance by the square of the number of turns,
/// so our notional turns ratio, Nratio = sqrt of the turns ratio, L1/L3
	float Nratio = sqrt(fin.L1/fin.L3);
	//~ printf("\nThen we calculate the Turns Ratio (Nratio) of our virtual transformer"); // DBPRINT
	//~ printf("\nNratio:\t%.4f\n",Nratio); // DBPRINT
	//~ printf("\nThen we use Nratio to calculate impedance inverter components (nc1, nc12, and nc2)\n"); // DBPRINT
/// The Norton Transform virtual cap calculations
	float nc1 = (1-Nratio)*fin.C23;
	float nc12 = Nratio*fin.C23;
	float nc2 = Nratio*fin.C23*(Nratio-1);
	//~ printf("nc1 = %.6f nF\tnc12 = %.6f nF\tnc2 = %.6f nF\n",nc1*GIGA, nc12*GIGA, nc2*GIGA); // DBPRINT
	//~ printf("\nThen we merge our virual components into the circuit\n"); //  // DBPRINT
	//~ //printf("\n******************************************\n"); // DBPRINT
	//~ printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n"); // DBPRINT
/// Apply the transform to the appropriate filter caps
	fin.C2 += nc1;
	fin.C23 = nc12;
	fin.C3 += nc2;
/// End Transforming Impedance 

	result = fin;
	printf("\nComponent values after Norton's transform:"); // DBPRINT
	printBPF3(result); // DBPRINT
	//eqcaps_series(fin.C1,fin.C2,fin.C12);
	//~ eqcaps_series((1e-6),(5e-6),(5e-6));
	//~ eqcaps_series((1),(5),(5));
	//~ modBPF_C_3(0.000001, 2777, 10000000);
	//~ modBPF_C_3((fin.L3), fin.R2, cntrFreqHz_3);
	//~ modBPF_C_3((fin.L3), 75, cntrFreqHz_3);
	printf("\n******* End Norton's Transform *******\n\n"); // DBPRINT
	return result;
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
char * floatToStringBPF3(float f){
	char buffer[DATA_SIZE];
    return gcvt(f,DATA_SIZE,buffer);
}
void storeDataInFileBPF3(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringBPF3(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}
void clearDataFromFileBPF3(){
  	FILE * fPtr = fopen(TXT_FILE, "w");
  	fclose(fPtr);
}
int getUserInputBPF3(){
	int exitFlag = 0;	
	int ch;
	printf("Please choose from the following: \n");
	printf("1 for Butterworth Filter with 3 tuned circuits\n");
	printf("2 for Chebychev 0.1dB with 3 tuned circuits\n");
	printf("3 for Chebychev 0.5dB with 3 tuned circuits\n");
	printf("4 for Chebychev 1.0dB with 3 tuned circuits\n");
	printf("5 for Bessel with 3 tuned circuits\n");
  	//~ printf("11 to clear bpfData.txt\n");
	//~ printf("12 for store to file\n");
	printf("10 for quit\n");
	scanf(" %i", &ch);
	
	switch (ch) {
		case 1:
			computeValues_3(1);
			break;
		case 2:
			computeValues_3(2);
			break;
		case 3:
			computeValues_3(3);
			break;
		case 4:
			computeValues_3(4);
			break;
		case 5:
			computeValues_3(5);
			break;
		//~ case 11:
			//~ clearDataFromFileBPF3();
			//~ printf("bpfData.txt is now cleared of all data");
			//~ break;
		//~ case 12:
			//~ printf("Writing data to file\n");
			//~ storeDataInFileBPF3(tempf);
			//~ break;
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
	else getUserInputBPF3();
}
//
	//~ int main(int argc, char const *argv[]) {
	//~ getUserInputBPF2();
	//~ return 0;
 //~ }

/// Variadic Arguments
//~ float eqcaps_series(float caps, ...){
	//~ float result = 0.0;	
	//~ int i;	
	
	//~ va_list valist;
	//~ /* initialize valist for num number of arguments */
	//~ va_start(valist, caps);

	//~ /* access all the arguments assigned to valist */
	//~ for (i = 0; i < caps; i++) {
	  //~ printf("\narg list: %lf\n",va_arg(valist, double));	  
	  //~ result += va_arg(valist, double);
	  //~ printf("loop result: %f\t",result);
	//~ }

	//~ /* clean memory reserved for valist */
	//~ va_end(valist);
	//~ result = (1/result);
	//~ printf("\nequivalent cap value = %lf nF\n",result*TERA);
	//~ return result;
//~ }

//~ double derive(double (*f)(double), double x0)
//~ {
    //~ const double delta = 1.0e-6; // or similar
    //~ double x1 = x0 - delta;
    //~ double x2 = x0 + delta;
    //~ double y1 = f(x1);
    //~ double y2 = f(x2);
    //~ return (y2 - y1) / (x2 - x1);
//~ }

//// call it as follows:
//// #include <math.h>

//// double der = derive(sin, 0.0);
//// printf("%lf\n", der); // should be around 1.0

//// DO NOT REMOVE THE FOLLOWING SECTION!! ////
// https://www.electricaltechnology.org/2020/11/rlc-circuits-series-parallel-equations.html

// Equation Jackpot!! See Below....
// https://www.electricaltechnology.org/2020/11/active-passive-frequency-filters-formulas.html 

//// Band-Pass Filter ////
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

//// End of Bandpass Filter ////

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



//~ 	Standard Inductor Values
//~ 	nH,µH		nH,µH		nH,µH		nH,µH
//~ 	1.0			10			100			1000
//~ 	1.1			11			110			1100
//~ 	1.2			12			120			1200
//~ 	1.3			13			130			1300
//~ 	1.5			15			150			1500
//~ 	1.6			16			160			1600
//~ 	1.8			18			180			1800
//~ 	2.0			20			200			2000
//~ 	2.2			22			220			2200
//~ 	2.4			24			240			2400
//~ 	2.7			27			270			2700
//~ 	3.0			30			300			3000
//~ 	3.3			33			330			3300
//~ 	3.6			36			360			3600
//~ 	3.9			39			390			3900
//~ 	4.3			43			430			4300
//~ 	4.7			47			470			4700
//~ 	5.1			51			510			5100
//~ 	5.6			56			560			5600
//~ 	6.2			62			620			6200
//~ 	6.8			68			680			6800
//~ 	7.5			75			750			7500
//~ 	8.2			82			820			8200
//~ 	8.7			87			870			8700
//~ 	9.1			91			910			9100


