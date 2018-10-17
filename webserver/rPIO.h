//Kaveh Pezeshki kpezeshki@hmc.edu
//10/12/2018 A modified version of GPIO.h provided in the lab with added timer and SPI support

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
#define SPIBASE   perifBase+0x204000 //from page 152

// Constants for Pin funct
#define OUTPUT 0b001

//LED output pin
#define LEDPIN 21

//IO memory addresses
#define TIMER_CONTROL_STATUS (* (volatile unsigned int *) (TIMER))
#define TIMER_COUNTER_LOW    (* (volatile unsigned int *) (TIMER+1))
#define TIMER_1_COMPARE      (* (volatile unsigned int *) (TIMER+4))
 
#define SPI0_FIFO    (*(volatile unsigned int *) (SPI+1))
#define SPI0_CONTROL (*(volatile unsigned int *) (SPI+0)) 

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
  //setting pin states, pin type 4 corresponds to SPI
  setPinType(8, 4);  //CE
  setPinType(9, 4);  //MISO
  setPinType(10, 4); //MOSI
  setPinType(11, 4); //SCLK
  //reading the current SPI status
  int status = *(SPI+0);
  //printf("pre-setup status 0x%x\n", status);
  //settng up SPI clock
  //bits 15:0 of CLK register define the clock divider, where SCLK = core clk / CDIV
  //The core clock is 250MHz
  int clkdivide = 25000000/clkspeed;
  *(SPI+2)      = clkdivide;
  //printf("Clock divide coefficient %d\n", *(SPI+2));
  //setting control bits
  int control = cs + (cpha << 2) + (cpol << 3) + (clear << 4) + (cspol << 6);
  //printf("Setting SPI control bits as %x\n", control);
  *(SPI+0)  = control;
  //taking the SPI interface out of power saving mode
  *(SPI+0) |= 1 << 7;

  //reading the current SPI status
  status = *(SPI+0);
  //printf("post-setup status 0x%x\n", status);
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
//@param us: the time to delay in microseconds
void delayus(int us) {
  //clearing relevant CS bit
  TIMER_CONTROL_STATUS = 1 << 1;
  //setting the compare register
  TIMER_1_COMPARE = TIMER_COUNTER_LOW + us;
  //waiting until the relevant CS bit becomes 1
  int timerStatus = 0;
  while(timerStatus == 0) {timerStatus = (TIMER_CONTROL_STATUS >> 1) & 1;}
}

//writes a given byte of data over SPI0 and returns the read byte
//@param byte: the data to send
char writeReadSPI0Byte(char byte) {
  SPI0_FIFO = byte;
  //printf("Current SPI Status %x\n", SPI0_CONTROL);
  volatile unsigned int readComplete = (SPI0_CONTROL >> 16) & 1; //only 1 if DONE bit is 1
  //printf("Current readComplete Status %x\n", readComplete);
  while (readComplete == 0) {
    //we do nothing
  readComplete = (SPI0_CONTROL >> 16) & 1; //only 1 if DONE bit is 1
  //printf("Current readComplete Status %x\n", readComplete);
  }
  return SPI0_FIFO; //received data
}

//writes a given short of data over SPIO0 and returns the rea short
//@param send: the short to send
ushort writeReadSPI0Short(ushort send) {
	//reusing writeReadSPI0Byte
	//sending the upper bit
	char byte1 = writeReadSPI0Byte( (send>>8) );
	//sending the lower bit
	char byte2 = writeReadSPI0Byte( (send & 0x00FF) );
	//printf("byte1 byte2 %x %x\n", byte1,byte2);
	//combining the two received bytes
	ushort received = ( ((ushort) byte1) << 8) | byte2;
	return received;	
}

//stops SPI communication, sets CS lines HIGH
void stopSPI() {
  //printf("masking with %X\n", 0xFFFFFF7F);
  *(SPI+0) &= 0xFFFFFF7F; //writing 0 into TA bit
 
  //reading the current SPI status
  //int status = *(SPI+0);
  //printf("current SPI status 0x%x\n", status);
}

//starts SPI communication, sets CS lines LOW
void startSPI() {
  *(SPI+0) |= 1 << 7; //writing 1 into TA bit
}


#endif
