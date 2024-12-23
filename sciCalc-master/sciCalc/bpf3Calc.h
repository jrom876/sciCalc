// bpf3Calc.h //
#ifndef BPF3CALC_H
#define BPF3CALC_H

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

#define PI        	3.14159265358979323846 // ad infinitum
#define DATA_SIZE 	1000

/**
3 element filters
Filter Type 		q1 	k12 	k23 	q3
Butterworth 		1 	0.707 	0.707 	1
Chebychev 0.1dB ripple 	1.433 	0.662 	0.662 	1.433
Chebychev 0.5dB ripple 	1.864 	0.647 	0.647 	1.864
Chebychev 1dB ripple 	2.210 	0.638 	0.638 	2.210
Bessel 			0.337 	1.748 	0.684 	2.203

Ratio of 3dB to ripple bandwidth
N 		f3dB / f ripple
3 		1.0949
4 		1.0530
5 		1.0338
**/

#define Q1BW_3    	1.0 // Butterworth k-q values
#define K12BW_3     	0.707
#define K23BW_3     	0.707
#define Q3BW_3      	1.0

#define Q1CH_01DB_3 	1.433 // Chebychev 0.1dB k-q values
#define K12CH_01DB_3 	0.662
#define K23CH_01DB_3 	0.662
#define Q3CH_01DB_3	1.433

#define Q1CH_05DB_3 	1.864  // Chebychev 0.5dB k-q values
#define K12CH_05DB_3 	0.647
#define K23CH_05DB_3 	0.647
#define Q3CH_05DB_3 	1.864

#define Q1CH_1DB_3  	2.21  // Chebychev 1.0dB k-q values
#define K12CH_1DB_3 	0.638
#define K23CH_1DB_3 	0.638
#define Q3CH_1DB_3  	2.21

#define Q1BES_3     	0.337 // Bessel k-q values
#define K12BES_3    	1.748
#define K23BES_3    	0.684
#define Q3BES_3     	2.203

#define f3dB2rbw_3	1.0949 // Ratio of 3dB to ripple bandwidth
#define f3dB2rbw_4	1.0530 // for 3, 4, and 5 resonant circuits
#define f3dB2rbw_5	1.0338

//#define OBP       	2
float calcQbp_3(float cf, float bw);
void setKQValues_3(int choice);

float calcQ1_3(void);
float calcQ3_3(void);
float calcK12_3(void);
float calcK23_3(void);
float calcCNode3(float cf, float ind);
float calcC12_3(void);
float calcC23_3(void);
float calcC1_3(void);
float calcC2_3(void);
float calcC3_3(void);
float calcR1_3(void);
float calcR2_3(void);
struct comp_3  computeValues_3(int ch);
struct comp_3 setValues_3 (float cf, float bw, float ind, int ch);
struct comp_3 NT_3 (struct comp_3 fin, float scale);
float calcBW_3 (float r, float c);
float modBPF_C_3(float wl, float r, float frq);

void printBPF3(struct comp_3 f1);
char * floatToStringBPF2(float f);
void storeDataInFileBPF2(float input);
int getUserInputBPF3(void);
void clearDataFromFileBPF2(void);

#endif
