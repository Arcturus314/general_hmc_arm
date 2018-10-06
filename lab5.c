// lab5.c 9/05/2018
// A C program to play arbitrary sets of notes with the Raspberry Pi
// Kaveh Pezeshki kpezeshki@g.hmc.edu



// Notes for Fur Elise, E155 Lab 5

// Pitch in Hz, duration in ms
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
#define TIMER_CLO ((volatile unsigned int *) (sys_timer + 0x4))
#define TIMER_C1  ((volatile unsigned int *) (sys_timer + 0x10))
#define TIMER_C2  ((volatile unsigned int *) (sys_timer + 0x14))
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
  //a square wave will be modulated by a step up - down function defined by System Timer 1

  //we first reset Timer 1
  TIMER_CS[0] |= 0b0100;
  //we then write Timer 2 Compare register with the current time plus an offset
  TIMER_C1[0] = TIMER_CLO + time*1000000; //multiply by 1e6 as timer counts in us

  //while time has not yet elapsed we generate a square wave of the correct frequency
  int time_elapsed = 0; //high when time elapsed >= time

  //initializing the speakerState variable
  int speakerState = 0;

  while (time_elapsed == 0) {
    time_elapsed = ((unsigned int) TIMER_CS[0] >> 1)&1;

    //generating a square wave at a specified frequency
    //change the state of the output after every 1e6/(2*frequency) clock cycles
    //Modulo operator provides this

    int changeTime = 1000000/(2*frequency);
    
    if ((unsigned int) TIMER_CLO[0] % changeTime == 0) {
      speakerState = ~speakerState;
      digitalWrite(SPEAKER_PIN, speakerState);
    }

  }


}


//plays a two dimensional array of numbers with each element given by [frequency, time]
void playArray(int array[][2]) {
  //first finding the size of the array
  size_t numNotes = sizeof(array)/sizeof(array[0]);
  for (size_t i = 0; i < numNotes; i++) {
    playNote(array[i][0], array[i][1]);
  }
}

int main() {
  printf("starting program...\n");
  pioInit();
  pinMode(SPEAKER_PIN, 1);
  printf("playing tones...\n");
  while (1) {
	  digitalWrite(SPEAKER_PIN, 1);
  }
  //TIMER_CS[0] |=0b100;
  //playNote(500, 10);
  //playArray(notes);
  printf("finished playing tones...\n");
}
