//	Package:	sciCalc
//	File:		primeNumsCalc.c
//	Purpose:	Generate Prime Numbers between two limits
//	Author:		jrom876

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

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "primeNumsCalc.h"
#define DATA_SIZE 1000

float tempf;

int genPrimeNums()
{
  int n1, n2, i, j, flag;
  printf("Enter first number: ");
  scanf("%d", &n1);
  
  printf("Enter second number: ");
  scanf("%d", &n2);
  
  printf("Prime numbers between %d and %d are: ", n1, n2);
  for(i=n1+1; i<n2; ++i)
  {
      flag=0;
      for(j=2; j<=i/2; ++j)
      {
        if(i%j==0)
        {
          flag=1;
          break;
        }
      }
      if(flag==0)
        printf("%d ",i);
  }
  return 0;
}


int getUserInputPN(){
	int exitFlag = 0;
	float dkm, fg;
	int var;
	printf("Please choose from the following: \n");
	printf("1 To generate all Prime Numbers between two numbers\n");
	printf("8 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			genPrimeNums();
			break;
		case 2:
			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:
			
			break;
		case 7:
			
			break;
		case 8:
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exitFlag = 1;
			break;
	}
	if(0 != exitFlag) return 0;
	getUserInputPN();
}

//~ int main(int argc, char const *argv[]) {
	//~ getUserInputPN();
	//~ genPrimeNums();
	//~ return 0;
//~ }
