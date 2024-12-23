// linkBudgetCalc.c 
#ifndef LINKBUDGETCALC_H
#define LINKBUDGETCALC_H

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

char * floatToStringLB(float f);
void storeDataInFileLB(float input);
int getUserInputLB(void);
void clearDataFromFileLB(void);

float twoWayPathLossDB(float freq, float dist, float rxs);
float rxPwrWatts(float Pt, float GtdB, float GrdB, float R, float freq);
float rxPwrDBM(float PtdBm, float GtdB, float GrdB, float R, float freq);

float getFresnelRadius( float freqG, float distKM );
float getPLF(float theta);
float antGainFromFreq(float eff, float ap, float fr);
float getLambda(float freq);
float getFreq(float lam);

#endif
