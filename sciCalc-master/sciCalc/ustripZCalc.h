// ustripZCalc.h
// microstrip impedance calculator
#ifndef USTRIPZCALC_H
#define USTRIPZCALC_H

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

struct USZ {
	float Eeff; // effective dielectric constant
	float Zout; // characteristic impedance
};

//~ float calcZout(float h, float w, float t, float er);
float convertMilsToMM(float mil);
float convertMMToMils(float mm);
float calcZoutMils(float er,float w, float t, float h);
float calcZoutMM(float er,float wm, float tm, float hm);

float calcZEff(float er, float w, float h);
float zoutMM(float w, float h);	
void printUS(struct USZ f1);
struct USZ setValuesUS(float er, float w, float h);
struct USZ putValuesUS(float er, float w, float th, float h);

char * floatToStringUS(float f);
void storeDataInFileUS(float input);
int getUserInputUS(void);
void clearDataFromFileUS(void);

#endif
