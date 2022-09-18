// Arudino Sample Code to use ADS1256 library

// Efficient Input Cycling
// Reads 4 differential channels using effiecient input cycling
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

#include <ADS1256.h>
#include <SPI.h>

float clockMHZ = 7.68; // crystal frequency used on ADS1256
float vRef = 2.5; // voltage reference
float y1 = 400.0; // calibrated mass to be added
float x[2] = {};
float avg_size = 1000.0; // amount of averages for each mass measurement
int cali_time = 15000;
int j = 0;
long val;

// Initialize ADS1256 object

ADS1256 adc(clockMHZ,vRef,false); // RESETPIN is permanently tied to 3.3v

void setup()
{
  Serial.begin(115200);
  
  for (int i=2;i<=8;i++){
    pinMode(i,INPUT_PULLUP);
  }

  adc.begin(ADS1256_DRATE_1000SPS,ADS1256_GAIN_64,false); 
  adc.setChannel(0,1);
  
  while(j < 2){
    if (Serial.read() == 'R'){
      float x0 = 0L;
      for (int ii=0;ii<int(avg_size);ii++){
        adc.waitDRDY();
        x0+=adc.readCurrentChannel();
      }
      x0/=long(avg_size);
      val=long (x0*10000000L);
      x[j]=float (val/100.0);
      Serial.println(x[j]);
      j+=1;
    }
    else{
        adc.waitDRDY();
        adc.readCurrentChannel();
    }
  }  
}

void loop()
{ 
  if(Serial.read() == 'R'){
    float reading[400]={};
    for(int i =0; i<=399;i++){
      adc.waitDRDY();
      reading[i] = adc.readCurrentChannel();
    }
    
    for(int i =0; i<=399;i++){
      // calculating mass based on calibration and linear fit
      val = long (reading[i]*10000000L);
      reading[i] = float (val/100.00);
      float ratio_1 = (float) (reading[i]-x[0]);
      float ratio_2 = (float) (x[1]-x[0]);
      float ratio = ratio_1/ratio_2;
      float mass = y1*ratio;
      Serial.println(mass);
      delay(10);
      }
  }
//    else{
//      adc.waitDRDY();
//      adc.readCurrentChannel();
//    }
} 
  
