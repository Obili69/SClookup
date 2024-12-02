# SDlookup Library Documentation

## Overview
The SDlookup library provides functionality for reading temperature-resistance lookup tables from SD cards and performing temperature-to-voltage conversions. This library is designed for Arduino/ESP32 platforms and integrates with SD card modules using SPI communication.

## Dependencies
- Arduino.h
- SD.h
- SPI.h

## Constants
- `maxReading`: Maximum number of readings that can be stored (default: 400)

## Functions

### initSD
Initialize the SD card with custom SPI pins.

```cpp
bool initSD(uint8_t MOSI, uint8_t MISO, uint8_t SCK, uint8_t CS)
```

#### Parameters
- `MOSI`: Master Out Slave In pin number
- `MISO`: Master In Slave Out pin number
- `SCK`: Serial Clock pin number
- `CS`: Chip Select pin number

#### Returns
- `bool`: Returns 0 if initialization is successful (Note: This appears to be inverse logic)

#### Example
```cpp
if(initSD(23, 19, 18, 5)) {
    Serial.println("SD card initialization failed");
}
```

### readSDarray
Reads temperature and resistance values from a CSV file on the SD card.

```cpp
bool readSDarray(float *arrayT, float *arrayR, String filename)
```

#### Parameters
- `arrayT`: Pointer to temperature array
- `arrayR`: Pointer to resistance array
- `filename`: Name of the CSV file to read

#### Returns
- `bool`: Returns 1 if successful, 0 if failed

#### Example
```cpp
float tempArray[maxReading];
float resArray[maxReading];
if(readSDarray(tempArray, resArray, "lookup.csv")) {
    Serial.println("Data loaded successfully");
}
```

### tempToU
Converts temperature to voltage using lookup tables and voltage divider calculations.

```cpp
float tempToU(float tempr, float *Rarr, float *Tarr, float *dspRes, float *dspTemp)
```

#### Parameters
- `tempr`: Input temperature value
- `Rarr`: Pointer to resistance array
- `Tarr`: Pointer to temperature array
- `dspRes`: Pointer to store calculated resistance
- `dspTemp`: Pointer to store calculated temperature
- `minTemp`: Minimum temperature in the lookup table
- `maxTemp`: Maximum temperature in the lookup table

#### Returns
- `float`: Returns 1 if successful, 0 if temperature is out of range

#### Example
```cpp
float resistance, temperature;
if(tempToU(25.0, resArray, tempArray, &resistance, &temperature)) {
    Serial.printf("Resistance: %.2f, Voltage: %.2f\n", resistance, temperature);
}
```

### mapfloat
Maps a float value from one range to another.

```cpp
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
```

#### Parameters
- `x`: Value to map
- `in_min`: Input range minimum
- `in_max`: Input range maximum
- `out_min`: Output range minimum
- `out_max`: Output range maximum

#### Returns
- `float`: Mapped value

## File Format
The library expects CSV files with the following format:
- Values separated by semicolons (;)
- First value: Temperature
- Second value: Resistance
- One pair per line

Example:
```
-50;350.5
-49;348.2
...
```

## Error Handling
- The library prints error messages to Serial when SD card operations fail
- Functions return boolean values to indicate success/failure
- Temperature conversion checks for out-of-range values

## Limitations
- Maximum of 400 readings can be stored (defined by maxReading)
- File names must be compatible with SD FAT filesystem
- Values must be within the float range of the platform

## Example Usage

```cpp
#include "SDlookup.h"

// Pin definitions
#define MOSI_PIN 23
#define MISO_PIN 19
#define SCK_PIN  18
#define CS_PIN   5

float tempArray[maxReading];
float resArray[maxReading];
float resistance, voltage;

void setup() {
    Serial.begin(115200);
    
    // Initialize SD card
    if(initSD(MOSI_PIN, MISO_PIN, SCK_PIN, CS_PIN)) {
        Serial.println("SD initialization failed");
        return;
    }
    
    // Read lookup table
    if(!readSDarray(tempArray, resArray, "temp_lookup.csv")) {
        Serial.println("Failed to read lookup table");
        return;
    }
    
    // Convert temperature to voltage
    if(tempToU(25.0, resArray, tempArray, &resistance, &voltage)) {
        Serial.printf("For 25°C: Resistance=%.2f, Voltage=%.2fV\n", 
                     resistance, voltage);
    }
}
```