#include <stdio.h>
int main(void) {
	printf("Content-Type: text/plain;charset=us-ascii\n\n");
	printf("Hello world\n\n");
	int val = ~(1<<7);
	printf("val %x\n", val);
	return 0;
}
