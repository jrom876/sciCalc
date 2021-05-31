
//	Package:	sciCalc
//	File:		bpf2Calc.c 
//	Purpose:	Calculate Band Pass Filter component values for 
//				Butterworth, Chebychev, and Bessel filters with 
//				2 or 3 resonant circuits using the K-Q Method
//	Author:		jrom876

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

// https://stackoverflow.com/questions/6418807/how-to-work-with-complex-numbers-in-c
// https://www.antenna-theory.com/basics/impedance.php
// https://en.wikipedia.org/wiki/Complex_conjugate
// https://www.techiedelight.com/return-multiple-values-function-c/

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//~ #include <complex.h>    /* Standard Library of Complex Numbers */
#include "complexCalc.h"

struct complexNum sum_complex (struct complexNum z1, struct complexNum z2) {
	struct complexNum  sum;
	sum.real = z1.real + z2.real;
	sum.imag = z1.imag + z2.imag;
    printf("The sum_complex(): Z1 + Z2 = %.2f %+.2fi\n", sum.real, sum.imag);
    return sum;
}

struct complexNum difference_complex (struct complexNum z1, struct complexNum z2) {	
	struct complexNum  difference;
	difference.real = z1.real - z2.real;
	difference.imag = z1.imag - z2.imag;
    printf("The difference: Z1 - Z2 = %.2f %+.2fi\n", difference.real, difference.imag);
    return difference;
}

struct complexNum product_complex (struct complexNum z1, struct complexNum z2) {
	struct complexNum  product;
	product.real = z1.real * z2.real;
	product.imag = z1.imag * z2.imag;
    printf("The product(): Z1 + Z2 = %.2f %+.2fi\n", product.real, product.imag);
    return product;
}

struct complexNum quotient_complex (struct complexNum z1, struct complexNum z2) {
	struct complexNum  quotient;
	if (z2.real == 0 && z2.imag == 0) {
		printf("Division by zero is not possible\n");
		exit(0);
	} 
	else {
		quotient.real = z1.real / z2.real;
		quotient.imag = z1.imag / z2.imag;
		printf("The quotient: Z1 + Z2 = %.2f %+.2fi\n", quotient.real, quotient.imag);
	}
    return quotient;
}

struct complexNum conjugate_complex (struct complexNum z1) {
	struct complexNum conjugate;
	conjugate.real = z1.real;
	conjugate.imag = (z1.imag) * -1;
    printf("The conjugate of Z1 = %.2f %+.2fi\n", conjugate.real, conjugate.imag);
    return conjugate;
}

struct complexNumArray getNums(void) {
	float z1real, z1imag, z2real, z2imag;
	struct complexNum a, b;
	struct complexNumArray result;
	printf("Please enter z1real: \n");
	scanf(" %f", &z1real);
	a.real = z1real;			
	printf("Please enter z1imag: \n");
	scanf(" %f", &z1imag);
	a.imag = z1imag;			
	printf("Please enter z2real: \n");
	scanf(" %f", &z2real);
	b.real = z2real;			
	printf("Please enter z2imag: \n");
	scanf(" %f", &z2imag);			
	b.imag = z2imag;
	result.cnum1 = a; result.cnum2 = b;
	return result;	
}


int getUserInputCC () {
	int exitFlag = 0;
	float z1real, z1imag, z2real, z2imag;
	struct complexNum z1, z2;
	int var;
	printf("Please choose from the following: \n");
	printf("1 for sum_complex\n");
	printf("2 for difference_complex\n");
	printf("3 for product_complex\n");
	printf("4 for quotient_complex\n");
	printf("5 for conjugate_complex\n");
	printf("8 for quit\n");
	scanf(" %i", &var);

	switch (var) {
		case 1:	
			printf("Please enter z1real: \n");
			scanf(" %f", &z1real);
			z1.real = z1real;			
			printf("Please enter z1imag: \n");
			scanf(" %f", &z1imag);
			z1.imag = z1imag;			
			printf("Please enter z2real: \n");
			scanf(" %f", &z2real);
			z2.real = z2real;			
			printf("Please enter z2imag: \n");
			scanf(" %f", &z2imag);			
			z2.imag = z2imag;		
			sum_complex(z1, z2);
			break;
		case 2:
			printf("Please enter z1real: \n");
			scanf(" %f", &z1real);
			z1.real = z1real;			
			printf("Please enter z1imag: \n");
			scanf(" %f", &z1imag);
			z1.imag = z1imag;			
			printf("Please enter z2real: \n");
			scanf(" %f", &z2real);
			z2.real = z2real;			
			printf("Please enter z2imag: \n");
			scanf(" %f", &z2imag);			
			z2.imag = z2imag;	
			difference_complex(z1, z2);
			break;
		case 3:
			printf("Please enter z1real: \n");
			scanf(" %f", &z1real);
			z1.real = z1real;			
			printf("Please enter z1imag: \n");
			scanf(" %f", &z1imag);
			z1.imag = z1imag;			
			printf("Please enter z2real: \n");
			scanf(" %f", &z2real);
			z2.real = z2real;			
			printf("Please enter z2imag: \n");
			scanf(" %f", &z2imag);			
			z2.imag = z2imag;	
			product_complex(z1, z2);
			break;
		case 4:
			printf("Please enter z1real: \n");
			scanf(" %f", &z1real);
			z1.real = z1real;			
			printf("Please enter z1imag: \n");
			scanf(" %f", &z1imag);
			z1.imag = z1imag;			
			printf("Please enter z2real: \n");
			scanf(" %f", &z2real);
			z2.real = z2real;			
			printf("Please enter z2imag: \n");
			scanf(" %f", &z2imag);			
			z2.imag = z2imag;	
			quotient_complex(z1, z2);
			break;
		case 5:
			printf("Please enter z1real: \n");
			scanf(" %f", &z1real);
			z1.real = z1real;			
			printf("Please enter z1imag: \n");
			scanf(" %f", &z1imag);
			z1.imag = z1imag;	
			conjugate_complex(z1);
			break;
		case 8:
			printf("Quitting\n");
			exitFlag = 1;
			break;
		default:
			printf("Wrong choice. Now exiting.\n");
			exit(0);
			exitFlag = 1;
			break;
	}
	if(0 != exitFlag) return 0;
	getUserInputCC();
}

//~ int main() {

    //~ double complex z1 = 1.0 + 3.0 * I;
    //~ double complex z2 = 1.0 - 4.0 * I;

	//~ sum_complex(z1,z2);
	//~ difference_complex(z1,z2);
	//~ product_complex(z1,z2);
	//~ quotient_complex(z1,z2);
	//~ conjugate_complex(z1,z2);
	
    //~ printf("Working with complex numbers:\n\v");

    //~ printf("Starting values: Z1 = %.2f + %.2fi\tZ2 = %.2f %+.2fi\n", creal(z1), cimag(z1), creal(z2), cimag(z2));

    //~ double complex sum = z1 + z2;
    //~ printf("The sum: Z1 + Z2 = %.2f %+.2fi\n", creal(sum), cimag(sum));

    //~ double complex difference = z1 - z2;
    //~ printf("The difference: Z1 - Z2 = %.2f %+.2fi\n", creal(difference), cimag(difference));

    //~ double complex product = z1 * z2;
    //~ printf("The product: Z1 x Z2 = %.2f %+.2fi\n", creal(product), cimag(product));

    //~ double complex quotient = z1 / z2;
    //~ printf("The quotient: Z1 / Z2 = %.2f %+.2fi\n", creal(quotient), cimag(quotient));

    //~ double complex conjugate = conj(z1);
    //~ printf("The conjugate of Z1 = %.2f %+.2fi\n", creal(conjugate), cimag(conjugate));

    //~ return 0;
//~ }
