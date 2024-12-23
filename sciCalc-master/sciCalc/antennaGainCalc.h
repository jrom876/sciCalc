// antennaGainCalc.h 
#ifndef ANTENNAGAINCALC_H
#define ANTENNAGAINCALC_H

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

char * floatToStringAG(float f);
void storeDataInFileAG(float input);
int getUserInputAG(void);
void clearDataFromFileAG(void);
float calcAntGainFromFreq(float et, float ap, float fr);
float calcLambda(float freq);
float calcAntGainFromLambda(float et, float ap, float lam);
float calcFreq(float lam);

#endif
