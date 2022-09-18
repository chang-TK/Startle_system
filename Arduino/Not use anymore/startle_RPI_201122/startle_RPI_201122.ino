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

float reading[400]={};
//int pinr = 8;
//int pino = 7;
float clockMHZ = 7.68; // crystal frequency used on ADS1256
float vRef = 2.5; // voltage reference
// Initialize ADS1256 object

ADS1256 adc(clockMHZ,vRef,false); // RESETPIN is permanently tied to 3.3v

float y1 = 100.0; // calibrated mass to be added
float x1 = 0L;
float x0 = 0L;
float avg_size = 1000.0; // amount of averages for each mass measurement
int cali_time = 15000;

void setup()
{
  Serial.begin(115200);
  
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
  adc.begin(ADS1256_DRATE_1000SPS,ADS1256_GAIN_64,true); 
  
//  pinMode(pinr,INPUT);
//  pinMode(pino,OUTPUT);
//  digitalWrite(pinr,HIGH);
  adc.setChannel(0,1);

  while(1){
    if (Serial.read() == 'R')  {
        delay(1000);
        break;
    }
  }
//  

  delay(2000);
  //Baseline activity with no object
  for (int ii=0;ii<int(avg_size);ii++){
    
    x0+=adc.readCurrentChannel();
    delayMicroseconds(855);
  }
  x0/=long(avg_size);

  Serial.println(x0*(100000));
//  digitalWrite(pino,HIGH); //Send signal when done
//  delay(100);
//  digitalWrite(pino,LOW);
// 
 
  //Add Calibrated Mass, wait for 15 secs
  delay(cali_time);
  for (int jj=0;jj<int(avg_size);jj++){
        x1+=adc.readCurrentChannel();
        delayMicroseconds(855);
      }
  x1/=long(avg_size);

  Serial.println(x1*(100000));      
//  digitalWrite(pino,HIGH); //Send signal when done
//  delay(100);
//  digitalWrite(pino,LOW);
}

void loop()
{ 
  
  if(Serial.read() == 'R'){

 
    for(int i =0; i<=399;i++){
      reading[i] = adc.readCurrentChannel();
      delayMicroseconds(855);
    }
    //    Serial.println("--------------");
    delay(1000);
    for(int i =0; i<=399;i++){
      //Serial.println(i);
      // calculating mass based on calibration and linear fit
      float ratio_1 = (float) (reading[i]-x0);
      float ratio_2 = (float) (x1-x0);
      float ratio = ratio_1/ratio_2;
      float mass = y1*ratio;
      Serial.println(mass);
      delay(10);
      }
    //Serial.println("==============");  
    }
  else{
    }
} 
  
