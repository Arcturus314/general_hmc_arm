//Kaveh Pezeshki kpezeshki@g.hmc.edu
//10/16/18 turns off GPIO20

#include "rPIO.h"

int main(void)
{
	SetUpPerif();
	setPinType(20, 1);
	digitalWrite(20,0);

	printf("Content-Type: text/plain;charset=us-ascii\n\n");
	printf("LED 2 OFF\n\n");
}
