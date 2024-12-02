# SDlookup Library

SDlookup is an Arduino/ESP32 library for temperature sensor simulation using resistance lookup tables. It provides functionality to read temperature-resistance pairs from an SD card and calculate corresponding voltage outputs based on a voltage divider configuration.

## Features
- Read temperature-resistance lookup tables from SD card
- Calculate voltage outputs for temperature inputs
- Support for custom SPI pin configurations
- Range checking for temperature inputs
- Configurable step size for temperature values

## Installation

1. Download the library files
2. Place them in your Arduino libraries folder
3. Include the library in your sketch:
```cpp
#include "SDlookup.h"
```

## Dependencies
- Arduino.h
- SD.h
- SPI.h

## Hardware Setup
The library requires:
- An SD card module connected via SPI
- Proper voltage (3.3V or 5V depending on your module)
- Correct pin connections:
  - MOSI (Master Out Slave In)
  - MISO (Master In Slave Out)
  - SCK (Serial Clock)
  - CS (Chip Select)

## Constants
- `maxReading`: Maximum number of readings that can be stored (default: 400)

## CSV File Format Specification

### Separator
The file must use semicolons (;) as separators, not commas.

### File Structure
The file must contain:
1. **First Line**: Contains two values:
   - First value: Step size for temperature increments
   - Second value: Maximum temperature of the range
2. **Second Line**: Contains two values:
   - First value: Minimum temperature of the range
   - Second value: Initial resistance value
3. **Subsequent Lines**: Temperature-resistance pairs:
   - First value: Temperature
   - Second value: Corresponding resistance

### Example CSV File
```
1.0;100.0    # Step size;Maximum temperature
-40.0;5000.0 # Minimum temperature;Initial resistance
-39.0;4800.5 # Temperature;Resistance pairs
-38.0;4600.3
-37.0;4400.1
...
```

### Format Requirements
- Each line must contain exactly one semicolon
- All values must be valid floating-point numbers
- Maximum of 400 lines (defined by maxReading)
- Values should be in simple decimal format (e.g., "25.0" not "2.5e1")
- No headers or comments are allowed in the actual file
- No empty lines are allowed
- No whitespace before or after values is necessary

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

### tempToU
Converts temperature to voltage using lookup tables and voltage divider calculations.

```cpp
float tempToU(float tempr, float *Rarr, float *Tarr, float *dspRes, float *dspTemp)
```

#### Parameters
- `tempr`: Input temperature value
- `Rarr`: Pointer to resistance array (includes step size and max temp)
- `Tarr`: Pointer to temperature array (includes min temp)
- `dspRes`: Pointer to store calculated resistance
- `dspTemp`: Pointer to store calculated temperature

#### Returns
- `float`: Returns 1 if successful, 0 if temperature is out of range

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

## Complete Example

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
    
    // Print configuration
    Serial.printf("Step size: %.1f\n", resArray[0]);
    Serial.printf("Temperature range: %.1f to %.1f\n", tempArray[1], resArray[1]);
    
    // Convert temperature to voltage
    if(tempToU(25.0, resArray, tempArray, &resistance, &voltage)) {
        Serial.printf("For 25°C: Resistance=%.2f, Voltage=%.2fV\n", 
                     resistance, voltage);
    } else {
        Serial.println("Temperature out of range!");
    }
}

void loop() {
    // Your code here
}
```

## Troubleshooting

### SD Card Issues
1. Check if the SD card is properly formatted (FAT16/FAT32)
2. Verify the SPI connections
3. Ensure the CS pin matches your configuration
4. Check if the SD card is properly inserted

### File Reading Issues
1. Verify the file name (8.3 format recommended)
2. Check the file format matches the specification
3. Ensure no empty lines or extra characters in the file
4. Verify all values are valid floating-point numbers

### Temperature Conversion Issues
1. Check if the temperature is within the defined range
2. Verify the lookup table contains appropriate values
3. Ensure the step size matches your requirements

## Limitations
- Maximum of 400 readings can be stored (defined by maxReading)
- File names must be compatible with SD FAT filesystem
- Values must be within the float range of the platform
- Temperature must be within the range defined in the lookup table

## Contributing
Improvements to the library are welcome. Please follow these steps:
1. Fork the repository
2. Create your feature branch
3. Submit a pull request

## License
This library is released under the MIT License.

## Version History
- 1.0.0: Initial release

## Author
[Your Name/Organization]