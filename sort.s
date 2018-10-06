//sort.s
//9/26/2018 kpezeshki@hmc.edu
//sorts twelve signed bytes

//The algorithm steps through the array 12 times. For every step through the array, it compares every two adjacent elements. If the two elements are in the incorrect order (first > second) then it swaps the values in the two memory locations.


//Relevant Registers:

//R3: base address of array
//R4: iteration number for iterations through the entire array
//R5: iteration position within the array
//R6: iteration position +1 within the array
//R7: value of array[R5]
//R8: value of array[R5+1]

.text
.global main
main: 
	LDR R3, =array //load base address of array into R3
	MOV R4, #0      //setting iteration number to 0
	MOV R5, #0      //setting iteration position to 0
iteration:
	CMP R4, #12        //if R4 is greater than or equal to 12, stop program execution
	BGE done           //jumping to the end of the program
	ADD R4, R4, #1     //incrementing R4		
	MOV R5,     #0     //setting R5 back to 0
position:
	CMP   R5, #11        //if R5 is greater than or equal to 11, stop program execution
	BGE   position_done  //11 is used because comparing element i and i+1: hence i+1=12 when i=1
	ADD   R6, R5, #1     //compute R5+1, the address of the element to compare against, in R6 
	LDRSB R7, [R3, R5]   //load data at position R5 from the array into R7
	LDRSB R8, [R3, R6]   //load data at position R5+1 from the array into R8
	CMP   R7, R8         //only swap when R7 > R8
	BLE   swapped        //so skip swap and jump to next step of loop 
	STRB  R8, [R3, R5]   //store R7 into array position R6
	STRB  R7, [R3, R6]   //store R8 into array position R5
swapped:
	ADD   R5, R5, #1     //incrementing R5
	B   position         //this iteration of the inner loop is done
position_done:
	B iteration        //this iteration of the outer loop is done

//rest of main method

done:
	NOP
	BX LR

.data
array:

.byte 0x42
.byte 0x52
.byte 0x62
.byte 0x72
.byte 0x82
.byte 0x92
.byte 0x32
.byte 0x22
.byte 0x12
.byte 0x02
.byte 0xF2
.byte 0x42

//rest of array

.end
