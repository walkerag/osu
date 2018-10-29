/*
 * This is the file in which you'll write the functions required to 
implement
 * a stack using two queues.  Make sure to add your name and 
@oregonstate.edu
 * email address below:
 *
 * Name: Adam Walker
 * Email: walkerad@oregonstate.edu
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


#ifndef TYPE
#define TYPE int
#endif

/**************************************************
 All of the initial Queue functions
 ***************************************************/
struct link {
	TYPE value;
	struct link * next;
};

struct listQueue {
	struct link *firstLink;
	struct link *lastLink;
	int size;
};

/*
* This function takes a queue and allocates the memory. It then
* creates a sentinel and assigns the first link and last link
* to the sentinel.
*/
void listQueueInit(struct listQueue *q) {
	
	   // Allocate memory for sentinel
 	   struct link *lnk = (struct link *) malloc(sizeof(struct link));
	   assert(lnk != 0);
	   lnk->next = 0;
	   lnk->value = 0;
	   q->firstLink = q->lastLink = lnk;
	   q->size = 0;

}

/*
 * This function creates a new queue. Parts of the create include 
allocating
 * the memory, initializing all of the values and returning a pointer to 
the newly
 * created queue.
 */
struct listQueue * listQueueCreate()
{

	// Allocate the memory
	struct listQueue *newQueue  = (struct listQueue *) malloc(sizeof(struct listQueue));
	// Initialize the queue
	listQueueInit(newQueue);
	// Return pointer
	return newQueue;
	
}


/*
* This function returns a 1 or 0 based on the statement looking at
* the first link. If the next value is null it is empty, and will return 1
*/
int listQueueIsEmpty(struct listQueue *q) {
	
	return q->size == 0;

}

/*
 * This function adds a new link and value to the back of the queue. It 
takes
 * a list and a type variable, allocates the memory and adjusts the proper 
links
 * to make the connection. No return value.
 */
void listQueueAddBack(struct listQueue *q, TYPE e) {
	
	assert( q != 0 );
    struct link *addBack = (struct link *) malloc(sizeof(struct link));

    addBack->value = e;
    addBack->next = 0;

    // Now the second to last element
    q->lastLink->next = addBack;

    // Update last link
    q->lastLink = addBack;

    // Update size
    q->size++;

}

/*
 * This function takes a list argument and removes the link at the front.
 */
void listQueueRemoveFront(struct listQueue *q) {

	assert( q != 0 );
	assert( !listQueueIsEmpty(q) );

	struct link* newFront = q->firstLink->next->next;

	// Set value to zero
	q->firstLink->next->value=0;

	// Remove the first link
	free( q->firstLink->next );

	// Update sentinel
	q->firstLink->next = newFront;

	// Update size
	q->size--;

}

/*
 * Function returns the value at the front of the list.
 */
TYPE listQueueFront(struct listQueue *q) {
	
	assert( q != 0 );
	assert( !listQueueIsEmpty(q) );

	TYPE returnVal = q->firstLink->next->value;
	return returnVal;

}


/*
* This function is a tester function that iterates through the list
* and prints out the values at each link.
*/
void printList(struct listQueue* l)
{
	assert(l != 0);
	
	struct link * printMe = l->firstLink->next;

	while (printMe!= NULL)
	{
		printf("Value: %d\n", printMe->value);
		printMe = printMe->next;
	}
}

/**************************************************
Stack Implementation
***************************************************/

struct linkedListStack {
	struct listQueue *Q1;
	struct listQueue *Q2;
	int structSize;
};

/*
 * This function initializes the values of the created Stack. Initializes 
both 
 * queues and the structSize.
 */
void linkedListStackInit(struct linkedListStack * s)
{

	// Create 2 queues and set size to zero
	s->Q1 = listQueueCreate();
	s->Q2 = listQueueCreate();
	s->structSize = 0;

 }

/*
* This function creates the linked list stack. It allocates the memory and 
calls the
* initialization function to initialize all of the values. It then returns 
the stack.
*/
struct linkedListStack * linkedListStackCreate(){

	// Create and initialize
    struct linkedListStack* newStack = (struct linkedListStack *) malloc(sizeof(struct linkedListStack));
    linkedListStackInit( newStack );

	return newStack;

}

/*
* This function returns 1 if the linked list stack is empty and otherwise 
returns a 0.
*/
int linkedListStackIsEmpty(struct linkedListStack *s) {

    assert(s != 0);
	return s->structSize == 0;

}

/*
 * This is the linked list acting as a stack push function. It takes 
 * a linked list stack argument and a value and pushes it onto the stack. 
The
 * function then also increases the size of the stack by 1.
 */
void linkedListStackPush(struct linkedListStack *s, TYPE d) {
	
    assert(s != 0);

    // Add value to whichever queue is functioning as the stack
    if( listQueueIsEmpty(s->Q2) ){

		listQueueAddBack(s->Q1,d);

	} else {

		listQueueAddBack(s->Q2,d);

	}

	s->structSize++;

}

/*
 * This function pops a value off of the stack. It does this by moving all
values
 * that are currently on the stack to the other queue. The stack top is 
maintained
 * at the back of the queue list. 
 */
void linkedListStackPop(struct linkedListStack *s) {
	
    assert( s != 0 );
	if( linkedListStackIsEmpty(s) ){

		printf("Stack is empty\n");
		return;

	}

	struct listQueue *stackQueue;
	struct listQueue *emptyQueue;
	struct link * qLink;

	// Find which queue is acting as stack, assign correct names
	if( listQueueIsEmpty(s->Q1) ){

		// Q2 holds the values currently
		qLink = s->Q2->firstLink->next;
		stackQueue = s->Q2;
		emptyQueue = s->Q1;

	} else {

		// Q1 holds the values currently
		qLink = s->Q1->firstLink->next;
		stackQueue = s->Q1;
		emptyQueue = s->Q2;

	}


	// Transfer all but last of values over to the empty queue
	while ( qLink->next != 0 )
	{

		// Add value to back of empty queue
		listQueueAddBack(emptyQueue, qLink->value);

		// Get next link
		qLink = qLink->next;

		// Remove from stack queue
		listQueueRemoveFront(stackQueue);

	}

	// Remove the front value of stack queue
	listQueueRemoveFront(stackQueue);

	stackQueue->lastLink = stackQueue->firstLink;

	// Reduce size by 1;
	s->structSize--;

}

/*
 * This function returns the value that is at the back of the queue that 
is 
 * maintaining the values of the stack.
 */
TYPE linkedListStackTop(struct linkedListStack *s) {

    assert( s != 0 );
	assert( !linkedListStackIsEmpty(s) );

	// Find starting queue
	struct link * qLink;
	if( listQueueIsEmpty(s->Q1) ){

		// Q2 holds the values currently
		qLink = s->Q2->firstLink->next;

	} else {

		// Q1 holds the values currently
		qLink = s->Q1->firstLink->next;

	}

	// Run until last link reached
	TYPE returnVal;
	while ( qLink != 0 )
	{
		// Value of link
		returnVal = qLink->value;

		// Get next link
		qLink = qLink->next;
	}

	return returnVal;

}

/*
 * This function goes through the stack and removes each link in the
queue.
 * It then frees the struct itself. 
 */
void linkedListStackFree(struct linkedListStack *s){
	
      assert(s != 0);
	
	while (s->structSize != 0) 
	{
		linkedListStackPop(s);
	}

	free(s->Q1->firstLink);
	free(s->Q2->firstLink);
	free(s->Q1);
	free(s->Q2);

	free(s);

}

/*
 * Main is used to test the stack ADT.
 */
int main(int argc, char* argv[])
{

	struct linkedListStack *stack = linkedListStackCreate();

	//Test Stack
	//Push 4 values onto the stack
	printf("Pushing the value: 1\n");
	linkedListStackPush(stack, 1);
	printf("Pushed.\n\n");

	printf("Pushing the value: 2\n");
	linkedListStackPush(stack, 2);
	printf("Pushed.\n\n");

	printf("Pushing the value: 3\n");
	linkedListStackPush(stack, 3);
	printf("Pushed.\n\n");

	printf("Pushing the value: 4\n");
	linkedListStackPush(stack, 4);
	printf("Pushed.\n\n");

	//Print value at the top and then remove it
	printf("Value at the top of stack %d now being popped. \n",linkedListStackTop(stack));
	linkedListStackPop(stack);
	printf("Value popped.\n\n");

	printf("Value at the top of stack: %d now being popped. \n", linkedListStackTop(stack));
	linkedListStackPop(stack);
	printf("Value popped.\n\n");

	printf("Value at the top of stack: %d now being popped. \n", linkedListStackTop(stack));
	linkedListStackPop(stack);
	printf("Value popped.\n\n");

	printf("Value at the top of stack: %d now being popped. \n", linkedListStackTop(stack));  
	linkedListStackPop(stack);
	printf("Value popped.\n\n");

	//Try to pop when the stack is empty prints error:
	printf("Trying to pop empty stack:\n");

	linkedListStackPop(stack);

	
	//Push and Pop alternating
	printf("Pushing the value: 10\n");
	linkedListStackPush(stack, 10);
	printf("Pushed.\n\n");

	printf("Pushing the value: 11\n");
	linkedListStackPush(stack, 11);
	printf("Pushed.\n\n");

	printf("One more pop:\n");
	linkedListStackPop(stack);
	printf("Value at the top of stack: %d\n", linkedListStackTop(stack));

	linkedListStackFree(stack);

	return 0;

}


