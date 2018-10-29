TITLE Program Assignment 3: Integer Accumulator	(Assignment3.asm)

; Author: Adam Walker
; Last Modified: 10/28/2018
; Project Number: 3                Due Date: 10/28/2018
; Description: Calculate sum, average, and count of user inputted negative integers. 

INCLUDE Irvine32.inc

.data

; Constants
LOWER_LIMIT = -100

; Introductory messages and prompts
intro_1			BYTE	"Welcome to the Integer Accumulator by Adam Walker", 0
ec_1			BYTE	"**EC: Number the lines during user input.", 0
ec_2			BYTE	"**EC: Calculate and display the average as a floating-point number, rounded to the nearest .001.", 0
name_prompt		BYTE	"What is your name? ", 0
name_response		BYTE	"Hello, ", 0
acc_instruct1		BYTE	"Please enter numbers in [-100, -1].", 0
acc_instruct2		BYTE	"Enter a non-negative number when you are finished to see results.", 0
acc_prompt1		BYTE	"Enter number ", 0
acc_prompt2		BYTE	": ", 0

; Variables to hold user input and calculated fields
username		BYTE	21 DUP(0)
byteCount		DWORD	?
numberCount		DWORD	1
numberSum	 	DWORD	0
numberAvg	 	DWORD	0
numberAvgFloat		QWORD	?

; End of program messages, including error message if no negative integers entered
acc_num_begin		BYTE	"You entered ", 0
acc_num_end 		BYTE	" valid numbers.", 0
acc_sum		 	BYTE	"The sum of your valid numbers is ", 0
acc_avg		 	BYTE	"The rounded average is ", 0
acc_float	 	BYTE	"The floating point average is ", 0
no_numbers		BYTE	"Oops! No valid numbers were entered", 0
goodbye			BYTE	"Thank you for playing Integer Accumulator! It's been a pleasure to meet you, ", 0

; Variables used for floating point calculation
thousand		WORD	1000
newCtrlWord		WORD	010000000000b
oldCtrlWord		WORD	?

.code
main PROC

; Set control word rounding behavior
; Reference: https://stackoverflow.com/questions/23358537/assembly-round-floating-point-number-to-001-precision-toward-%E2%88%9E
    fstcw	oldCtrlWord               ; store control word
    mov		ax, oldCtrlWord
    and		ah, 11110011b             ; clear _only_ RC field
    or		ah, 00000100b             ; set _only_ RC field
    mov		newCtrlWord, ax
    fldcw	newCtrlWord               ; load control word
	
; SECTION A: Introduction
; Gives program title and programmer name
; Lists extra credit options
	mov	edx, OFFSET intro_1
	call	writeString
	call	CrLf
	mov	edx, OFFSET ec_1
	call	writeString
	call	CrLf
	mov	edx, OFFSET ec_2
	call	writeString
	call	CrLf

; SECTION B: Deliver User Instructions 
; Prompt user to enter their name and greets them
; Gives instructions on range of negative numbers accepted
	mov	edx, OFFSET name_prompt
	call	writeString
	mov	edx,OFFSET username			; Storing user's name
	mov	ecx,SIZEOF username
	call	readString
	mov	byteCount,eax
	mov	edx, OFFSET name_response
	call	writeString
	mov	edx, OFFSET username
	call	writeString
	call	CrLf
	call	CrLf
	mov	edx, OFFSET acc_instruct1		; Integer must be between -100 and -1 inclusive
	call	writeString
	call	CrLf
	mov	edx, OFFSET acc_instruct2
	call	writeString
	call	CrLf


; SECTION C: Get User Data
; User inputs number of Fibonnaci terms to print
; Check number is between 1 and 46
; If not, continue prompting until user inputs valid number

GET_INPUT: 
	mov	edx, OFFSET acc_prompt1		
	call	writeString
	mov	eax, numberCount
	call	writeDec
	mov	edx, OFFSET acc_prompt2	
	call	writeString
	call	readInt
	CMP	eax, LOWER_LIMIT			; Checks if inputted number below lower limit
	JL	GET_INPUT				; Ignore input below lower limit, jump to input request
	CMP	eax, 0					; Check if input is positive
	JGE	CALCULATE_RESULTS			; If positive input, go get results
	inc	numberCount
	add	numberSum, eax				; Add user's inputted number to running total
	JMP	GET_INPUT				; Keep requesting input until positive number reached


; SECTION D: Calculate Results
; Calculate sum, rounded integer average
; Also do floating point divison, with quotient rounded to .001
; Reference: http://www.masmforum.com/board/index.php?PHPSESSID=786dd40408172108b65a5a36b09c88c0&topic=18114;prev_next=next
; Reference: https://stackoverflow.com/questions/23358537/assembly-round-floating-point-number-to-001-precision-toward-%E2%88%9E
; Reference: https://stackoverflow.com/questions/40663550/write-the-value-in-the-8087s-top-of-stack-to-a-variable-in-memory	

CALCULATE_RESULTS:

	dec	numberCount					; Decrement to equal the number of valid numbers (otherwise will be off by 1)
	CMP	numberCount, 0					; Check if at least one valid number entered
	JBE	NO_VALID_NUMBERS
	mov	edx, 0
	mov	eax, numberSum
	CDQ							; Sign extend
	idiv	numberCount					; Quotient is in EAX
	mov	numberAvg, eax	
	fild	numberSum					; Begin floating point division
	fidiv	numberCount
	fimul	thousand					; Multiply by thousand
	frndint							; Rounds to integer
	fidiv	thousand					; Divide by thousand
	FST	numberAvgFloat					; Save to named variable


; SECTION E: Return results
; Print count, sum, average, and floating point average
; Error message if no valid numbers were entered

	mov	edx, OFFSET acc_num_begin
	call	writeString
	mov	eax, numberCount
	call	writeDec
	mov	edx, OFFSET acc_num_end
	call	writeString
	call	CrLf
	mov	edx, OFFSET acc_sum
	call	writeString
	mov	eax, numberSum
	call	writeInt
	call	CrLf
	mov	edx, OFFSET acc_avg
	call	writeString
	mov	eax, numberAvg
	call	writeInt
	call	CrLf
	mov	edx, OFFSET acc_float
	call	writeString
	fld	numberAvgFloat
	call	writeFloat
	JMP	GOODBYE_LABEL				; Prevents showing error message

NO_VALID_NUMBERS:
	mov	edx, OFFSET no_numbers
	call	writeString

; SECTION F: Goodbye
; Say goodbye using the user's previously inputted name

GOODBYE_LABEL:
	call	CrLf
	mov	edx, OFFSET goodbye
	call	writeString
	mov	edx, OFFSET username
	call	writeString
	
	exit	; exit to operating system
main ENDP

END main
