// bpf2Calc.c //

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bpf2Calc.h"
#define PI        3.14159265358979323846
#define DATA_SIZE 1000
//#define OBP       2

#define Q1BW      1 // Butterworth k-q values
#define K12BW     0.707
#define K23BW     0.707
#define Q3BW      1

#define Q1CH_01DB 1.433 // Chebychev 0.1dB k-q values
#define K12CH_01DB 0.662
#define K23CH_01DB 0.662
#define Q3CH_01DB 1.433

#define Q1CH_05DB 1.864  // Chebychev 0.5dB k-q values
#define K12CH_05DB 0.647
#define K23CH_05DB 0.647
#define Q3CH_05DB 1.864

#define Q1CH_1DB  2.21  // Chebychev 1.0dB k-q values
#define K12CH_1DB 0.638
#define K23CH_1DB 0.638
#define Q3CH_1DB  2.21

#define Q1BES     0.337 // Bessel k-q values
#define K12BES    1.748
#define K23BES    0.684
#define Q3BES     2.203

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

float l_2, c12_2, c1_2, c2_2, r1_2, r2_2; // 2 resonant circuits
float l_3, c12_3, c23_3, c1_3, c2_3, c3_3, r1_3, r2_3; // 3 resonant circuits
float Obp = 0;
float tempf;
//int exitFlag = 0;

float calcObp(){
  float Obp = (cntrFreqHz/bw3dB);
  return Obp;
}

float calcQ1(float q1){
  float result = Obp*q1;
  // printf("Q1 = %f\n",result);
  // tempf = result;
  return result;
}

float calcQ2(float q2){
  float result = Obp*q2;
  // printf("Q2 = %f\n",result);
  // tempf = result;
  return result;
}

float calcK12(float k12){
  float result = k12/Obp;
  // printf("K12 = %f\n",result);
  // tempf = result;
  return result;
}

float calcCNode(float frq, float ind){
  float result = 1/(((2*PI*frq)*(2*PI*frq))*ind);
  // printf("CNode = %f\n",result);
  // tempf = result;
  return result;
}

/// Component calculations with 2 tuned circuits ///
float calcC12_2(float q){
  float result = (calcK12()*calcCNode());
  printf("C12 = %f\n",result);
  tempf = result;
  return result;
}

float calcC1_2(float q){
  float result = (calcCNode() - calcQ1());
  printf("C1 = %f\n",result);
  tempf = result;
  return result;
}

float calcC2_2(float q){
  float result = (calcCNode() - calcC12());
  printf("C2 = %f\n",result);
  tempf = result;
  return result;
}

float calcR1_2(float f, float ind){
  float result = ((2*PI*f*ind*(calcQ1())));
  printf("R1 = %f\n",result);
  tempf = result;
  return result;
}

float calcR2_2(float f, float ind){
  float result = ((2*PI*f*ind*(calcQ2())));
  printf("R2 = %f\n",result);
  tempf = result;
  return result;
}
/////////////////////////////////////////////

float calcBW_2(){

}


/////////////////////////////////////////////
//////////// File and IO Handling ///////////
/////////////////////////////////////////////
/// The following 3 functions create a .txt
/// file if it does not already exist; convert an
/// input float to a string and append the .txt
/// file with that string on a new line; and
// clear all data from the .txt file if requested.
////////////////////////////////////////////
char * floatToStringBPF(float f){
		char buffer[DATA_SIZE];
    return gcvt(f,DATA_SIZE,buffer);
}

void storeDataInFileBPF(float input){
	FILE * fPtr = fopen("bpfData.txt", "a");
	char * data = floatToStringBPF(input);
	//fgets(data, DATA_SIZE, fPtr);
	fputs(data, fPtr);
	fputs("\n", fPtr);
  fclose(fPtr);
}

void clearDataFromFileBPF(){
  FILE * fPtr = fopen("bpfData.txt", "w");
  fclose(fPtr);
}

int getUserInputBPF(){
	int exitFlag = 0;
	float q1,q2,k12, f, k;
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
			printf("Please enter C temp: \n");
			scanf(" %f", &c);
  		printf("Please enter C temp: \n");
  		scanf(" %f", &c);
			printf("Please enter C temp: \n");
			scanf(" %f", &c);
			ctof(c);
			break;
		case 2:
			printf("Please enter C temp: \n");
			scanf(" %f", &c);
			ctok(c);
			break;
		case 3:
			printf("Please enter F temp: \n");
			scanf(" %f", &c);
			ftoc(c);
			break;
		case 4:
			printf("Please enter F temp: \n");
			scanf(" %f", &c);
			ftok(c);
			break;
		case 5:
			printf("Please enter K temp: \n");
			scanf(" %f", &c);
			ktoc(c);
			break;
		case 6:
			printf("Please enter K temp: \n");
			scanf(" %f", &c);
			ktof(c);
			break;
		case 7:
			printf("Writing data to file\n");
			storeDataInFileBPF(tempf);
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
	getUserInputBPF();
}
