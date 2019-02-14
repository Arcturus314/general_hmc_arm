#Kaveh Pezeshki
#10/10/2018
#Manages dump1090 output and writes most recent output to file flights.txt

import subprocess
import time

output_file = "/var/www/html/flights2.txt"
output_file_parsed = "/var/www/html/flights_parsed.txt"

file_structure = {
        "Hex":    0,
        "Mode":   1,
        "Sqwk":   2,
        "Flight": 3,
        "Alt":    4,
        "Spd":    5,
        "Hdg":    6,
        "Lat":    7,
        "Long":   8,
        "Sig":    9,
        "Msgs":   10
}

min_num_elements  = 12
info_to_include   = ["Flight", "Alt", "Lat", "Long"]
delimiting_string = ","

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
        #parsing 
        dataset_without_clearscreen = dataset_full.replace(set_init_sequence, '')
        dataset_splitlines = dataset_without_clearscreen.split("\n")[2:] #ignoring the first two header lines
        dataset_listelements = [] #for each line sublist, contents follows dict file_structure
        for line in dataset_splitlines:
            dataset_listelements.append(list(filter(lambda a: a != '', line.split(" ")))) 
        #writing out data to a file

        #writing raw data
        f = open(output_file, "w")
        f.write(dataset_without_clearscreen) #removing the clear screen byte sequence
        f.close()

        #writing parsed data
        f = open(output_file_parsed, "w")
        for line in dataset_listelements:
            if len(line) >= min_num_elements:
                for info in info_to_include:
                    f.write(line[file_structure[info]])
                    if info != info_to_include[-1]: f.write(delimiting_string)
                if line != dataset_splitlines[-1]: f.write("\n")
        f.close()

        dataset_full = "" #clearing the dataset

    dataset_full += dataset
