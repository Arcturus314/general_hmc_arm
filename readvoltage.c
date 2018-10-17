//Kaveh Pezeshki kpezeshki@hmc.edu
//10/12/2018 A voltage measurement program using the muddpi ADC

#include <stdio.h>
#include "rPIO.h"


int main() {

    //first set up SPI (50KHz)
    SetUpPerif();
    SetUpTimer();
    SetUpSPI0(0, 0, 0, 0, 0, 50000); //CPOL=0, CPHA=0
    //then continually write a series of bits through spi0
    //printf("\n\n");
    while (1) {
    //for(int i = 0; i < 1; i++) {
	//bringing CS line high
	startSPI();
	//read in two bytes, as output is 10 bits
	//first byte we transmit 0x60 = 0b01100000: start, single mode, channel 0
	//second byte we transmit can take any value
	ushort fullval = writeReadSPI0Short(0x6000);
	//remove top 6 bits, as we expect the output in the lower 10 bits of the short
	fullval &= 0x03FF;
	
	//printf("fullval %x %d\n", fullval, fullval);
	//converting to voltage. As the reference voltage is 3.3V this is the full scale deflection
	float voltage = 3.3*((float)fullval/1023.0);
	printf("voltage %f\n", voltage);
	//bringing CS line low
	stopSPI();
	//delayus(100000);
    }
    //printf("\n\n");
    //bringing the CS line low
    stopSPI();

    return 0;
}
