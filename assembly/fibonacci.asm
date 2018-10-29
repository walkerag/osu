TITLE Program Assignment 2: Fibonacci Numbers

; Author: Adam Walker
; Last Modified: 10/14/2018
; Project Number: 2                Due Date: 10/14/2018
; Description: Calculates and returns Fibonacci series. 

INCLUDE Irvine32.inc

.data

; Constants
UPPER_LIMIT = 46
NUMBERS_PER_LINE = 5

; Introductory messages and prompts
intro_1			BYTE	"Fibonacci Numbers", 0
intro_2			BYTE	"Programmed by Adam Walker", 0
name_prompt		BYTE	"What's your name? ", 0
name_response		BYTE	"Hello, ", 0
fib_instruct1		BYTE	"Enter the number of Fibonacci terms to be displayed", 0
fib_instruct2		BYTE	"Give the number as an integer in the range [1 .. 46].", 0
fib_prompt		BYTE	"How many Fibonacci terms do you want? ", 0

; Error message
fib_error		BYTE	"Out of range. Enter a number in [1 .. 46]", 0

; Variables to hold 2 numbers and calculated fields
username		BYTE	21 DUP(0)
byteCount		DWORD	?
fibRange		DWORD	?
fibPrintInt1		DWORD	1
fibPrintInt2		DWORD	1

; Used when reporting results
gap			BYTE	"     ", 0

; End of program
final_message		BYTE	"Results certified by Adam Walker", 0
goodbye			BYTE	"Goodbye, ", 0

.code
main PROC
	
; SECTION A: introduction
; Gives program title and programmer name
	mov	edx, OFFSET intro_1
	call	writeString
	call	CrLf
	mov	edx, OFFSET intro_2
	call	writeString
	call	CrLf
	call	CrLf

; SECTION B: userInstructions 
; Prompt user to enter their name and greets them
; Gives instructions on how to choose number of Fibonacci terms
	mov	edx, OFFSET name_prompt
	call	writeString
	mov	edx,OFFSET username
	mov	ecx,SIZEOF username
	call	readString
	mov	byteCount,eax
	mov	edx, OFFSET name_response
	call	writeString
	mov	edx, OFFSET username
	call	writeString
	call	CrLf
	mov	edx, OFFSET fib_instruct1
	call	writeString
	call	CrLf
	mov	edx, OFFSET fib_instruct2
	call	writeString
	call	CrLf

; SECTION C: getUserData
; User inputs number of Fibonnaci terms to print
; Check number is between 1 and 46
; If not, continue prompting until user inputs valid number

	FIB_PROMPT_LABEL:
		call	CrLf
		mov	edx, OFFSET fib_prompt
		call	writeString
		call	readInt
		mov	fibRange, eax
		CMP	fibRange, 1
		JL	BAD_fibRange_LABEL		; Bad input if less than 1
		CMP	fibRange, UPPER_LIMIT
		JG	BAD_fibRange_LABEL		; Bad input if greater than 46
		JMP	GOOD_fibRange_LABEL		; Implementing as a post-test loop, whereby conditions are checked after initial prompt

	BAD_fibRange_LABEL:
		mov	edx, OFFSET fib_error	; Prompt user to try again
		call	writeString
		JMP	FIB_PROMPT_LABEL

; SECTION D: displayFibs
; Calculates and prints Fibonacci series of length specified by user

GOOD_fibRange_LABEL:
	mov	ecx, fibRange			; Using user's inputted number as a loop counter
	call	CrLf

FIB_LOOP:

	test	ecx, 1				; https://stackoverflow.com/questions/49116747/assembly-check-if-number-is-even
	JZ	EVEN_LABEL			; Storing two numbers for series, and using loop counter to determine which gets incremented	

	ODD_LABEL:
	mov	eax, fibPrintInt1		; Print next number in series, then add two stored numbers together for next value
	call	writeDec
	mov	edx, OFFSET gap
	call	writeString
	add	eax, fibPrintInt2
	mov	fibPrintInt1, eax

	JMP	LINE_END

	EVEN_LABEL:
	mov	eax, fibPrintInt2
	call	writeDec
	mov	edx, OFFSET gap
	call	writeString
	add	eax, fibPrintInt1
	mov	fibPrintInt2, eax

	LINE_END:					; used to print a line break after every X numbers printed, as determined by NUMBERS_PER_LINE
	mov	eax, fibRange
	sub	eax, ecx				; See how many numbers have been printed so far
	inc	eax
	mov	edx, 0
	mov	ebx, NUMBERS_PER_LINE
	div	ebx
	CMP	edx, 0					; Check if remainder is zero	
	JNE	END_LOOP				; If not equal, jump to end, otherwise print a line carriage
	call	CrLf

	END_LOOP:

	loop FIB_LOOP

; SECTION E: farewell
; Say goodbye using the user's previously inputted name
	call	CrLf
	call	CrLf
	mov	edx, OFFSET final_message
	call	writeString
	call	CrLf
	mov	edx, OFFSET goodbye
	call	writeString
	mov	edx, OFFSET username
	call	writeString
	
	exit	; exit to operating system
main ENDP

END main
