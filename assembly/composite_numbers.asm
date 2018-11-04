TITLE Program Assignment 4: Composite Numbers

; Author: Adam Walker
; Last Modified: 11/03/2018
; Project Number: 4                Due Date: 11/04/2018
; Description: Calculate and print a user-specified total of composite numbers. 

INCLUDE Irvine32.inc

.data

; Constants
UPPER_LIMIT = 2000
NUMBERS_PER_LINE = 10
NUMBERS_PER_PAGE = 400

; Introductory messages and prompts
intro_1			BYTE	"Composite Numbers   Programmed by Adam Walker", 0
ec_1			BYTE	"**EC: Align the output columns.", 0
ec_2			BYTE	"**EC: Display more composites (composite limit now set to 2000 rather than 400) and show them one page at a time (400 per page).", 0
comp_intro1		BYTE	"Enter the number of composite numbers you would like to see.", 0
comp_intro2		BYTE	"I'll accept orders for up to ", 0
comp_intro3		BYTE	" composites.", 0
comp_get_num1		BYTE	"Enter the number of composites to display [1 .. ", 0
comp_get_num2		BYTE	"]: ", 0

; Error message if number out of range
comp_bad_num	BYTE	"Out of range. Try again.", 0

; Used when printing composites
comp_page_break	BYTE	"Press any key to continue ...", 0
gap			BYTE	" ", 0

; Variables to hold user input and calculated fields
comps_to_find		DWORD	0	; Total number of composites to return
good_num		BYTE	0	; Used during input validation
is_comp			BYTE	0	; Flags if number being checked is a composite number
comp_counter		BYTE	0	; Used within inner loop
current_num		DWORD	1	; Number being checked
numbers_printed_line	DWORD	0	; Tracks numbers printed on the current line
numbers_printed_page	DWORD	0	; Tracks numbers printed on the current page

; End of program message
goodbye			BYTE	"Results certified by Adam. Goodbye.", 0

.code
main PROC
	call intro
	call getUserData
	call showComposites
	call farewell

	exit	; exit to operating system

main ENDP


; Procedure to introduce program
; receives: intro_1, ec_1, and ec_2 are global variables
; returns: none
; preconditions: none	
; registers changed: edx
intro PROC

	mov	edx, OFFSET intro_1
	call	writeString
	call	CrLf
	mov	edx, OFFSET ec_1
	call	writeString
	call	CrLf
	mov	edx, OFFSET ec_2
	call	writeString
	call	CrLf

	ret

intro ENDP

; Procedure to prompt user for total composite numbers to be returned
; receives: comp_intro1, comp_intro2, comp_intro3, comp_get_num1, comp_get_num2, good_num, and comps_to_find are global variables. 
; UPPER_LIMIT is global constant.
; returns: none
; preconditions: none	
; registers changed: edx, eax
getUserData PROC

	call	CrLf
	mov	edx, OFFSET comp_intro1
	call	writeString
	call	CrLf
	mov	edx, OFFSET comp_intro2
	call	writeString
	mov	eax, UPPER_LIMIT
	call	writeDec
	mov	edx, OFFSET comp_intro3
	call	writeString
	call	CrLf
	call	CrLf

	NUM_INPUT:									; do while loop for input validation

		mov	edx, OFFSET comp_get_num1
		call	writeString
		mov	eax, UPPER_LIMIT
		call	writeDec
		mov	edx, OFFSET comp_get_num2
		call	writeString
		call	readInt

		call	validate						; Validate input is within specified range

		CMP	good_num, 1
		JNE	NUM_INPUT


	mov	comps_to_find, eax					; Save the user's input

	call	CrLf

	ret

getUserData ENDP


; Procedure to prompt user for total composite numbers to be returned
; receives: comp_intro1, comp_intro2, comp_intro3, comp_get_num1, comp_get_num2, good_num, and comps_to_find are global variables. 
; UPPER_LIMIT is global constant.
; returns: none
; preconditions: none	
; registers changed: edx, eax
validate PROC

	CMP		eax, UPPER_LIMIT			; Checks if inputted number above upper limit
	JG		BAD_INPUT					; Ignore input below lower limit, jump to input request
	CMP		eax, 0						; Check if input is positive
	JLE		BAD_INPUT					; If positive input, go get results

	mov		good_num, 1					; Good input
	JMP		END_VALIDATE

	BAD_INPUT:
	
		mov	edx, OFFSET comp_bad_num
		call	writeString
		call	CrLf

	END_VALIDATE:

	ret

validate ENDP


; Procedure to find and print the total number of composites requested by the user
; receives: comps_to_find, is_comp are global variables. 
; returns: none
; preconditions: comps_to_find must be greater than 0	
; registers changed: ecx, eax
showComposites PROC

	mov 	ecx, comps_to_find				; Loop based on the number of composites required
	mov	eax, 0

	L1:

		call	isComposite				; Check if number is a composite

		CMP	is_comp, 1
		JE	COMPOSITE_FOUND			; Go print the composite

		inc	ecx						; Stop loop counter decrementing, since number checked was not a composite
		JMP	LOOP_END

		COMPOSITE_FOUND:
			
			call	printComposite		; Print composite, allowing for appropriate line and page breaks

		LOOP_END:

		inc	current_num				; Try next highest number

		LOOP L1

	ret

showComposites ENDP


; Procedure to determine if number is a composite
; receives: is_comp, current_num, comp_counter are global variables. 
; returns: none
; preconditions: current_num must be greater than 0	
; registers changed: ebx, eax (ecx is changed but then restored)
isComposite PROC

	mov		is_comp, 0					; Flag for if number is a composite
	mov		ebx, ecx					; Save ecx register
	mov		ecx, current_num			; Use current value for loop
	mov		comp_counter, 0				; Set to zero
	
	L2:

		mov		eax, current_num		; Move number to be divided
		mov		edx, 0

		DIV		ecx						; Divide eax by loop value

		CMP		edx, 0					; Check if remainder zero
		JNE		END_L2	
		
		inc		comp_counter			; Increment composite number counter if remainder is zero

		END_L2:

		LOOP L2

	CMP		comp_counter, 2				; If 2 or less numbers are divisors, than number is prime

	JLE		NOT_COMP
	mov		is_comp, 1					; number is a composite number

	NOT_COMP:

	mov		ecx, ebx					; Restore ecx

	ret

isComposite ENDP


; Procedure to display a composite number to user
; receives: current_num, gap, numbers_printed_line, numbers_printed_page, comp_page_break are global variables
; returns: none
; preconditions: none	
; registers changed: edx, eax
printComposite PROC

			mov	eax, current_num		; Print composite number
			call	writeDec

			mov	edx, OFFSET gap
			call	writeString
			call	writeString

			CMP	eax, 1000				; Aligns columns by printing extra spaces for larger numbers
			JGE	LINE_BREAK
			call	writeString
			CMP	eax, 100
			JGE	LINE_BREAK
			call	writeString
			CMP	eax, 10
			JGE	LINE_BREAK
			call	writeString

			LINE_BREAK:

			inc	numbers_printed_line						; Increment line and page counters
			inc	numbers_printed_page

			CMP	numbers_printed_line, NUMBERS_PER_LINE		; Print a line break if limit reached
			JL	PAGE_BREAK
			call	CrLf
			mov	numbers_printed_line, 0

			PAGE_BREAK:

			CMP	numbers_printed_page, NUMBERS_PER_PAGE		; Ask user to press any key if page limit reached
			JL	END_PRINT_COMPOSITE
			mov	numbers_printed_page, 0
			mov	edx, OFFSET comp_page_break
			call	writeString
			call	CrLf
			call    ReadChar

			END_PRINT_COMPOSITE:

			ret;

printComposite ENDP


; Procedure to say goodbye to user at end of program
; receives: goodbye is a global variable
; returns: none
; preconditions: none	
; registers changed: edx
farewell PROC

	call	CrLf
	call	CrLf
	mov	edx, OFFSET goodbye
	call	writeString				; Print goodbye message
	
	ret

farewell ENDP
	

END main
