/*	stack.c: Stack application. */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dynArray.h"


/* ***************************************************************
Using stack to check for unbalanced parentheses.
***************************************************************** */

/* Returns the next character of the string, once reaches end return '0' (zero)
	param: 	s pointer to a string 	
	pre: s is not null		
*/
char nextChar(char* s)
{
	static int i = -1;	
	char c;
	++i;	
	c = *(s+i);			
	if ( c == '\0' )
		return '\0';	
	else 
		return c;
}

/* Checks whether the (), {}, and [] are balanced or not
	param: 	s pointer to a string 	
	pre: s is not null	
	post:	
*/
int isBalanced(char* s)
{

	assert( s != 0 );

	// Create a stack
	DynArr *dyn;
	dyn = newDynArr(2);

	char nextCharFound = nextChar(s);
	while( nextCharFound!='\0') {

		// If open parentheses, add to stack
		if( (nextCharFound==40) | (nextCharFound==91) | (nextCharFound==123) ) {

			pushDynArr(dyn, nextCharFound);

		// Close parentheses
		// Check if corresponding open parentheses is at top of stack
		} else if( (nextCharFound==41) | (nextCharFound==93) | (nextCharFound==125) ) {

			// If empty, no front parentheses, therefore unbalanced
			if( isEmptyDynArr(dyn) ) { deleteDynArr(dyn); return 0; }

			// Check correct front parentheses is at top
			TYPE topChar = topDynArr(dyn);
			if( nextCharFound==41 && topChar!=40 ) { deleteDynArr(dyn); return 0; }
			if( nextCharFound==93 && topChar!=91 ) { deleteDynArr(dyn); return 0; }
			if( nextCharFound==125 && topChar!=123 ) { deleteDynArr(dyn); return 0; }

			// Remove front parentheses
			popDynArr(dyn);

		}

		// Get next character
		nextCharFound = nextChar(s);

	}

	// Return true if stack is empty
	int returnVal = isEmptyDynArr(dyn);
	deleteDynArr(dyn);
	return returnVal;
}

int main(int argc, char* argv[]){
	
	char* s=argv[1];	
	int res;
	
	printf("Assignment 2\n");

	res = isBalanced(s);

	if (res)
		printf("The string %s is balanced\n",s);
	else 
		printf("The string %s is not balanced\n",s);
	
	return 0;	
}

