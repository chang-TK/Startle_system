# ARDUINO_Startle
 ---
 * Arduino codes for testing ads1256 and measuring startle response
 
 * Users should first connect arduino UNO, ads1256 and loadcell together
 
 * Codes are modified from Adien Akhmad mostly, necessary packages and libraries could be also found from his github
 https://github.com/adienakhmad/ADS1256, users are recommended to browse his page before using and for more understanding
 
# Files
---
test_ads1256: Test ads1256 with arduino UNO, read value from ads1256/loadcell

RPI_startle_linear: Used in real startle experiment with Raspberry pi, simple linear regression for loadcell calibration 

Not use anymore: Not using file, for debugging



# Wiring (most from Adien Akhmad)
---
ADS Board - Arduino UNO Board

5V - 5V

GND - GND

SCLK - pin 13 (SCK)

DIN - pin 11 (MOSI)

DOUT - pin 12 (MISO)

DRDY - pin 9

CS - pin 10

POWN - 5V

Loadcell green wire - AIN0

Loadcell white wire - AIN1

Loadcell red wire - 5V

Loadcell black wire - GND
