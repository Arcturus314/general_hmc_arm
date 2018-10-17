//Kaveh Pezeshki kpezeshki@hmc.edu
//10/12/2018 A simple SPI test for the Raspberry Pi

#include <stdio.h>
#include "rPIO.h"


int main() {

    //first set up SPI (50KHz)
    SetUpPerif();
    SetUpSPI0(0, 0, 0, 0, 0, 5000);
    //then continually write a series of bits through spi0
    printf("\n\n");
    for(int i = 0; i < 1; i++) {
	/*
        char byte1 = writeReadSPI0Byte(0x70);
	char byte2 = writeReadSPI0Byte(0x00);
	printf("byte1, byte2 %X %X\n",byte1,byte2);
	int fullval = ((byte1 & 0x03) << 8) | byte2;
	*/
	ushort fullval = writeReadSPI0Short(0x600)
	printf("fullval %d\n", fullval);
	float voltage = 3.3*(fullval/1023);
	printf("voltage %f\n", voltage);
    l
    printf("\n\n");
    stopSPI();

    return 0;
}
