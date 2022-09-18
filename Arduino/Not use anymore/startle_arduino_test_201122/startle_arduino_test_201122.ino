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

int sample_size = 400;
int wait_time= 10000;
int wait_time_rat= 5000;
float clockMHZ = 7.68; // crystal frequency used on ADS1256
float vRef = 2.5; // voltage reference



// Initialize ADS1256 object
ADS1256 adc(clockMHZ,vRef,false); // RESETPIN is permanently tied to 3.3v

//float sensor1;
float y1 = 400.0; // calibrated mass to be added
float x1 = 0L;
float x0 = 0L;
float avg_size = 1000.0; // amount of averages for each mass measurement
long val;
 
void setup()
{
  Serial.begin(115200);
  Serial.println("Starting ADC");
  for (int i=2;i<=8;i++){
    pinMode(i,INPUT_PULLUP);
  }
  //Serial.println(t);
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
  adc.begin(ADS1256_DRATE_1000SPS,ADS1256_GAIN_64,false); 
  adc.setChannel(0,1);
  Serial.println("ADC Started");
  delay(5000);

  for (int ii=0;ii<int(avg_size);ii++){
    adc.waitDRDY();
    adc.readCurrentChannel();
  }
  for (int ii=0;ii<int(avg_size);ii++){
    adc.waitDRDY();
    x0+=adc.readCurrentChannel();
  }
  
  x0/=float(avg_size);
//  val=long(x0*1000000L);
//  x0=float (val/10.0);
  Serial.println(x0*100000,2);

  
  //Serial.println("Add Calibrated Mass, you have"+ String(wait_time) +"secs");
  delay(wait_time);
  
  for (int ii=0;ii<int(avg_size);ii++){
    adc.waitDRDY();
    x1+=adc.readCurrentChannel();
  }
  x1/=float(avg_size);
//  val=long(x1*1000000L);
//  x1=float (val/10.0);
  Serial.println(x1*100000,2);

  Serial.println("Calibration Complete");
  Serial.println("Put on rat");
  delay(wait_time_rat);

  
}

void loop()
{ 
  
    //adc.readCurrentChannel();
    float reading[400]={};
    float sum = 0;


    
    for(int i =0; i<=sample_size-1;i++){
      adc.waitDRDY();
      reading[i] = adc.readCurrentChannel();
    }
    //Serial.println(millis()-t0);


    for(int i =0; i<=(sample_size-1);i++){
        //Serial.println(i);
        //calculating mass based on calibration and linear fit
//        val = long (reading[i]*1000000L);
//        reading[i]= float(val/10.0);
        float ratio_1 = (float) (reading[i]-x0);
        float ratio_2 = (float) (x1-x0);
        float ratio = ratio_1/(ratio_2);
        //float ratio = ratio_1/(1.0);
        float mass = y1*ratio;
        sum += mass;
        //Serial.println(mass);
        //delay(10);
    }
    float avg = sum/400.0;
    Serial.println(avg);



}
