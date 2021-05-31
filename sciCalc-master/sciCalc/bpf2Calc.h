// bpf2Calc.h 
#ifndef BPF2CALC_H
#define BPF2CALC_H

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

#define PI        	3.14159265358979323846 // ad infinitum
#define DATA_SIZE 	1000
//#define OBP       2

float calcQbp(float cf, float bw);
void setKQValues(int choice);

float calcQ1(void);
float calcQ2(void);
float calcK12(void);
float calcCNode2(float cf, float ind);
float calcC12(void);
float calcC1(void);
float calcC2(void);
float calcR1(void);
float calcR2(void);
struct comp_2 computeValues_2(int ch);
struct comp_2 setValues_2(float cf, float bw, float ind, int ch);
struct comp_2 BBT_2 (struct comp_2 fin, float scale);
void printBPF2(struct comp_2 f1);
float calcBW_2 (float r, float c);

char * floatToStringBPF2(float f);
void storeDataInFileBPF2(float input);
int getUserInputBPF2(void);
void clearDataFromFileBPF2(void);

// 2 element filters
//~ Filter Type 			q1 		k12 	q2
//~ Butterworth 			1.414 	0.707 	1.414
//~ Chebychev 0.1dB ripple 	1.638 	0.711 	1.638
//~ Chebychev 0.5dB ripple 	1.950 	0.723 	1.950
//~ Chebychev 1dB ripple 	2.210 	0.739 	2.210
//~ Bessel 					0.5755 	0.900 	2.148

#define Q1BW      	1.414 // Butterworth k-q values
#define K12BW     	0.707
#define Q2BW      	1.414

#define Q1CH_01DB 	1.638 // Chebychev 0.1dB k-q values
#define K12CH_01DB 	0.711
#define Q2CH_01DB 	1.638

#define Q1CH_05DB 	1.95  // Chebychev 0.5dB k-q values
#define K12CH_05DB 	0.723
#define Q2CH_05DB 	1.95

#define Q1CH_1DB  	2.21  // Chebychev 1.0dB k-q values
#define K12CH_1DB 	0.729
#define Q2CH_1DB  	2.21

#define Q1BES     	0.5755 // Bessel k-q values
#define K12BES    	0.9
#define Q2BES     	2.148


#endif
