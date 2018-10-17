//Kaveh Pezeshki kpezeshki@g.hmc.edu
//10/16/18 turns off GPIO21

#include "rPIO.h"

int main(void)
{
	SetUpPerif();
	setPinType(21, 1);
	digitalWrite(21,0);

	printf("Content-Type: text/plain;charset=us-ascii\n\n");
	printf("LED 1 OFF\n\n");
}
