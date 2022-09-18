// Arudino Code for measuring startle response with ADS1256 library

// Written by Tsun-Kai, Chang, August 2020, for acoustic startle experiment, codes are modified from Adien Akhmad, https://github.com/adienakhmad/ADS1256


// to learn further, read on datasheet page 21, figure 19 : Cycling the ADS1256 Input Multiplexer
// http://www.ti.com/lit/ds/symlink/ads1256.pdf


#include <ADS1256.h>
#include <SPI.h>

float clockMHZ = 7.68; // crystal frequency used on ADS1256
float vRef = 2.512;     // voltage reference

const int cali_weight = 5; // how many weights for calibration, in this example, 5  different weight
float avg_size = 1000.0; // amount of averages for calibration mass measurement
const int data_point= 400;  // how many data points to record in each trial, 400 is adopted
float x[cali_weight] = {};
float y[cali_weight]={0, 200, 400, 600, 800};  //Standard weight for calibration(grams), could be adjusted
float lrCoef[2]={0,0};
int j = 0;
long val;

// Initialize ADS1256 object
ADS1256 adc(clockMHZ,vRef,false); // RESETPIN is permanently tied to 3.3v

void setup()
{
  Serial.begin(115200);
  
  for (int i=2;i<=8;i++){
    pinMode(i,INPUT_PULLUP); //for reducing noise
  }

  adc.begin(ADS1256_DRATE_1000SPS,ADS1256_GAIN_64,false); //sampling rate is set at 1000Hz, which is good enough in measuring startle response
  adc.setChannel(0,1);  //AIN0 and AIN1 are recommended to use

  // Calibration part
  // Arduino receive five triggers from RPI, calibration weight shuld be put on sequentially  
  while(j < cali_weight){
    adc.waitDRDY();
    adc.readCurrentChannel();
    if (Serial.read() == 'R'){
      float x0 = 0L;
      for (int ii=0;ii<int(avg_size);ii++){
        adc.waitDRDY();
        x0+=adc.readCurrentChannel();
        }
      x0/=long(avg_size);          // get average output value from calibration weight
      x[j]=x0;
      Serial.println(x[j]*100000); //original value might be very small, multiply it by 100000
      j+=1;
    }
  }
  
  simpLinReg(x, y, lrCoef, cali_weight);  //simple regression between output value and calibration weight, function below
}



void loop()
{ 
  // Ready for experiment, trials begin when receive trigger from RPI
  
  if(Serial.read() == 'R'){
    // read data from ads1256/loadcell
    float reading[data_point]={};
    for(int i =0; i<=(data_point-1);i++){
      adc.waitDRDY();
      reading[i] = adc.readCurrentChannel(); //receive data
      }
    
    for(int i =0; i<=(data_point-1);i++){
      // calculating mass based on calibration and linear fit
      float mass = reading[i]*lrCoef[0]+lrCoef[1];
      Serial.println(mass);  // report data to RPI
      }
  }
} 


void simpLinReg(float* x, float* y, float* lrCoef, int n){
  // pass x and y arrays (pointers), lrCoef pointer, and n.  The lrCoef array is comprised of the slope=lrCoef[0] and intercept=lrCoef[1].  n is length of the x and y arrays.
  // http://en.wikipedia.org/wiki/Simple_linear_regression

  // initialize variables
  float xbar=0;
  float ybar=0;
  float xybar=0;
  float xsqbar=0;
  
  // calculations required for linear regression
  for (int i=0; i<n; i++){
    xbar=xbar+x[i];
    ybar=ybar+y[i];
    xybar=xybar+x[i]*y[i];
    xsqbar=xsqbar+x[i]*x[i];
  }
  xbar=xbar/n;
  ybar=ybar/n;
  xybar=xybar/n;
  xsqbar=xsqbar/n;
  
  // simple linear regression algorithm
  lrCoef[0]=(xybar-xbar*ybar)/(xsqbar-xbar*xbar);
  lrCoef[1]=ybar-lrCoef[0]*xbar;
}
  
