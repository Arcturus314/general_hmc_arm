#include <stdio.h>

int array[] = {-1,2,9,-4,6,4,8,-5,2,-4,0,2};

int loop_pos, loop_count, temp;

int main() {
	//we print the array
	for (loop_pos=0; loop_pos < 12; loop_pos++) {
		printf("%d ", array[loop_pos]);
	}
	printf("\n");

	for (loop_count=0; loop_count<12; loop_count++) {
		for (loop_pos=0; loop_pos<11; loop_pos++) {
			if (array[loop_pos] > array[loop_pos+1]) {
				temp = array[loop_pos];
				array[loop_pos] = array[loop_pos+1];
				array[loop_pos+1] = temp;
			}
		}
	}

	//we print the array
	for (loop_pos=0; loop_pos < 12; loop_pos++) {
		printf("%d ", array[loop_pos]);
	}
	printf("\n");
}
			
