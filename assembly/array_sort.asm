TITLE Program Assignment 5: Sorting Random Integers

; Author: Adam Walker
; Last Modified: 11/18/2018
; Description: Generates a user-defined number of random integers into an array, prints it unsorted and sorted, prints the median

INCLUDE Irvine32.inc

.data

; Constants
MIN = 10
MAX = 200
LO = 100
HI = 999
NUMBERS_PER_COLUMN = 10

; Introductory messages and prompts
rand_intro1		BYTE	"Sorting Random Integers   Programmed by Adam Walker", 0
ec_1			BYTE	"**EC: Display numbers ordered by column (10 per column).", 0

rand_intro2		BYTE	"This program generates random numbers in the range [", 0
rand_intro3		BYTE	" .. ", 0
rand_intro4		BYTE	"],", 0
rand_intro5		BYTE	"displays the original list, sorts the list, and calculates the", 0
rand_intro6		BYTE	"median value. Finally, it displays the list sorted in descending order.", 0

num_intro1		BYTE	"How many numbers should be generated? [",0
num_intro2		BYTE	" .. ", 0
num_intro3		BYTE	"]: ", 0

unsorted_title	BYTE	"The unsorted random numbers:",0
sorted_title	BYTE	"The sorted list:",0

print_gap	BYTE	"   ",0

median_title1	BYTE	"The median is ",0
median_title2	BYTE	".",0

; Error message if number out of range
val_bad_num	BYTE	"Invalid input", 0

; Variables to hold user input and calculated fields
request		DWORD	0
random_number	DWORD	?
array		DWORD	MAX		DUP(?)	
first_value	DWORD	?
second_value	DWORD	?
column_starter	DWORD	0
column_counter	DWORD	0


.code
main PROC

	call	Randomize

	; Introduce program
	push	OFFSET ec_1
	push	OFFSET rand_intro6
	push	OFFSET rand_intro5
	push	OFFSET rand_intro4
	push	OFFSET rand_intro3
	push	OFFSET rand_intro2
	push	OFFSET rand_intro1
	call	introduction

	; Get and validate user input for number of integers
	push	OFFSET val_bad_num
	push	OFFSET num_intro3
	push	OFFSET num_intro2
	push	OFFSET num_intro1
	push	OFFSET request
	call	getData
	
	; Fill array with user specified number of integers
	push	request
	push	OFFSET array
	push	OFFSET random_number
	call	fillArray

	; Print unsorted array
	push	column_starter
	push	column_counter
	push	OFFSET print_gap
	push	request
	push	OFFSET array
	push	OFFSET unsorted_title
	call	displayList

	; Sort the array using bubble sort
	push	second_value
	push	first_value
	push	request
	push	OFFSET array
	call	sortList

	; Print the median
	push	OFFSET median_title2
	push	OFFSET median_title1
	push	request
	push	OFFSET array
	call	displayMedian

	; Print the sorted array
	push	column_starter
	push	column_counter
	push	OFFSET print_gap
	push	request
	push	OFFSET array
	push	OFFSET	sorted_title
	call	displayList

	exit	; exit to operating system

main ENDP


; Procedure to introduce program
; receives: various title variables on system stack
; returns: none
; preconditions: none	
; registers changed: edx, eax
introduction PROC

	push	ebp
	mov	ebp, esp

	mov	edx, [ebp + 8]
	call	writeString
	call	CrLf
	mov	edx, [ebp + 32]
	call	writeString
	call	CrLf
	call	CrLf
	mov	edx, [ebp + 12]
	call	writeString
	mov	eax, LO
	call	writeDec
	mov	edx, [ebp + 16]
	call	writeString
	mov	eax, HI
	call	writeDec
	mov	edx, [ebp + 20]
	call	writeString
	call	CrLf
	mov	edx, [ebp + 24]
	call	writeString
	call	CrLf
	mov	edx, [ebp + 28]
	call	writeString
	call	CrLf
	call	CrLf

	pop	ebp
	ret 	32

introduction ENDP


; Procedure to receive user specified number of integers
; receives: Various title messages, as well as request variable, all on system stack
; returns: none
; preconditions: none	
; registers changed: edx, eax
getData PROC

	push	ebp
	mov	ebp, esp

	mov	dl, 0

	NUM_INPUT:							; do while loop for input validation

		mov	edx, [ebp + 12]
		call	writeString
		mov	eax, MIN
		call	writeDec
		mov	edx, [ebp + 16]
		call	writeString
		mov	eax, MAX
		call	writeDec
		mov	edx, [ebp + 20]
		call	writeString
		call	readInt

		call	validate					; Validate input is within specified range

		CMP	dl, 1
		JNE	NUM_INPUT

	mov	ebx, [ebp + 8]						; Save the user's input to request variable
	mov	[ebx], eax							
	call	CrLf

	pop	ebp
	ret	20

getData ENDP


; Procedure to validate eax is within a certain range
; receives: Input through eax register, as well as MAX and MIN global constants, and error message on system stack
; returns: dl is set to 1 if input is good
; preconditions: none	
; registers changed: edx
validate PROC

	CMP	eax, MAX				; Checks if input is above or below specified limits
	JG	BAD_INPUT					
	CMP	eax, MIN					
	JL	BAD_INPUT					

	mov	dl, 1					; Good input
	JMP	END_VALIDATE

	BAD_INPUT:
	
		mov	edx, [ebp + 24]			; Print error message
		call	writeString
		call	CrLf

	END_VALIDATE:

	ret

validate ENDP


; Procedure to fill an array with random integers within a predefined range
; receives: Array OFFSET, array length, and random_number variable by system stack, as well as LO and HI global constants
; returns: none
; preconditions: none	
; registers changed: ecx, edx, eax, ebx
fillArray PROC

	push	ebp
	mov	ebp, esp

	mov	ecx, [ebp + 16]			; Using request variable as loop counter
	mov	edx, 0				; edx used to track where in array to slot number

	GENERATE_RANDOM:			; For length of array, generate and store random integers within range of LO to HI inclusive

		mov	eax, HI
		sub	eax, LO
		inc	eax
		call	RandomRange			
		add	eax, LO
		mov	ebx, [ebp + 12]		
		mov	[ebx + edx], eax	
		add	edx, 4

		LOOP GENERATE_RANDOM

	pop	ebp
	ret	12

fillArray ENDP


; Procedure to print an array in column format
; receives:  A title, Array OFFSET, array length, a gap variable, column counter, and column starter variables, all by system stack
; returns: none
; preconditions: none	
; registers changed: edx, ecx, eax, esi, edi
displayList PROC

	push	ebp
	mov	ebp, esp
	
	mov	edx, [ebp + 8]						; Print the title
	call	writeString
	call	CrLf

	mov	ecx, [ebp + 16]						; Use array length as loop counter

	; Calculate the number of columns to print by dividing array length by NUMBERS_PER_COLUMN, adding 1
	mov	edx, 0
	mov	eax, ecx
	mov	ebx, NUMBERS_PER_COLUMN
	DIV	ebx
	inc	eax

	mov	[ebp+24], eax						; Store the number of columns to print in column counter variable
	mov	esi, 0								
	mov	edi, 0								
	mov	edx, 0
	mov	[ebp+28], edx						; Column starter variable indicates where in array to begin printing each row

	PRINT_ARRAY:

			mov	ebx, [ebp + 12]				
			mov	eax, [ebx + esi]			
			call	writeDec			
			
			mov	eax, NUMBERS_PER_COLUMN		
			mov	ebx, 4
			mul	ebx
			add	esi, eax				; Increment ESI to correct location of next DWORD to be printed

			mov	edx, [ebp + 20]				; Prints the gap between numbers
			call	writeString

			inc	edi							 
			CMP	edi, [ebp+24]				; edi holds number of columns printed in row. Compare to column counter variable			
			JAE	LINE_BREAK

			mov	eax, [ebp + 16]				
			dec	eax
			mov	ebx, 4
			MUL	ebx
			CMP     esi, eax
			JA	LINE_BREAK				; If esi is out of range, then jump to line break

			JMP	END_PRINT

			LINE_BREAK:

			call	CrLf
			
			mov	edi, 0					; Now that a line break has occurred, reset counter variables
			mov	esi, 4
			add	[ebp+28], esi
			mov	esi, 0						
			add	esi, [ebp+28]

			END_PRINT:

			LOOP PRINT_ARRAY

    ; Reset column counter to zero
	mov	esi, 0
	mov	[ebp+24], esi

	pop	ebp
	ret	20

displayList ENDP


; Procedure to sort an array in descending order using bubble sort
; receives:  Array OFFSET, array length, and two variables to hold values being compared
; returns: none
; preconditions: none	
; registers changed: ecx, edi, edx, eax, ebx
sortList PROC

	push	ebp
	mov	ebp, esp
	mov	ecx, [ebp + 12]				; Use the array length (request variable) as loop counter
	dec	ecx

	OUTER_LOOP:

		mov	edi, ecx			; Save outer loop counter
		mov	edx, 0					

		INNER_LOOP:

			mov	ebx, [ebp + 8]		; Starting address of array
			mov	eax, [ebx + edx]	
			mov	[ebp + 16], eax		; Save first value
			add	edx, 4
			mov	eax, [ebx + edx]
			mov	[ebp + 20], eax		; Save second value

			; Compare two values
			; If first value is less than second value, we need to switch them
			; Push address of first value to stack and call exchangeElements
			; Otherwise no switch takes place
			mov	ebx, [ebp + 16]
			CMP	ebx, [ebp + 20]
			JAE	NO_SWITCH
			mov	ebx, [ebp + 8]		
			add	ebx, edx
			sub	ebx, 4
			push	ebx
			call	exchangeElements		 

			NO_SWITCH:

			LOOP	INNER_LOOP

		mov	ecx, edi

		LOOP	OUTER_LOOP

	pop	ebp
	ret	8

sortList ENDP


; Procedure to switch two DWORDs held in contiguous memory
; receives:  Address of first DWORD on the system stack
; returns: none
; preconditions: none	
; registers changed: ebx, eax, esi
exchangeElements PROC

	push	ebp
	mov	ebp, esp

	; eax and esi are used as temporary variables during the exchange
	mov	ebx, [ebp + 8]		
	mov	eax, [ebx]			
	add	ebx, 4
	mov	esi, [ebx]			

	; Flip the two DWORDS
	mov	[ebx], eax			
	sub	ebx, 4
	mov	[ebx], esi

	pop	ebp
	ret	4

exchangeElements ENDP


; Procedure to calculate and print median value of a sorted array
; receives:  Array OFFSET, array length, and two string title variables on the system stack
; returns: none
; preconditions: Array is sorted	
; registers changed: edx, eax, ebx
displayMedian PROC

	push	ebp
	mov	ebp, esp

	; Divide the number of integers in the set (the request variable) by 2. 
	; Use the remainder value to determine if request is odd or even
	mov	edx, 0
	mov	eax, [ebp + 12]	
	mov	ebx, 2
	DIV	ebx
	CMP	EDX, 0

	JE	EVEN_NUMBER

	mov	ebx, 4
	mul	ebx						; In the odd number case, multiply the quotient by 4 (as DWORD) to find the median's location

	mov	edx, [ebp + 8]					; Array OFFSET
	mov	ebx, [edx + eax]				; Location of median
	mov	eax, ebx

	JMP	PRINT_MEDIAN

	EVEN_NUMBER:

	mov	ebx, 4
	mul	ebx								
	mov	edx, [ebp + 8]					; In the even numbered case, we must add the 2 middle numbers and then divide by 2
	mov	ebx, [edx + eax]				
	sub	eax, 4
	add	ebx, [edx + eax]				
	mov	edx, 0
	mov	eax, ebx
	mov	ebx, 2
	DIV	ebx

	; If a remainder exists, the median ends in .5, and we must add 1 to round up
	CMP	edx, 0
	JE	PRINT_MEDIAN
	inc	eax		

	PRINT_MEDIAN:

	call	CrLF
	mov	edx, [ebp + 16]
	call	writeString
	call	writeDec
	mov	edx, [ebp + 20]
	call	writeString
	call	CrLF
	call	CrLF

	pop	ebp
	ret	16

displayMedian ENDP

END main
