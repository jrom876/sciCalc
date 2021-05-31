// vswrCalc.h 
#ifndef VSWRCALC_H
#define VSWRCALC_H

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

char * floatToStringVSWR(float f);
void storeDataInFileVSWR(float input);
int getUserInputVSWR(void);
float calcVSWRVoltageVSWR(float, float);
float calcVSWRGammaVSWR(float);
float calcVSWRPwrVSWR(float, float);

#endif
