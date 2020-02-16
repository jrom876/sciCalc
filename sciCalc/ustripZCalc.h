// ustripZCalc.h //
// microstrip impedance calculator //
float calcZout(float h, float w, float t, float er);
float convertMilsToMM(float mil);
float convertMMToMils(float mm);
char * floatToStringUS(float f);
void storeDataInFileUS(float input);
int getUserInputUS(void);
void clearDataFromFileUS(void);
