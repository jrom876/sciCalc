//	Package:	sciCalc
//	File:		psrr.c 
//	Purpose:	Calculate Power Supply Rejection Ratio for LDOs
//	Author:		jrom876

/**
	Copyright (C) 2024 
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

/**
 * https://en.wikipedia.org/wiki/Power_supply_rejection_ratio
 * https://eng.libretexts.org/Bookshelves/Electrical_Engineering/Electronics/Operational_Amplifiers_and_Linear_Integrated_Circuits_-_Theory_and_Application_(Fiore)/05%3A_Practical_Limitations_of_Op_Amp_Circuits/5.7%3A_CMRR_and_PSRR
 * 
 * PSRR[dB] = 10log10((dVsupply^2 * Av^2)/(dVout^2))dB
 * 
 * For example: an amplifier with a PSRR of 100 dB in a circuit 
 * to give 40 dB closed-loop gain would allow about 1 millivolt of 
 * power supply ripple to be superimposed on the output for 
 * every 1 volt of ripple in the supply. This is because

    100dB − 40dB = 60dB 

And since that's 60 dB of rejection, the sign is negative so:

    1V ⋅ 10^(-60/20) = 0.001V = 1mV 
    
 * **/

#include <math.h>
#include <check.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "psrr.h"

/// STANDARD DEFINITIONS FOR PROJECT SCICALC 
#define PI	3.14159265358979323846 // ad infinitum sine repeto
#define LIGHT_SPEED		299792458 // meters per second
#define DATA_SIZE 1000
#define DELTA 1.0e-6
#define KILO 1000
#define MEGA 1000000
#define GIGA 1000000000
#define TERA 1000000000000 

float psrrdB(float Vripplein, float Vrippleout) {
	float result = 20*log10(Vripplein/Vrippleout);
	printf("psrrdB result = %0.3f dB\n", result);
	return result;
}

float psrr(float dVsupp, float VgaindB, float dVout) {
	float vgdB = (pow(10, (VgaindB/10)));
	printf("vgdB = %0.3f\n", vgdB);
	float result = 10*log10(((dVsupp*dVsupp)*(vgdB*vgdB))/(dVout*dVout));
	//~ float result = 10*log10(((dVsupp*dVsupp)*(VgaindB*VgaindB))/(dVout*dVout));
	printf("pssr result = %0.6f dB\n", result);
	return result;
}

float ripple (float psrr, float clGain) {
	clGain = 10*log10(clGain);
	float exponent = -1 * (psrr - clGain);
	float result = pow(10,(exponent/20));
	printf("exponent = %0.3f\n", exponent);
	printf("ripple result = %0.6f\n", result);
	return result;
}

int main(int argc, char const *argv[]) {
	//~ psrr(5, 10, 3.3);
	ripple (psrr(5, 40, 3.3), 40);
	//~ ripple (100, 40);
	return 0;
}
