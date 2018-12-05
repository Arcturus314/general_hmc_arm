#Kaveh Pezeshki 12/5/2018
#kpezeshki@hmc.edu
#logs 3-axis acceleration data from an ADXL345 at 800Hz while logging temperature data from a BME280 at 1Hz

#TODO: Actually make some way to close the file / terminate execution
#TODO: Turn on a green light when data logging starts
#TODO: other things presumably


import time
import datetime
import Adafruit_ADXL345
import Adafruit_BME280


#file to log
output_file = "cw_output_"+str(datetime.datetime.now()+".csv")
version_num = "v0.1"


#creating an ADXL345 instance
accel = Adafruit_ADXL345.ADXL345()
#creating a BME280 instance
envir = Adafruit_BME280.BME280()

#setting ADXL345 data rate to 800 Hz and fsd to 16g
accel.set_range(Adafruit_ADXL345.ADXL345_RANGE_16_G)
accel.set_data_rate(Adafruit_ADXL345.ADXL345_DATARATE_3200_HZ)

#opening file 
logfile = open(output_file, "w")

logfile.write("Clay-Wolkin Fellowship Acceleration Data Logger " + version_num + 
              "\n Acceleration: 3200Hz at 16G FSD \n Temperature: 1Hz \n Current Date: " + str(datetime.datetime.now()) + 
              "\n Time, Accel X, Accel Y, Accel Z, Temp")

#recording start time
starttime = time.time()

#storing the current number of logs since last temp reading. Read temp every 5000 logs
num_logs = 0
temp_str = str(envir.read_temperature())

#starting data logging loop
while(True):
    #fetching time
    deltatime_str = str(time.time()-starttime)
    #fetching acceleration in xyz
    accel_x, accel_y, accel_z = accel.read()
    accel_x_str = str(accel_x)
    accel_y_str = str(accel_y)
    accel_z_str = str(accel_z)
    #fetching temp if necessary
    if num_logs == 5000:
        num_logs = 0
        temp_str = str(envir.read_temperature())
    #creating line to log
    line_log = deltatime_str + "," + accel_x_str + "," + accel_y_str + "," + accel_z_str + "," + temp_str
    #logging said line
    logfile.write(line_log)
    #incrementing num_logs
    num_logs += 1
#never close the file cause file corruption will definitely never happen no sir no chance of that here