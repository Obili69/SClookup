#ifndef SDlookup_h
#define SDlookup_h

#include "Arduino.h"
#include <SD.h>
#include <SPI.h>


bool readSDarray(float *arrayT, float *arrayR, String filename);
bool initSD(uint8_t MOSI, uint8_t MISO, uint8_t SCK, uint8_t CS);
float tempToU(float tempr);
float tempToU(float temp);
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
float tempToU(float tempr, float *Rarr, float *Tarr);
void processLine(String line, float *ptR, float *ptT);

#endif
