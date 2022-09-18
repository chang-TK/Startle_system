# startle.py
#    written by Der-Yow Chen, 20190821
#                             20190427 updated
#    modified by Tsun-Kai Chang 20210208
##################################################
#import alsaaudio
from scipy import stats
import RPi.GPIO as GPIO
import serial
import sys,time,random
import numpy
import matplotlib.pyplot as plt
from scipy.integrate import simps
from datetime import datetime

# Experiment Design
chamber=4
session="matching"
session2="matching"
samplesize=400
TrialNo=60
TrialNo2=0
dura=300          #duration before trial begin in secs
dura2=10
ITI=15000          #ITI in milli secs
background_gpio=24
noise_burst_gpio=23
grams=[0,200,400,600,800]
data= numpy.zeros(samplesize)
data2= numpy.zeros(samplesize)
data3= numpy.zeros(samplesize)
data4= numpy.zeros(samplesize)
value=numpy.zeros(len(grams))
slope=numpy.zeros(chamber)
intercept=numpy.zeros(chamber)
dt_string = str(datetime.now())

# use P1 header pin numbering convention
GPIO.setmode(GPIO.BCM)
# Set up the GPIO channels - one input and one output
GPIO.setup(background_gpio, GPIO.OUT,initial=1)
GPIO.setup(noise_burst_gpio, GPIO.OUT,initial=1)

# Initializing
#Serial communication
ser=[]
ser.append(serial.Serial('/dev/ttyACM0',115200))
ser.append(serial.Serial('/dev/ttyACM1',115200))
ser.append(serial.Serial('/dev/ttyACM2',115200))
ser.append(serial.Serial('/dev/ttyACM3',115200))

#Turn serial communication
for i in range(len(ser)):
    ser[i].close()
time.sleep(10)
for i in range(len(ser)):
    ser[i].open()
time.sleep(10)


# Experiment setup
while(1):
    ID = input("Rat1's ID: ")
    ID2 = input("Rat2's ID: ")
    ID3 = input("Rat3's ID: ")
    ID4 = input("Rat4's ID: ")
    
    weight = input("Rat1's weight: ")
    weight2 = input("Rat2's weight: ")
    weight3 = input("Rat3's weight: ")
    weight4 = input("Rat4's weight: ")
    
    group = input("Rat1's group: ")
    group2 = input("Rat2's group: ")
    group3 = input("Rat3's group: ")
    group4 = input("Rat4's group: ")
    
    challenge = input("Rat1's challenge drug: ")
    challenge2 = input("Rat2's challenge drug: ")
    challenge3 = input("Rat3's challenge drug: ")
    challenge4 = input("Rat4's challenge drug: ")
    
    che = input("Everything correct? [y/n]: ")
    if(che=="y"):
        break
#################################


# Prepare
outfilename=time.strftime("%Y%m%d%H%M%S_matching") + ".txt"
outfilename2=time.strftime("%Y%m%d%H%M%S_matching") + "_trial.txt"

outfile=open(outfilename,"w")
outfile.write("#TrialNo, subject, session, weight, group, SampleNo, Value, challenge\n") 
outfile2=open(outfilename2,"w")
outfile2.write("#TrialNo, subject, session, weight, group, Peak, AUC, PeakTime, Mean, Mean200, Min, Peak-Min, challenge\n")
outfile3=open("calibration.txt","a")
##################################


#################################
# Display Experiment Info
print("Startle Experiment:")
print("  TrialNo:%3d, ITI:%5d msec." % (TrialNo, ITI))
print("Initializing...")
print ("--------")
#################################

j=0
while j<5:
    #Send trigger to arduino, for calibration
    input('put on %d grams, then press any key '%(grams[j]))
    ser[0].write(str.encode("R"))
    ser[1].write(str.encode("R"))
    ser[2].write(str.encode("R"))
    ser[3].write(str.encode("R"))
    #ser[0:3].write(str.encode("R"))
    myData = float(ser[0].readline().strip())
    myData2 = float(ser[1].readline().strip())
    myData3 = float(ser[2].readline().strip())
    myData4 = float(ser[3].readline().strip())
    if myData and myData2 and myData3 and myData4:
        print("chamber1:",myData)
        print("chamber2:",myData2)
        print("chamber3:",myData3)
        print("chamber4:",myData4)
        print("----------------------")
        
        outfile3.write("%s, %s, %d, %lf\n"%(dt_string,1,grams[j],myData))
        outfile3.write("%s, %s, %d, %lf\n"%(dt_string,2,grams[j],myData2))
        outfile3.write("%s, %s, %d, %lf\n"%(dt_string,3,grams[j],myData3))
        outfile3.write("%s, %s, %d, %lf\n"%(dt_string,4,grams[j],myData4))
        j = j+1
        
    else:
        continue   

outfile3.close()   
outfile3=open("calibration.txt","r")
lines = outfile3.readlines()

j=0
for i in range(chamber):
    t=chamber*len(grams)-j
    for k in range(len(grams)):
        last_lines = lines[-t]
        res = last_lines.split(', ')
        print(res[3])
        value[k]=res[3]
        t=t-chamber
        
    sl, inte, r_value, p_value, std_err = stats.linregress(value,grams)
    slope[i]=sl
    intercept[i]=inte
    j=j+1

outfile3.close()    
#Check calibration
#ser.write[0:3](str.encode("R"))
input("Put on test weight")
ser[0].write(str.encode("R"))
ser[1].write(str.encode("R"))
ser[2].write(str.encode("R"))
ser[3].write(str.encode("R"))

for i in range(samplesize):
    data[i]= slope[0]*float(ser[0].readline().strip())+intercept[0]
    data2[i]= slope[1]*float(ser[1].readline().strip())+intercept[1]
    data3[i]= slope[2]*float(ser[2].readline().strip())+intercept[2]
    data4[i]= slope[3]*float(ser[3].readline().strip())+intercept[3]


print(data.mean())
print(data2.mean())
print(data3.mean())
print(data4.mean())

#Initializaion
data= numpy.zeros(samplesize)
data2= numpy.zeros(samplesize)
data3= numpy.zeros(samplesize)
data4= numpy.zeros(samplesize)
    
#set background noise wait for 5 mins
input("Ready? ")
#m.setvolume(vol_background)
GPIO.output(background_gpio,0)
time.sleep(1)
GPIO.output(background_gpio,1)
time.sleep(dura)

####trial begin####
k=1
for trial in range(TrialNo):
    print("Trial %3d:" % (trial+1))
    d0 =time.time()
    k=k^1
    #trigger arduino
    ser[0].write(str.encode("R"))
    ser[1].write(str.encode("R"))
    ser[2].write(str.encode("R"))
    ser[3].write(str.encode("R"))
    #noise burst
    GPIO.output(noise_burst_gpio,k)
    time.sleep(0.5)
    GPIO.output(noise_burst_gpio,1)
    #print(time.time()-d0)
    
    #print (str(int((time.time()-d0)*1000))+"mS in data acqusition of " + str(samplesize) + " samples.\n")
    #################################
    # read data from arduino
    for i in range(samplesize):
        data[i]= slope[0]*float(ser[0].readline().strip())+intercept[0]
        data2[i]= slope[1]*float(ser[1].readline().strip())+intercept[1]
        data3[i]= slope[2]*float(ser[2].readline().strip())+intercept[2]
        data4[i]= slope[3]*float(ser[3].readline().strip())+intercept[3]

    # Plot this trial
    plt.clf() 
    plt.title("Trial: " + str(trial+1))
    plt.xlabel("Time (msec)")
    plt.ylabel("Amplitude (arbitury)")
    plt.plot(range(samplesize) , data)
    plt.plot(range(samplesize) , data2)
    plt.plot(range(samplesize) , data3)
    plt.plot(range(samplesize) , data4)
    plt.draw()
    plt.pause(0.005)

    
    # Write data to data file
    for i in range(samplesize):
        outfile.write("%3d, %s, %s, %s, %s, %3d, %lf, %s\n"%(trial+1,ID,session,weight,group,i+1,data[i],challenge)) 
        outfile.write("%3d, %s, %s, %s, %s, %3d, %lf, %s\n"%(trial+1,ID2,session,weight2,group2,i+1,data2[i],challenge2))  
        outfile.write("%3d, %s, %s, %s, %s, %3d, %lf, %s\n"%(trial+1,ID3,session,weight3,group3,i+1,data3[i],challenge3))  
        outfile.write("%3d, %s, %s, %s, %s, %3d, %lf, %s\n"%(trial+1,ID4,session,weight4,group4,i+1,data4[i],challenge4))  
    # Calculate Trial Summary
    Peak=max(data)
    Peak2=max(data2)
    Peak3=max(data3)
    Peak4=max(data4)
    
    PeakTime=numpy.argmax(data)+1
    PeakTime2=numpy.argmax(data2)+1
    PeakTime3=numpy.argmax(data3)+1
    PeakTime4=numpy.argmax(data4)+1
    
    Mean=data.mean()
    Mean2=data2.mean()
    Mean3=data3.mean()
    Mean4=data4.mean()
    
    data200=data[0:200]
    data200_2=data2[0:200]
    data200_3=data3[0:200]
    data200_4=data4[0:200]
    
    Mean200=data200.mean()
    Mean200_2=data200_2.mean()
    Mean200_3=data200_3.mean()
    Mean200_4=data200_4.mean()
    
    Min=min(data)
    Min2=min(data2)
    Min3=min(data3)
    Min4=min(data4)
    
    ## area under curve
    #a1=simps(Mean*numpy.ones(samplesize),numpy.array(range(0,samplesize)))
    #a2=simps(Mean2*numpy.ones(samplesize),numpy.array(range(0,samplesize)))
    #a3=simps(Mean3*numpy.ones(samplesize),numpy.array(range(0,samplesize)))
    
    data_auc=simps(abs(data-Mean),numpy.array(range(0,samplesize)))
    data_auc2=simps(abs(data2-Mean2),numpy.array(range(0,samplesize)))
    data_auc3=simps(abs(data3-Mean3),numpy.array(range(0,samplesize)))
    data_auc4=simps(abs(data4-Mean4),numpy.array(range(0,samplesize)))
    #print(data_auc)
    #print(data_auc2)
    #print(data_auc3)
    
    print("%.2f" %Mean)
    print("%.2f" %Mean2)
    print("%.2f" %Mean3)
    print("%.2f" %Mean4)
    
    outfile2.write("%3d, %s, %s, %s, %s, %lf, %lf, %4d, %lf, %lf, %lf, %lf, %s\n" 
     % (trial+1, ID, session, weight, group, Peak, data_auc, PeakTime, Mean, Mean200, Min, Peak-Min, challenge)) 
    outfile2.write("%3d, %s, %s, %s, %s, %lf, %lf, %4d, %lf, %lf, %lf, %lf, %s\n" 
     % (trial+1, ID2, session ,weight2, group2, Peak2, data_auc2, PeakTime2, Mean2, Mean200_2, Min2, Peak2-Min2, challenge2)) 
    outfile2.write("%3d, %s, %s, %s, %s, %lf, %lf, %4d, %lf, %lf, %lf, %lf, %s\n" 
     % (trial+1, ID3, session ,weight3, group3, Peak3, data_auc3,PeakTime3, Mean3, Mean200_3, Min3, Peak3-Min3, challenge3)) 
    outfile2.write("%3d, %s, %s, %s, %s, %lf, %lf, %4d, %lf, %lf, %lf, %lf, %s\n" 
     % (trial+1, ID4, session ,weight4, group4, Peak4, data_auc4,PeakTime4, Mean4, Mean200_4, Min4, Peak4-Min4, challenge4))
    # Wait for ITI: inter-trial interval
    data= numpy.zeros(samplesize)
    data2= numpy.zeros(samplesize)
    data3= numpy.zeros(samplesize)
    data4= numpy.zeros(samplesize)
    time.sleep((ITI/1000.0)-(time.time()-d0))

#################################
# Finishing
GPIO.cleanup()
outfile.close()
outfile2.close()
ser[0].close()
ser[1].close()
ser[2].close()
ser[3].close()
#################################




