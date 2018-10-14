//Kaveh Pezeshki kpezeshki@hmc.edu
//10/12/2018 A modified version of GPIO.h provided in the lab with added timer support

#ifndef _INCLUDE_GPIO_
#define _INCLUDE_GPIO_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>


// Mem locations for Periferals
#define perifBase 0x3F000000
#define GPIOBASE  perifBase+0x200000
#define TIMERBASE perifBase+0x3000
#define SPIBASE   perifBase+0x204000 //152

// Constants for Pin funct
#define OUTPUT 0b001

//LED output pin
#define LEDPIN 21

volatile unsigned int *GPIO;  // pointer to base of GPIO registers
volatile unsigned int *TIMER; // pointer to base of timer registers
volatile unsigned int *SPI;   // pointer to base of SPI registers

// from http://mygeekyfun.blogspot.com/2012/06/gpio-on-raspberry-pi.html
// map a page (4096 bytes) starting at physical address base
volatile unsigned int *phys_to_virt(long base)
{
  int mem_fd;
  char *io_mem;
  char *io_m;
  volatile unsigned *io;

  mem_fd=open("/dev/mem",O_RDWR|O_SYNC);

  // create page-aligned pointer to 4096 bytes
  io_mem=(char *) malloc(8192);
  io_mem+=4096;
  io_mem=(char*) (((int) io_mem) & 0xfffff000);

  io_m=(char *) mmap((caddr_t)io_mem,4096,
                     PROT_READ|PROT_WRITE,
                     MAP_SHARED|MAP_FIXED,
                     mem_fd,base);
  //io=(volatile unsigned *)io_m;
  return((int *)io_m);
}

// Sets up the pointers for GPIO.h
void SetUpPerif()
{
	GPIO = phys_to_virt(GPIOBASE);
}

void SetUpTimer() {
  TIMER = phys_to_virt(TIMERBASE);
}

//Sets a pin as an input, output, or an ALT mode
//pin: the pin to set the mode of
//mode: the mode of pin pin, from the following values:
//000: INPUT
//001: OUTPUT
//100 - 010: ALT modes
void setPinType( int pin, int func)
{
	int reg = pin/10;
	int offset = (pin % 10)*3;
	volatile unsigned int* loc = GPIO + reg;
	*(loc) = *(volatile unsigned int*) (loc) &~((0b111 & ~ func) << offset);
	*(loc) =  *(volatile unsigned int*) (loc) | ((0b111 &   func) << offset);
}

/*
@param cs: chip select, 0-2
@param cpha: clock phase
  0: transition at middle of data bit
  1: transition at beginning of data bit
@param cpol: clock polarity
  0: rest state of clock low
  1: rest state of clock high
@param clear:
  0: no action
  1: clear TX FIFO
  2: clear RX FIF
  3: clear TX / RX FIFO
@param cspol:
  0: chip select active low
  1: chip select active high
@param clkspeed:
  frequency for the serial clk
*/
void SetUpSPI0(int cs, int cpha, int cpol, int clear, int cspol, int clkspeed) {
  SPI = phys_to_virt(SPIBASE);
  //setting control bits
  *(SPI+0) = cs + (cpha << 2) + (cpol << 3) + (clear << 4) + (cspol << 6);
  //setting pin states, pin type 4 corresponds to SPI
  setPinType(8, 4);  //CE
  setPinType(9, 4);  //MISO
  setPinType(10, 4); //MOSI
  setPinType(11, 4); //SCLK
  //settng up SPI clock
  //bits 15:0 of CLK register define the clock divider, where SCLK = core clk / CDIV
  //The core clock is 250MHz
  *(SPI+2) = 250000000/clkspeed;
  //taking the SPI interface out of power saving mode
  *(SPI+0) |= 1 >> 7;
}

// Used to write a pin high or low
void digitalWrite(int pin, int val)
{
	int reg = pin / 32;
	int offset = pin % 32;
	volatile unsigned int* loc = GPIO + reg + 7;
	if (val){
		*(loc) = 1 << offset;
	}
	else {
		loc += 3;
	}	*(loc) = 1 << offset;
}

//used to delay program execution by a certain number of ms
//@param timerNum: the system timer (0-3) to use
//@param ms: the time to delay in microseconds
void delayms(int timerNum, int us) {
  //setting up relevant pointers
  volatile unsigned int* controlStatus = TIMER;
  volatile unsigned int* counterLow    = TIMER + 1;
  volatile unsigned int* compare       = TIMER + 3 + timerNum;
  //clearing relevant CS bit
  *(controlStatus) = 1 << timerNum;
  //setting the compare register
  *(compare)       = *(controlStatus) + us;
  //waiting until the relevant CS bit becomes 1
  int timerStatus = 0;
  while(timerStatus == 0) {timerStatus = (*(controlStatus) >> timerNum) & 1;}
}

//writes a given byte of data over SPI0
//@param byte: the data to send
void writeSPI0Byte(char byte) {
  *(SPI+1) = byte;
}

//waits until an SPI transfer has completed and returns the result
char readSPI0Byte() {
  volatile unsigned int readComplete = (*(SPI+0) >> 16) & 1; //only 1 if DONE bit is 1
  while (readComplete == 0) {
    //we do nothing
  }
  return *(SPI+1); //received data
}

#endif