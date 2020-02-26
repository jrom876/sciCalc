// antennaGainCalc.c //

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "antennaGainCalc.h"
#define PI            3.14159265358979323846
#define LIGHT_SPEED   299792458 // meters per second
#define DATA_SIZE     1000
#define TXT_FILE "antennaGainData.txt"

// BN: Lambda always means wavelength(m), and eta
// always means % efficiency in this program

float eta       = 0; // antenna efficiency - %
float lambda    = 0; // wavelength - meters
float aperature = 0; // antenna aperature - meters^2
float frequency = 0;
float gain      = 0;
float tempf;
//int exitFlag = 0;


float calcAntGain(float et, float ap, float fr){
  float lam = calcLambda(fr);
  float result = ((10*log10((et*4*PI*ap)/(lam*lam))));
  printf("Parabolc Antenna gain = %f\n",result);
  eta = et; aperature = ap; frequency = fr;
  gain = result; tempf = result;
  return result;
}

float calcLambda(float freq){
  float clambda = LIGHT_SPEED/freq;
  printf("wavelength = %f \n",clambda);
  tempf = clambda;
  return clambda;
}

void printLambda(){
  //lambda = LIGHT_SPEED/frequency;
  printf("wavelength = %f meters\n",lambda);
}

////////////////////////////////////////////
/// The following 3 functions create a .txt
/// file if it does not already exist; convert an
/// input float to a string and append the .txt
/// file with that string on a new line; and
// clear all data from the .txt file if requested.
////////////////////////////////////////////
char * floatToStringAG(float f){
	    char buffer[DATA_SIZE];
    	return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileAG(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringAG(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  	fclose(fPtr);
}

void clearDataFromFileAG(){
  FILE * fPtr = fopen(TXT_FILE, "w");
  fclose(fPtr);
}
//////////////////////////////////////
//////////////////////////////////////
int getUserInputAG(){
	int exitFlag = 0;
	float e, a, f, l;
	int var;
	printf("Please choose from the following: \n");
	printf("1 for calcAntGain()\n");
	printf("2 for calcLambda(freq)\n");
	// printf("3 for ftoc\n");
	// printf("4 for ftok\n");
	// printf("5 for ktoc\n");
	printf("6 for clear data from file\n");
	printf("7 for store to file\n");
	printf("8 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
		        printf("Please enter efficiency: \n");
			scanf(" %f", &e);
		        printf("Please enter aperature area in m^2: \n");
		        scanf(" %f", &a);
		        printf("Please enter frequency in Hz: \n");
		        scanf(" %f", &f);
			calcAntGain(e, a, f);
			break;
		case 2:
			printf("Calculating Lambda\n");
			printf("Please enter frequency: \n");
			scanf(" %f", &f);
			calcLambda(f);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			clearDataFromFileAG();
      			printf("antennaGainData.txt is now cleared of all data");
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileAG(tempf);
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
	getUserInputAG();
}

//
// int main(int argc, char const *argv[]) {
//   calcAntGain(0.92, 100, 25000000);
//   printLambda(2400000000);
//   getUserInputAG();
//   return 0;
// }
