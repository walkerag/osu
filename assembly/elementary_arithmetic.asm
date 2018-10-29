TITLE Program Assignment 1: Elementary Arithmetic

; Author: Adam Walker
; Last Modified: 09/23/2018
; Project Number: 1                Due Date: 10/07/2018
; Description: Returns sum, difference, product, (integer) quotient and remainder of two numbers. 

INCLUDE Irvine32.inc

.data

; Introductory messages
intro_1			BYTE	"	Elementary Arithmetic by Adam Walker	", 0
ec_1			BYTE	"**EC: Program repeats until user chooses to quit.", 0
ec_2			BYTE	"**EC: Program verifies second number less than first.", 0
ec_3			BYTE	"**EC: Program calculates and displays quotient as floating point number", 0
instructions		BYTE	"Enter 2 numbers, and I'll show you the sum, difference, product, quotient, and remainder.", 0
num_1_prompt		BYTE	"First number: ", 0
num_2_prompt		BYTE	"Second number: ", 0

; Error messages
second_larger		BYTE	"The second number must be less than the first!", 0
divide_zero		BYTE	"We can't divide by zero!", 0

; Variables to hold 2 numbers and calculated fields
num_1			DWORD	?
num_2			DWORD	?
sum			DWORD	0
difference		DWORD	0
product			DWORD	0
quotient		DWORD	0
remain			DWORD	0
quotient_float		QWORD	?

; Used when reporting results
plus			BYTE	" + ", 0
minus			BYTE	" - ", 0
divide			BYTE	" / ", 0
multiply		BYTE	" x ", 0
equals			BYTE	" = ", 0
remainder		BYTE	" remainder ", 0
floating		BYTE	"Floating point quotient: ", 0

; End of program
continue_prompt	BYTE	"Enter 1 to continue playing, 0 to quit", 0
goodbye			BYTE	"Goodbye!", 0

; For floating point calculation
thousand 	WORD 1000
newCtrlWord  	WORD 010000000000b
oldCtrlWord  	WORD ?

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

; SECTION A: Introduce program and extra credit options
	mov	edx, OFFSET intro_1
	call	writeString
	call	CrLf
	mov	edx, OFFSET ec_1
	call	writeString
	call	CrLf
	mov	edx, OFFSET ec_2
	call	writeString
	call	CrLf
	mov	edx, OFFSET ec_3
	call	writeString
	call	CrLf
	call	CrLf

; Loop until user decides to quit
; Reference: https://en.wikibooks.org/wiki/X86_Disassembly/Loops#While_Loops
beginning:

; SECTION B: GET/VALIDATE DATA

; Asking user to enter 2 numbers
	mov	edx, OFFSET instructions
	call	writeString
	call	CrLf
	call	CrLf

; Prompt user for the two numbers
	mov	edx, OFFSET num_1_prompt
	call	writeString
	call	readInt
	mov	num_1, eax

	mov	edx, OFFSET num_2_prompt
	call	writeString
	call	readInt
	mov	num_2, eax

; Check second number is not 0 to avoid dividing by zero
; If it is zero, show error message
	cmp	num_2, 0
	jne 	DIVIDE_ZERO_JMP
	
	mov	edx, OFFSET divide_zero
	call	CrLf
	call	writeString
	call	CrLf
	call	CrLf
	JMP	CONTINUE_JMP

	DIVIDE_ZERO_JMP:

; Check second number is not larger than first
; Status flag (SF) is set if result is negative
; If not set, skip error message
	mov	eax, num_1
	cmp	eax, num_2
	jns	SECOND_LARGER_JMP

	; Second number too large
	mov	edx, OFFSET second_larger
	call	CrLf
	call	writeString
	call	CrLf
	call	CrLf
	JMP	CONTINUE_JMP

	SECOND_LARGER_JMP:

; SECTION C: Perform arithmetic calculations

; Sum
	mov	eax, num_1
	add	eax, num_2
	mov	sum, eax

; Subtract
	mov	eax, num_1
	sub	eax, num_2
	mov	difference, eax

; Multiply
; Reference: https://en.wikibooks.org/wiki/X86_Assembly/Arithmetic
	mov	eax, num_1
	imul	eax, num_2
	mov	product, eax

; Divide
	mov	eax, num_1
	mov	edx, 0
	div	num_2
	mov	remain, edx
	mov	quotient, eax

; Divide, with floating point quotient rounded to .001
; Reference: http://www.masmforum.com/board/index.php?PHPSESSID=786dd40408172108b65a5a36b09c88c0&topic=18114;prev_next=next
; Reference: https://stackoverflow.com/questions/23358537/assembly-round-floating-point-number-to-001-precision-toward-%E2%88%9E
; Reference: https://stackoverflow.com/questions/40663550/write-the-value-in-the-8087s-top-of-stack-to-a-variable-in-memory	
	fild	num_1
	fidiv	num_2
	fimul 	thousand			; Multiply by thousand
	;call	ShowFPUStack			; Used to debug
	frndint					; Rounds to integer
	fidiv 	thousand			; Divide by thousand
	FST	quotient_float			; Save to named variable

; SECTION D: Report results, and ask if user wants to continue

; Sum
	mov	eax, num_1
	call	writeDec
	mov	edx, OFFSET plus
	call	writeString
	mov	eax, num_2
	call	writeDec
	mov	edx, OFFSET equals
	call	writeString
	mov	eax, sum
	call	writeDec
	call	CrLf

; Subtract
	mov	eax, num_1
	call	writeDec
	mov	edx, OFFSET minus
	call	writeString
	mov	eax, num_2
	call	writeDec
	mov	edx, OFFSET equals
	mov	eax, difference
	call	writeString
	call	writeDec
	call	CrLf

; Multiply
	mov	eax, num_1
	call	writeDec
	mov	edx, OFFSET multiply
	call	writeString
	mov	eax, num_2
	call	writeDec
	mov	edx, OFFSET equals
	call	writeString
	mov	eax, product
	call	writeDec
	call	CrLf

; Divide
	mov	eax, num_1
	call	writeDec
	mov	edx, OFFSET divide
	call	writeString
	mov	eax, num_2
	call	writeDec
	mov	edx, OFFSET equals
	call	writeString
	mov	eax, quotient
	call	writeDec
	mov	edx, OFFSET remainder
	call	writeString
	mov	eax, remain
	call	writeDec
	call	CrLf

; Divide, with floating point quotient rounded to .001
	mov	edx, OFFSET floating
	call	writeString
	fld	quotient_float
	call	writeFloat
	call	CrLf
	call	CrLf

; Prompt user to continue
; Reference: http://unixwiz.net/techtips/x86-jumps.html
	CONTINUE_JMP:
	mov	edx, OFFSET continue_prompt
	call	writeString
	call	CrLf
	call	readInt
	cmp	eax, 1
	JE	beginning

; SECTION E: Goodbye

	mov	edx, OFFSET goodbye
	call	writeString

	exit	; exit to operating system
main ENDP

END main
