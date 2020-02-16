// antennaGainCalc.h //

char * floatToStringAG(float f);
void storeDataInFileAG(float input);
int getUserInputAG(void);
void clearDataFromFileAG(void);
float calcAntGain(float et, float ap, float fr);
float calcLambda(float freq);
