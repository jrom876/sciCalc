// inrushICalc.h 
#ifndef INRUSHCALC_H
#define INRUSHCALC_H

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

////////// Functions ///////////

/// Set required values ///
struct ICL setValuesIC(float va, float fr, float io, float vo, float nc, float zs, float er);
/// Calculate Capacitive Reactance ///
float calcXC(void);
/// Calculate Minimum Resistance
float calcMinRes(void);
//////// Calculate Max Inrush Current ////////
float calcMaxI(void);
float calcMaxI_noLoad(void);
/// Calculate Steady State Current ///
float calcSSI(void);

float setSSI(void);

float calcTotEnergy(void);

void printICL(struct ICL f1);
char * floatToStringIC(float f);
void storeDataInFileIC(float input);
void clearDataFromFileIC();
int getUserInputIC();


#endif
