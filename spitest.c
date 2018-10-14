//Kaveh Pezeshki kpezeshki@hmc.edu
//10/12/2018 A simple SPI test for the Raspberry Pi

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <rPIO.h>


int main() {

    //first set up SPI (50KHz)
    SetUpSPI0(0, 0, 0, 0, 0, 50000);
    //then continually write a series of bits through spi0
    while(1) {
        printf("writing byte 0xFF\n");
        writeSPI0Byte(0xFF);
        printf("writing byte 0x00\n");
        writeSPI0Byte(0x00);
    }

    return 0;
}
