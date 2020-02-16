// bpf2Calc.h //
// #ifndef BPF2CALC_H
// #define BPF2CALC_H

char * floatToStringBPF(float f);
void storeDataInFileBPF(float input);
int getUserInputBPF(void);
void clearDataFromFileBPF(void);

#define PI        3.14159265358979323846
#define DATA_SIZE 1000
//#define OBP       2

#define Q1BW      1.414 // Butterworth k-q values
#define K12BW     0.707
#define Q2BW      1.414

#define Q1CH_01DB 1.638 // Chebychev 0.1dB k-q values
#define K12CH_01DB 0.711
#define Q2CH_01DB 1.638

#define Q1CH_05DB 1.95  // Chebychev 0.5dB k-q values
#define K12CH_05DB 0.723
#define Q2CH_05DB 1.95

#define Q1CH_1DB  2.21  // Chebychev 1.0dB k-q values
#define K12CH_1DB 0.729
#define Q2CH_1DB  2.21

#define Q1BES     0.5755 // Bessel k-q values
#define K12BES    0.9
#define Q2BES     2.148
// #endif
