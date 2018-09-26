//sort.s
//9/26/2018 kpezeshki@hmc.edu
//sorts twelve signed bytes

.text
.global main
main: 
	LDR R3, =array //load base address of array into R3

//rest of main method

done:
	NOP
	BX LR

.data

array:
	.byte 0x08
	.byte 0x10
	.byte 0xFF

//rest of array

.end
