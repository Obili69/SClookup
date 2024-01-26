#include "Arduino.h"
#include <SD.h>
#include <SPI.h>

#define maxReading 400 // Maximum number of readings
int indey = 0;
File myFile;

float tempToU(float tempr, float *Rarr, float *Tarr){
  int minTemp = *(Tarr +1);
  int maxTemp = *(Rarr +1);
  float step = *Rarr;
  float resisstance = 0;
  int ind = 0;
  if(minTemp <= tempr && tempr <= maxTemp){
    if(minTemp < 0){
      ind = ((0 - minTemp)/step)+(tempr/step)+2;
    }
    else{
      ind = (minTemp/step)+(tempr/step)+2;
    }
    resisstance = mapfloat(tempr,*(Tarr + ind),*(Tarr + 1 + ind),*(Rarr + ind),*(Rarr + 1 + ind));
    
    return  (5.0 - 5000*((5.0)/((((1000+resisstance)*251000)/((1000+resisstance)+251000)) + 5000)))/(1000+resisstance)*resisstance;
  }
  else{
    return 0;
  }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

bool initSD(uint8_t MOSI, uint8_t MISO, uint8_t SCK, uint8_t CS){
  SPI.begin(SCK, MISO, MOSI, CS);
  if(!SD.begin(CS)){
    Serial.println("SD no worki");
  }
  return 0;
}

bool readSDarray(float *arrayT, float *arrayR, String filename){
  
  myFile = SD.open(filename); // Update with your file name
  if (myFile) {
    Serial.println(filename);
    String line = "";
    while (myFile.available() && indey < maxReading) {
      char ch = myFile.read();
      if (ch == '\n' || ch == '\r') {
        if (line.length() > 0) {
          processLine(line, arrayR, arrayT);
          line = "";
        }
      } 
      else {
        line += ch;
      }
    }
    // Process the last line if the file doesn't end with a newline
    if (line.length() > 0) {
      processLine(line, &arrayR[0], &arrayT[0]);
    }
    myFile.close();
  }
  else {
    Serial.printf("error opening %s", filename);
    return 0;
  }
  return 1;
}

void processLine(String line, float *ptR, float *ptT) {
  int commaIndex = line.indexOf(';');

  if (commaIndex != -1) {
    String firstValue = line.substring(0, commaIndex);
    String secondValue = line.substring(commaIndex + 1);
    *(ptT+indey) = firstValue.toFloat();
    *(ptR+indey) = secondValue.toFloat();
    indey++;
 }
}