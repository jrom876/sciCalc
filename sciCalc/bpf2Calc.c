// bpf2Calc.c //

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bpf2Calc.h"
#define TXT_FILE "bpf2Data.txt"

//  inductor    Q1          Q2          K12BW        CNode
float lBW,      q1BW,       q2BW,       k12BW,      cnodeBW;    // Butterworth
float lCH_01,   q1CH_01,    q2CH_01,    k12CH_01,   cnodeCH_01; // Chebychev 0.1dB
float lCH_05,   q1CH_05,    q2CH_05,    k12CH_05,   cnodeCH_05; // Chebychev 0.5dB
float lCH_1,    q1CH_1,     q2CH_1,     k12CH_1,    cnodeCH_1;  // Chebychev 1.0dB
float lBES,     q1BES,      q2BES,      k12BES,     cnodeBES;   // Bessel

float cntrFreqHz = 0;
float bw3dB_Hz = 0;
float inductorValue_H = 0;
int choice = 0;

float l, c12, c1, c2, r1, r2;
float Obp = 0;
float = q1, q2, k12;
float tempf;
//int exitFlag = 0;

float calcObp(float cf, float bw){
  Obp = (cf/bw);
  return Obp;
}

void setKQValues(int choice){
  switch(choice){
    case 1:
      q1 = Q1BW;
      k12 = K12BW;
      q2 = Q2BW;
      break;
    case 2:
      q1 = Q1CH_01DB;
      k12 = K12CH_01DB;
      q2 = Q2CH_01DB;
      break;
    case 3:
      q1 = Q1CH_05DB;
      k12 = K12CH_05DB;
      q2 = Q2CH_05DB;
      break;
    case 4:
      q1 = Q1CH_1DB;
      k12 = K12CH_1DB;
      q2 = Q2CH_1DB;
      break;
    case 5:
      q1 = Q1BES;
      k12 = K12BES;
      q2 = Q2BES;
      break;
    default:
      break;
  }
}

float calcQ2(void){
  float result = Obp*q2;
  float q2;
  // printf("Q2 = %f\n",result);
  // tempf = result;
  return result;
}

float calcK12(void){
  float result = k12/Obp;
  // printf("K12 = %f\n",result);
  // tempf = result;
  return result;
}

float calcCNode(float cf, float ind){
  float result = 1/(((2*PI*cf)*(2*PI*cf))*ind);
  // printf("CNode = %f\n",result);
  // tempf = result;
  return result;
}

/// Component calculations with 2 tuned circuits ///
float calcC12(void){
  float result = (calcK12()*calcCNode());
  printf("C12 = %f\n",result);
  tempf = result;
  return result;
}

float calcC1(void){
  float result = (calcCNode() - calcC12());
  printf("C1 = %f\n",result);
  tempf = result;
  return result;
}

float calcC2(float q){
  float result = (calcCNode() - calcC12());
  printf("C2 = %f\n",result);
  tempf = result;
  return result;
}

float calcR1(float f, float ind){
  float result = ((2*PI*f*ind*(calcQ1())));
  printf("R1 = %f\n",result);
  tempf = result;
  return result;
}

float calcR2(float f, float ind){
  float result = ((2*PI*f*ind*(calcQ2())));
  printf("R2 = %f\n",result);
  tempf = result;
  return result;
}
/////////////////////////////////////////////

void computeValues(float cf, float bw, float ind, int ch){
  calcObp(cf, bw);
  setValues(ch);
  calcQ1();
  calcQ2();
  calcK12();
  calcCNode(cf, ind);
  calcC12();
  calcC1();
  calcC2();
  calcR1();
  calcR2();
}

/////////////////////////////////////////////
//////////// File and IO Handling ///////////
/////////////////////////////////////////////
/// The following 4 functions create a .txt
/// file if it does not already exist; convert an
/// input float to a string and append the .txt
/// file with that string on a new line;
// clear all data from the .txt file if requested;
// and allow users to select menu choices.
////////////////////////////////////////////
char * floatToStringBPF2(float f){
		char buffer[DATA_SIZE];
    return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileBPF2(float input){
	FILE * fPtr = fopen(TXT_FILE, "a");
	char * data = floatToStringBPF2(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  fclose(fPtr);
}

void clearDataFromFileBPF2(){
  FILE * fPtr = fopen(TXT_FILE, "w");
  fclose(fPtr);
}

int getUserInputBPF2(){
	int exitFlag = 0;
	//float q1,q2,k12,cnode;
  float bw,cf,ind;
	int var;
	printf("Please choose from the following: \n");
	printf("1 for Butterworth Filter with 2 tuned circuits\n");
	printf("2 for Chebychev 0.1dB with 2 tuned circuits\n");
	printf("3 for Chebychev 0.5dB with 2 tuned circuits\n");
	printf("4 for Chebychev 1.0dB with 2 tuned circuits\n");
	printf("5 for Bessel with 2 tuned circuits\n");
	printf("6 for Butterworth Filter with 3 tuned circuits\n");
	printf("7 for Chebychev 0.1dB with 3 tuned circuits\n");
	printf("8 for Chebychev 0.5dB with 3 tuned circuits\n");
	printf("9 for Chebychev 1.0dB with 3 tuned circuits\n");
	printf("10 for Bessel with 3 tuned circuits\n");
  printf("11 to clear bpfData.txt\n");
	printf("12 for store to file\n");
	printf("13 for quit\n");
	scanf(" %i", &var);
	switch (var) {
		case 1:
			printf("Please enter Center Frequency in Hz: \n");
			scanf(" %f", &cf);
  		printf("Please enter 3dB bandwidth n Hz: \n");
  		scanf(" %f", &bw);
			printf("Please enter inductor value in Henries: \n");
			scanf(" %f", &ind);
			ctof(c);
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
			clearDataFromFileBPF2();
			printf("bpfData.txt is now cleared of all data");
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileBPF2(tempf);
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
	getUserInputBPF2();
}
//
// int main(int argc, char const *argv[]) {
//   calcAntGain(0.92, 100, 25000000);
//   printLambda(2400000000);
//   getUserInputBPF2();
//   return 0;
// }
