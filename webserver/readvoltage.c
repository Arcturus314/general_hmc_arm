//Kaveh Pezeshki kpezeshki@hmc.edu
//10/12/2018 A voltage measurement program using the muddpi ADC
//reads and prints ADC CH0 Voltage

#include <stdio.h>
#include "rPIO.h"


int main() {

    //printing document format
    printf("Content-Type: text/plain;charset=us-ascii\n\n");
    //first set up SPI (50KHz)
    SetUpPerif();
    SetUpTimer();
    SetUpSPI0(0, 0, 0, 0, 0, 50000); //CPOL=0, CPHA=0

    ushort fullval; //the raw voltage value
    float  voltage; //the processed voltage value
    
    //bringing CS line high
    startSPI();
    //read in two bytes, as output is 10 bits
    //first byte we transmit 0x60 = 0b01100000: start, single mode, channel 0
    //second byte we transmit can take any value
    fullval = writeReadSPI0Short(0x6000);
    //remove top 6 bits, as we expect the output in the lower 10 bits of the short
    fullval &= 0x03FF;
	
    //converting to voltage. As the reference voltage is 3.3V this is the full scale deflection
    voltage = 3.3*((float)fullval/1023.0);
    printf("Phototransistor Voltage: %fV\n\n", voltage);
    //bringing CS line low
    stopSPI();
	
    //bringing the CS line low
    stopSPI();

    return 0;
}
