// Arudino Sample Code to test ADS1256 in startle response
// Necessary library and package are from https://github.com/adienakhmad/ADS1256 , written by Adien Akhmad

// Reads the first two channels only (AIN0 and AIN1)
/* Sensor reading:
  sensor1 = differential input connected on AIN0 - AIN1
  sensor2 = differential input connected on AIN2 - AIN3
  sensor3 = differential input connected on AIN4 - AIN5
  sensor4 = differential input connected on AIN6 - AIN7
*/

// to learn further, read on datasheet page 21, figure 19 : Cycling the ADS1256 Input Multiplexer
// http://www.ti.com/lit/ds/symlink/ads1256.pdf

// Written by Adien Akhmad, August 2015
// Modfified  Jan 2019 by Axel Sepulveda for ATMEGA328
// Modfified  AUg 2020 by Tsun-Kai Chang for measure startle reflex

#include <ADS1256.h>
#include <SPI.h>

float clockMHZ = 7.68; // crystal frequency used on ADS1256
float vRef = 2.5; // voltage reference
//float reading[400]={};

// Initialize ADS1256 object
ADS1256 adc(clockMHZ,vRef,false); // RESETPIN is permanently tied to 3.3v



void setup()
{
  Serial.begin(115200);
  //adc.sendCommand(SDATAC);
  //Serial.println("SDATAC command sent");
  //Serial.println("Starting ADC");
  // start the ADS1256 with data rate of 15 SPS
  // other data rates: 
  // ADS1256_DRATE_30000SPS
  // ADS1256_DRATE_15000SPS
  // ADS1256_DRATE_7500SPS
  // ADS1256_DRATE_3750SPS
  // ADS1256_DRATE_2000SPS
  // ADS1256_DRATE_1000SPS
  // ADS1256_DRATE_500SPS
  // ADS1256_DRATE_100SPS
  // ADS1256_DRATE_60SPS
  // ADS1256_DRATE_50SPS
  // ADS1256_DRATE_30SPS
  // ADS1256_DRATE_25SPS
  // ADS1256_DRATE_15SPS
  // ADS1256_DRATE_10SPS
  // ADS1256_DRATE_5SPS
  // ADS1256_DRATE_2_5SPS
  // 
  // NOTE : Data Rate vary depending on crystal frequency. Data rates listed below assumes the crystal frequency is 7.68Mhz
  //        for other frequency consult the datasheet.
  //Posible Gains 
  //ADS1256_GAIN_1 
  //ADS1256_GAIN_2 
  //ADS1256_GAIN_4 
  //ADS1256_GAIN_8 
  //ADS1256_GAIN_16 
  //ADS1256_GAIN_32 
  //ADS1256_GAIN_64 
  adc.begin(ADS1256_DRATE_1000SPS,ADS1256_GAIN_64,false);  //sampling rate is set at 1000Hz, which is good enough in measuring startle response
  adc.setChannel(0,1);      //AIN0 and AIN1 are recommended to use
}

void loop()
{     
      adc.waitDRDY();
      Serial.println(adc.readCurrentChannel()*100000);    //original value might be very small, multiply it by 100000

}
