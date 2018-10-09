// lab5.c 9/05/2018
// A C program to play arbitrary sets of notes with the Raspberry Pi
// Kaveh Pezeshki kpezeshki@g.hmc.edu



// Notes for Fur Elise, E155 Lab 5, and Cantina Band
// Cantina band taken from musicnotes.com/sheetmusic/mtd.asp?ppn=MN01333703

// Pitch in Hz, duration in ms

const int cantina[][2] = {
	{440, 250},
	{587, 250},
	{440, 250},
	{587, 250},
	{440, 250},
	{587, 250},
	{440, 125},
	{415, 125},
	{440, 250},
	{440, 125},
	{415, 125},
	{440, 125},
	{392, 125},
	{0,   125},
	{370, 125},
	{392, 125},
	{370, 125},
	{349, 500},
	{294, 250},
	{0,   250},
	{440, 250},
	{587, 250},
	{440, 250},
	{587, 250},
	{440, 250},
	{587, 250},
	{440, 125},
	{415, 125},
	{440, 250},
	{392, 250},
	{392, 250},
	{370, 250},
	{392, 250},
	{523, 250},
	{466, 250},
	{440, 250},
	{392, 250},
	{440, 250},
	{587, 250},
	{440, 250},
	{587, 250},
	{440, 250},
	{587, 250},
	{440, 125},
	{415, 125},
	{440, 250},
	{523, 250},
	{523, 250},
	{440, 250},
	{392, 250},
	{349, 500},
	{294, 500},
	{294, 500},
	{349, 500},
	{440, 500},
	{523, 500},
	{622, 250},
	{587, 250},
	{370, 250},
	{440, 250},
	{349, 1000},
	{440, 250},
	{349, 250},
	{440, 250},
	{0, 250},
	{440, 250},
	{349, 250},
	{440, 250},
	{0, 250},
	{440, 250},
	{349, 250},
	{370, 250},
	{440, 250},
	{349, 500},
	{294, 500},
	{440, 250},
	{349, 250},
	{440, 250},
	{0, 250},
	{440, 250},
	{349, 250},
	{0, 250},
	{440, 250},
	{349, 250},
	{415, 250},
	{440, 250},
	{392, 500},
	{261, 500},
	{440, 250},
	{349, 250},
	{440, 250},
	{0, 250},
	{440, 250},
	{349, 250},
	{440, 250},
	{0, 250},
	{440, 250},
	{349, 250},
	{370, 250},
	{440, 250},
	{349, 500},
	{293, 500},
	{233, 125},
	{293, 125},
	{249, 250},
	{233, 125},
	{293, 125},
	{249, 250},
	{370, 250},
	{440, 250},
	{293, 500},
	{293, 125},
	{349, 125},
	{466, 125},
	{587, 125},
	{370, 250},
	{440, 250},
	{349, 1000},
	{349, 500},
	{698, 125},
	{125, 0}



};

const int notes[][2] = {
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	250},
{0,	125},
{494,	125},
{523,	125},
{587,	125},
{659,	375},
{392,	125},
{699,	125},
{659,	125},
{587,	375},
{349,	125},
{659,	125},
{587,	125},
{523,	375},
{330,	125},
{587,	125},
{523,	125},
{494,	250},
{0,	125},
{330,	125},
{659,	125},
{0,	250},
{659,	125},
{1319,	125},
{0,	250},
{623,	125},
{659,	125},
{0,	250},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	500},
{0,	0}};

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

/////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////

// GPIO FSEL Types
#define INPUT  0
#define OUTPUT 1
#define ALT0   4
#define ALT1   5
#define ALT2   6
#define ALT3   7
#define ALT4   3
#define ALT5   2

#define GPFSEL   ((volatile unsigned int *) (gpio + 0))
#define GPSET    ((volatile unsigned int *) (gpio + 7))
#define GPCLR    ((volatile unsigned int *) (gpio + 10))
#define GPLEV    ((volatile unsigned int *) (gpio + 13))
//timer addresses
#define TIMER_CS  ((volatile unsigned int *) (sys_timer + 0x0))
#define TIMER_CLO ((volatile unsigned int *) (sys_timer + 0x1))
#define TIMER_C1  ((volatile unsigned int *) (sys_timer + 0x4))
#define TIMER_C2  ((volatile unsigned int *) (sys_timer + 0x5))
#define INPUT  0
#define OUTPUT 1

// Physical addresses
#define BCM2836_PERI_BASE        0x3F000000
#define GPIO_BASE               (BCM2836_PERI_BASE + 0x200000)
#define BLOCK_SIZE (4*1024)
#define SYS_TIMERS              (BCM2836_PERI_BASE + 0x3000)

//Pin addresses
#define SPEAKER_PIN  12 //GPIO12 on the Raspberry Pi

// Pointers that will be memory mapped when pioInit() is called
volatile unsigned int *gpio; //pointer to base of gpio
//Timer pointers
volatile unsigned int *sys_timer; //pointer to base of timers

void pioInit() {
	int  mem_fd;
	void *reg_map;

	// /dev/mem is a psuedo-driver for accessing memory in the Linux filesystem
	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
	      printf("can't open /dev/mem \n");
	      exit(-1);
	}

  //GPIO block
	reg_map = mmap(
	  NULL,             //Address at which to start local mapping (null means don't-care)
      BLOCK_SIZE,       //Size of mapped memory block
      PROT_READ|PROT_WRITE,// Enable both reading and writing to the mapped memory
      MAP_SHARED,       // This program does not have exclusive access to this memory
      mem_fd,           // Map to /dev/mem
      GPIO_BASE);       // Offset to GPIO peripheral

	if (reg_map == MAP_FAILED) {
      printf("gpio mmap error %d\n", (int)reg_map);
      close(mem_fd);
      exit(-1);
    }

	gpio = (volatile unsigned *)reg_map;

  //Timer block (using syntax from GPIO block)
  reg_map = mmap(
                 NULL,
                 BLOCK_SIZE,
                 PROT_READ | PROT_WRITE,
                 MAP_SHARED,
                 mem_fd,
                 SYS_TIMERS //Offset to timer peripheral
                 );
  if (reg_map == MAP_FAILED) {
	  printf("timer mmap error %d\n", (int)reg_map);
	  close(mem_fd);
	  exit(-1);
  }

  sys_timer = (volatile unsigned *) reg_map;

}
 
//-----GPIO FUNCTIONS-----

//Sets a pin as an input, output, or an ALT mode
//pin: the pin to set the mode of
//mode: the mode of pin pin, from the following values:
//000: INPUT
//001: OUTPUT
//100 - 010: ALT modes
void pinMode(int pin, int mode) {
  //using code discussed in class
  int reg = pin / 10;
  int offset = 3*(pin % 10);
  GPFSEL[reg] &= ~( (0b111 & ~mode) << offset );
  GPFSEL[reg] |=  (0b111 & mode) << offset;
}


//Changes the state of a pin declared as an output
//pin: the pin to set the state of
//value: the state of the pin to set
//  0: low
//  1: high
void digitalWrite(int pin, int value) {
  //using code discussed in class
  int reg = pin / 32;
  int offset = pin % 32;
  if (value) GPSET[reg] = 1 << offset;
  else       GPCLR[reg] = 1 << offset;
}

//-----TIMER FUNCTIONS-----
//The BCM2837 has an integrated 1MHz continuously-incrementing timer at register CLO
//By moving CLO+time -> C1 (compare register 1), CS register bit 1 will go high when system counter has reached the compare register value
//We initially need to reset CS[1] by writing a 1


//plays a note of frequency frequency (Hz) for time time (s)
void playNote(int frequency, int time) {
  
  //if time==0 immediately return
  if (time==0) {return;}
  
  //a square wave will be modulated by a step up - down function defined by System Timer 1
  //we first reset Timer 1
  TIMER_CS[0] |= 0b0100;
  //we then write Timer 1 Compare register with the current time plus an offset
  TIMER_C1[0] = TIMER_CLO[0] + time*1000; //multiply by 1e3 as timer counts in us

  //while time has not yet elapsed we generate a square wave of the correct frequency
  int time_elapsed = 0; //high when time elapsed >= time

  //when 0, the speaker is low. when 1, the speaker is high
  int speakerState = 0;

  //after every changeTime increments in CLO, speakerState toggles
  int changeTime  = 0;
  //used to track changeTime increments
  int numToggles  = 0;
  if (frequency != 0) {
	changeTime  = 1000000/(2*frequency);
  	numToggles = ((unsigned int) TIMER_CLO[0]) / changeTime;
  }

  while (time_elapsed == 0) {
    //printf("TIMER_CS[0] %x\n", TIMER_CS);	  
    time_elapsed = ((unsigned int) TIMER_CS[0] >> 1)&1;
    //printf("time_elapsed %d", time_elapsed);
    //generating a square wave at a specified frequency
    //change the state of the output after every 1e6/(2*frequency) clock cycles
    //Modulo operator provides this

    //printf("TIMER_CLO[0] %u\n", TIMER_CLO[0]);
    //printf("num_measured_toggles %u\n", ((unsigned int) TIMER_CLO[0]) / changeTime);
    //printf("num toggles %u\n", num_toggles);
    if (changeTime == 0) {}
    else if (((unsigned int) TIMER_CLO[0]) / changeTime > numToggles) {
      //printf("incrementing...");
      numToggles += 1;    
      speakerState = ~speakerState;
      digitalWrite(SPEAKER_PIN, speakerState);
    }

  }


}


//plays a two dimensional array of numbers with each element given by [frequency, time]
void playArray(int array[][2], int numNotes) {
  for (int i = 0; i < numNotes; i++) {
    printf("playing tone %d\n", i);
    playNote(array[i][0], array[i][1]);
  }
}

int main() {
  printf("starting program...\n");
  pioInit();
  pinMode(SPEAKER_PIN, 1);
  //finding size of array
  int numNotes = sizeof(cantina)/sizeof(cantina[0]);
  printf("array contains %d notes\n", numNotes);
  printf("playing tones...\n");
  playArray(cantina, numNotes);
  printf("finished playing tones...\n");
}
