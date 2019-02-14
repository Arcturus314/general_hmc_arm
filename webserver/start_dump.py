#Kaveh Pezeshki
#10/10/2018
#Manages dump1090 output and writes most recent output to file flights.txt

import subprocess
import time

output_file = "/var/www/html/flights2.txt"

#first starting dump1090
#returning an iterable with each set of plane printings
def run_dump1090():
    dump1090 = subprocess.Popen(['/home/pi/dump1090/dump1090', '--interactive'], stdout=subprocess.PIPE)
    return iter(dump1090.stdout.readline, b"") #sentinel of iterator is an empty string (returns on every line)


set_init_sequence = b'\x1b[H\x1b[2J'.decode("utf-8") #the command to clear the screen. we take this as the start of a plane dataset
dataset_full = "" #storing header and plane data

#stepping through the iterable
for dataset_bytes in run_dump1090():
    dataset = dataset_bytes.decode("utf-8")
    if set_init_sequence in dataset: #if we are in the inital line, with the byte sequence being the "clear screen"
        print("data set collected")
        #writing out data to a file
        f = open(output_file, "w")
        f.write(dataset_full.replace(set_init_sequence, '')) #removing the clear screen byte sequence
        dataset_full = "" #clearing the dataset

    dataset_full += dataset
