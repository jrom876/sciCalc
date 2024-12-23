// bpf1Calc.h //
#ifndef BPF1CALC_H
#define BPF1CALC_H

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

struct comp_1 {
	float R1;
	float R2;
	float C1;
	float C2;
	float L1;
};

struct lpf_1 {
	float R1;
	float R2;
	float C1;
	float C2;
	float L1;
};

//#define OBP       2
float calcQbp_1(float cf, float bw);
//~ void setKQValues_1(int ch);

float calcBW_1 (float r, float c);
float modBPF_C_1(float wl, float r, float frq);

float eqparallel_caps( int num, ...);
float eqseries_caps(int num, ...);

double derive(double (*f)(double), double x0);
double int_leftrect(double from, double to, double n, double (*func)());
double int_rightrect(double from, double to, double n, double (*func)());
double int_midrect(double from, double to, double n, double (*func)());
double int_trapezium(double from, double to, double n, double (*func)()); 
double int_simpson(double from, double to, double n, double (*func)());

void printBPF1(struct comp_1 f1);
int getUserInputBPF1(void);



#endif
