// inrushICalc.h 
#ifndef INRUSHCALC_H
#define INRUSHCALC_H

////////// Functions ///////////

/// Set required values ///
void setValuesIC(void);
/// Calculate Capacitive Reactance ///
float calcXC();
/// Calculate Minimum Resistance
float calcMinRes();
//////// Calculate Max Inrush Current ////////
float calcMaxI();
/// Calculate Steady State Current ///
float calcSSI();

float setSSI();
char * floatToStringIC(float f);
void storeDataInFileIC(float input);
void clearDataFromFileIC();
int getUserInputIC();


#endif
