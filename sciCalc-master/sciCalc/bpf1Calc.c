//	Package:	sciCalc
//	File:		bpf1Calc.c 
//	Purpose:	Calculate component values and other values for band pass
//				filters with variable numbers of resonant circuits 
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
#include "bpf1Calc.h"
#include "ustripZCalc.h"
#include "inrushICalc.h"
#include "complexCalc.h"
#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000
#define TERA 1000000000000 

float cntrFreqHz_1 = 0;
float bw3dB_kHz_1 = 0.000001;
float inductorValue_H_1 = 0;
float Qbp_1 = 0;
int choice_1 = 0;
float km_1 = 0; // magnitude scaling factor
float kf_1 = 0; // frequency scaling factor

/**
L filter = Ln Rt / (2 π Fbw )
C filter = Cn / (2*π*Fbw*Rt)


**/

//	Qbp = Fo / BW 3dB
float calcQbp_1(float cf, float bw){
	Qbp_1 = (cf/bw);
	//~ printf("Qbp = %f\n",Qbp); // DBPRINT
	return Qbp_1;
}

//~ void setKQValues_1(int ch){
	//~ choice = ch;
	//~ switch(choice){
		//~ case 1:
		  //~ q1 = Q1BW;
		  //~ k12 = K12BW;
		  //~ q2 = Q2BW;
		  //~ break;
		//~ case 2:
		  //~ q1 = Q1CH_01DB;
		  //~ k12 = K12CH_01DB;
		  //~ q2 = Q2CH_01DB;
		  //~ break;
		//~ case 3:
		  //~ q1 = Q1CH_05DB;
		  //~ k12 = K12CH_05DB;
		  //~ q2 = Q2CH_05DB;
		  //~ break;
		//~ case 4:
		  //~ q1 = Q1CH_1DB;
		  //~ k12 = K12CH_1DB;
		  //~ q2 = Q2CH_1DB;
		  //~ break;
		//~ case 5:
		  //~ q1 = Q1BES;
		  //~ k12 = K12BES;
		  //~ q2 = Q2BES;
		  //~ break;
		//~ default:
		  //~ break;
	//~ }
//~ }

float calcBW_1 (float r, float c) {
	float result = 1/(r*c*PI);	// (1/RC)/(PI/2) = 2/(RC*PI)
	printf("\nCalculated BW (1/(PI*RC)):\t%.3f kHz\n",result/KILO);
	return result;
}
void printBPF1(struct comp_1 f1) {
	printf("\nprintBPF1\n");
	//~ printf("\n");
	printf("%.6f nF\tC1\n",f1.C1*GIGA);
	printf("%.6f nF\tC2\n",f1.C2*GIGA);
	printf("%.6f uH\tL1\n",f1.L1*MEGA);
	printf("%.6f ohms\tR1\n",f1.R1);
	//~ printf("%.6f ohms\tR2\n",f1.R2);
	float cbw1 = 1/(f1.R1*f1.C1*PI);
	//~ float cbw2 = 1/(f1.R2*f1.C3*PI);
	//~ //calcBW_3(f1.R1,f1.C1);
	//~ printf("\nTarget BW:\t\t\t%.3f kHz\t\t%.6f MHz",bw3dB_kHz_3/KILO,bw3dB_kHz_3/MEGA);
	//~ printf("\nCalculated BW (1/(PI*RC)):\t%.3f kHz\t\t%.6f MHz\n",(cbw1/KILO),(cbw1/MEGA));
	//~ printf("Difference between the two:\t %.3f kHz\t\t%.6f MHz\n",((bw3dB_kHz_3-cbw1)/KILO),((bw3dB_kHz_3-cbw1)/MEGA));
	//~ printf("Deviation from Target BW:\t%.3f percent\n",(((bw3dB_kHz_3-cbw1)/KILO)/(bw3dB_kHz_3/KILO))*100);
	//~ //calcBW_3(f1.R2,f1.C3);
	//~ printf("\nTarget BW:\t\t\t%.3f kHz\t\t%.6f MHz",bw3dB_kHz_3/KILO,bw3dB_kHz_3/MEGA);
	//~ printf("\nCalculated BW (1/(PI*RC)):\t%.3f kHz\t\t%.6f MHz\n",(cbw2/KILO),(cbw2/MEGA));
	//~ printf("Difference between the two:\t %.3f kHz\t\t%.6f MHz\n",((bw3dB_kHz_3-cbw2)/KILO),((bw3dB_kHz_3-cbw2)/MEGA));
	//~ printf("Deviation from Target BW:\t%.3f percent\n",(((bw3dB_kHz_3-cbw2)/KILO)/(bw3dB_kHz_3/KILO))*100);
}

struct comp_1 setValues_1(float cf, float bw, float ind, int ch){
	struct comp_1 result;
// First we initialize globals with user input	
	cntrFreqHz_1 = cf*MEGA;
	//~ printf("cntrFreqHz: %.4f MHz\n",cntrFreqHz); // DBPRINT
	bw3dB_kHz_1 = bw*KILO;
	//~ printf("bw3dB_Hz: %.4f kHz\n", bw3dB_Hz); // DBPRINT
	inductorValue_H_1 = ind/MEGA;
// Now we calculate values in the proper order
	//~ setKQValues_1(ch);
	//calcQbp(cntrFreqHz_1, bw3dB_kHz_1);
	//~ calcQ1(); calcQ2(); calcK12();
	printf("This is only a test\n");
	//~ calcCNode2(cntrFreqHz_1, inductorValue_H_1);
	//~ result.C12 = calcC12();
	//~ result.C1 = calcC1();
	//~ result.C2 = calcC2();
	//~ result.R1 = calcR1();
	//~ result.R2 = calcR2();
	//~ result.L1 = inductorValue_H_1;
	//~ result.L2 = inductorValue_H_1;
	//~ printBPF2(result);
	//printf("\n");
	return result;
}

/// Variadic Arguments
float eqparallel_caps(int num, ...){
	printf("\nAdding Caps in Parallel\n");
	double result = 0.0, holder = 0.0;
	int i;		
	va_list valist;
	/* initialize valist for num number of arguments */
	va_start(valist, num);		
	/* access all the arguments assigned to valist */
	for (i = 0; i < num; i++) {
		holder = va_arg(valist, double);
		result += holder;
		printf("in loop holder: %lf\t",holder); // DBPRINT
		printf("in loop result: %lf\n",result); // DBPRINT
	}	
	/* clean memory reserved for valist */
	va_end(valist);	
	printf("out of loop holder: %lf\t",holder); // DBPRINT
	printf("out of loop result: %lf\t",result); // DBPRINT	
	printf("result: %.8f\t\n",(float)result); // DBPRINT
	return (float)result;
}
float eqseries_caps(int num, ...){
	printf("\nAdding Caps in Series\n");
	double result = 0.0, holder = 0.0;
	int i;		
	va_list valist;
	/* initialize valist for num number of arguments */
	va_start(valist, num);		
	/* access all the arguments assigned to valist */
	for (i = 0; i < num; i++) {
		holder = 1/va_arg(valist, double);
		result += holder;
		printf("in loop holder: %lf\t",holder); // DBPRINT
		printf("in loop result: %lf\n",result); // DBPRINT
	}	
	/* clean memory reserved for valist */
	va_end(valist);	
	printf("out of loop holder: %lf\t",holder); // DBPRINT
	printf("out of loop result: %lf\t",result); // DBPRINT	
	result = (1/result);
	printf("result: %.8f\t\n",(float)result); // DBPRINT
	return (float)result;
}

struct comp_1 setValues_LPF1(float cf, float bw, float ind, int ch){
	struct comp_1 result;
// First we initialize globals with user input	
	cntrFreqHz_1 = cf*MEGA;
	//~ printf("cntrFreqHz: %.4f MHz\n",cntrFreqHz); // DBPRINT
	bw3dB_kHz_1 = bw*KILO;
	//~ printf("bw3dB_Hz: %.4f kHz\n", bw3dB_Hz); // DBPRINT
	inductorValue_H_1 = ind/MEGA;
// Now we calculate values in the proper order
	//~ setKQValues_1(ch);
	//calcQbp(cntrFreqHz_1, bw3dB_kHz_1);
	//~ calcQ1(); calcQ2(); calcK12();
	printf("This is only a test of LPF1\n");
	//~ calcCNode2(cntrFreqHz_1, inductorValue_H_1);
	//~ result.C12 = calcC12();
	//~ result.C1 = calcC1();
	//~ result.C2 = calcC2();
	//~ result.R1 = calcR1();
	//~ result.R2 = calcR2();
	//~ result.L1 = inductorValue_H_1;
	//~ result.L2 = inductorValue_H_1;
	//~ printBPF1(result);
	//printf("\n");	
	eqparallel_caps(3,0.001,0.0012,0.0015);
	printf("\n");	
	eqseries_caps(3,0.001,0.0012,0.0015);
	return result;
}

/// Maintains the frq and res, but changes the ind to a user-defined 
/// value (wl), and returns the cap required to achieve these reqs
float modBPF_C_1(float wl, float r, float frq){
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

double derive(double (*f)(double), double x0) {
    const double delta = 1.0e-6; // or similar
    double x1 = x0 - delta;
    double x2 = x0 + delta;
    double y1 = f(x1);
    double y2 = f(x2);
    return (y2 - y1) / (x2 - x1);
}

// https://rosettacode.org/wiki/Numerical_integration#C
double int_leftrect(double from, double to, double n, double (*func)())
{
   double h = (to-from)/n;
   double sum = 0.0, x;
   for(x=from; x <= (to-h); x += h)
      sum += func(x);
   return h*sum;
}
 
double int_rightrect(double from, double to, double n, double (*func)())
{
   double h = (to-from)/n;
   double sum = 0.0, x;
   for(x=from; x <= (to-h); x += h)
     sum += func(x+h);
   return h*sum;
}
 
double int_midrect(double from, double to, double n, double (*func)())
{
   double h = (to-from)/n;
   double sum = 0.0, x;
   for(x=from; x <= (to-h); x += h)
     sum += func(x+h/2.0);
   return h*sum;
}
 
double int_trapezium(double from, double to, double n, double (*func)())
{
   double h = (to - from) / n;
   double sum = func(from) + func(to);
   int i;
   for(i = 1;i < n;i++)
       sum += 2.0*func(from + i * h);
   return  h * sum / 2.0;
}
 
double int_simpson(double from, double to, double n, double (*func)())
{
   double h = (to - from) / n;
   double sum1 = 0.0;
   double sum2 = 0.0;
   int i;
 
   double x;
 
   for(i = 0;i < n;i++)
      sum1 += func(from + h * i + h / 2.0);
 
   for(i = 1;i < n;i++)
      sum2 += func(from + h * i);
 
   return h / 6.0 * (func(from) + func(to) + 4.0 * sum1 + 2.0 * sum2);
}
//// call it as follows:
//// #include <math.h>

//// double der = derive(sin, 0.0);
//// printf("%lf\n", der); // should be around 1.0


int getUserInputBPF1(){
	int exitFlag = 0;	
	int ch;
	printf("Please choose from the following: \n");
	printf("1 for Butterworth Filter with 3 tuned circuits\n");
	printf("2 for Chebychev 0.1dB with 3 tuned circuits\n");
	printf("3 for Chebychev 0.5dB with 3 tuned circuits\n");
	printf("4 for Chebychev 1.0dB with 3 tuned circuits\n");
	printf("5 for Bessel with 3 tuned circuits\n");
	printf("10 for quit\n");
	scanf(" %i", &ch);
	
	switch (ch) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
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
	else getUserInputBPF1();
}
//
	//~ int main(int argc, char const *argv[]) {
	//~ getUserInputBPF2();
	//~ return 0;
 //~ }

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


/** 
Appendix 1
Tables of loss less Low Pass Filter normalised component values

Table A1. Normalised component values for Butterworth low pass filters
				G1			G2			G3			G4			G5			RL
N	RS 			Shunt C		Series L	Shunt C		Series L	Shunt C
1 	1.0000 		2.0000 														1.0000
2 	1.0000 		1.4142 		1.4142 											1.0000
3 	1.0000 		1.0000 		2.0000 		1.0000 								1.0000
4 	1.0000 		0.7654 		1.8478 		1.8478 		0.7654					1.0000
5 	1.0000 		0.6180 		1.6180 		2.0000 		1.6180		0.6180		1.0000


Table A2. Chebychev N=3 Normalised Component Values
Ripple  RS		Shunt C 	Series L 	Shunt C		RL		Voltage
dB				G1			G2			G3					Gain (dB)
0.1 	1.0000 	1.0316 		1.1474		1.0316 		1.0000 		0
0.2 	1.0000 	1.2276	 	1.1525 		1.2276 		1.0000 		0
0.5 	1.0000 	1.5963 		1.0967 		1.5963 		1.0000 		0
1.0 	1.0000 	2.0237 		0.9941 		2.0237 		1.0000 		0


Table A3. Chebychev N=4 Normalised Component Values
Ripple  RS		Series L 	Shunt C		Series L	Shunt C		RL		Voltage
dB				G1			G2			G3			G4					Gain (dB)
0.1 	1.0000 	1.1088 		1.3062 		1.7704		0.8181 		1.3554 		1.32
0.2 	1.0000 	1.3029 		1.2844 		1.9762 		0.8468 		1.5386 		1.87
0.5 	1.0000 	1.6704 		1.1925 		2.3662 		0.8419 		1.9841 		2.98
1.0 	1.0000 	2.0991 		1.0644 		2.8312 		0.7892 		2.6599 		4.25


Table A4. Chebychev N=5 Normalised Component Values
Ripple  RS		Series L 	Shunt C		Series L	Shunt C		Series L	RL		Voltage
dB				G1			G2			G3			G4			G5					Gain (dB)
0.1 	1.0000 	1.1468 		1.3712 		1.9750 		1.3712 		1.1468 		1.0000 		0
0.2 	1.0000 	1.3395 		1.3370 		2.1661 		1.3370 		1.3395 		1.0000 		0
0.5 	1.0000 	1.7058 		1.2296 		2.5409 		1.2296 		1.7058 		1.0000 		0
1.0 	1.0000	2.1350 		1.0911 		3.0010 		1.0911 		2.1350 		1.0000 		0

Table A5. Ratio of 3dB to ripple bandwidth
N 		f3dB / f ripple
3 		1.0949
4 		1.0530
5 		1.0338
**/

