/* CS261- Assignment 3 Part 1*/
/* Name: Adam Walker
 * Date: July 21 2018
 * Solution description: Implement linked list deque and bag
 * */

#include "linkedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// Double link
struct Link
{
	TYPE value;
	struct Link* next;
	struct Link* prev;
};

// Double linked list with front and back sentinels
struct LinkedList
{
	int size;
	struct Link* frontSentinel;
	struct Link* backSentinel;
};

/**
 * Allocates the list's sentinel and sets the size to 0.
 * The sentinels' next and prev should point to eachother or NULL
 * as appropriate.
 */
static void init(struct LinkedList* list) {

	   list->frontSentinel = (struct Link *) malloc(sizeof(struct Link));
	   assert(list->frontSentinel != 0);
	   list->backSentinel = (struct Link *) malloc(sizeof(struct Link));
	   assert(list->backSentinel != 0);
	   list->frontSentinel->next = list->backSentinel;
	   list->backSentinel->prev = list->frontSentinel;
	   list->size = 0;

}

/**
 * Adds a new link with the given value before the given link and
 * increments the list's size.
 */
static void addLinkBefore(struct LinkedList* list, struct Link* link, TYPE value)
{

	// New link
	struct Link *newLink = (struct Link *) malloc(sizeof(struct Link));

	newLink->value = value;
	newLink->next = link;
	newLink->prev = link->prev;

	// Updates next of previous link
	link->prev->next = newLink;

	// Updates previous of next link
	link->prev = newLink;

	// Update field size
	list->size++;

}

/**
 * Removes the given link from the list and
 * decrements the list's size.
 */
static void removeLink(struct LinkedList* list, struct Link* link)
{

	// Update previous link's next
	link->prev->next = link->next;

	// Update next link's previous
	link->next->prev = link->prev;

	free( link );

	// Update field size
	list->size--;

}

/**
 * Allocates and initializes a list.
 */
struct LinkedList* linkedListCreate()
{

	struct LinkedList* newDeque = malloc(sizeof(struct LinkedList));
	init(newDeque);
	return newDeque;

}

/**
 * Deallocates every link in the list including the sentinels,
 * and frees the list itself.
 */
void linkedListDestroy(struct LinkedList* list)
{

	while (!linkedListIsEmpty(list))
	{
		linkedListRemoveFront(list);
	}
	free(list->frontSentinel);
	free(list->backSentinel);
	free(list);

}

/**
 * Adds a new link with the given value to the front of the deque.
 */
void linkedListAddFront(struct LinkedList* list, TYPE value)
{

	addLinkBefore( list, list->frontSentinel->next, value );

}

/**
 * Adds a new link with the given value to the back of the deque.
 */
void linkedListAddBack(struct LinkedList* list, TYPE value)
{

	addLinkBefore( list, list->backSentinel, value );

}

/**
 * Returns the value of the link at the front of the deque.
 */
TYPE linkedListFront(struct LinkedList* list)
{

	assert( !linkedListIsEmpty(list) );
	TYPE returnVal = list->frontSentinel->next->value;
	return returnVal;

}

/**
 * Returns the value of the link at the back of the deque.
 */
TYPE linkedListBack(struct LinkedList* list)
{

	assert( !linkedListIsEmpty(list) );
	TYPE returnVal = list->backSentinel->prev->value;
	return returnVal;

}

/**
 * Removes the link at the front of the deque.
 */
void linkedListRemoveFront(struct LinkedList* list)
{

	assert( !linkedListIsEmpty(list) );
	removeLink( list, list->frontSentinel->next );

}

/**
 * Removes the link at the back of the deque.
 */
void linkedListRemoveBack(struct LinkedList* list)
{

	assert( !linkedListIsEmpty(list) );
	removeLink( list, list->backSentinel->prev );

}

/**
 * Returns 1 if the deque is empty and 0 otherwise.
 */
int linkedListIsEmpty(struct LinkedList* list)
{

	return list->size == 0;

}

/**
 * Prints the values of the links in the deque from front to back.
 */
void linkedListPrint(struct LinkedList* list)
{

	assert( !linkedListIsEmpty(list) );
	struct Link* currentLink = list->frontSentinel->next;

	// Stop when back sentinel is reached
	while( !EQ(currentLink, list->backSentinel) ){

		printf("%d\n",currentLink->value);
		currentLink = currentLink->next;

	}

}

/**
 * Adds a link with the given value to the bag.
 */
void linkedListAdd(struct LinkedList* list, TYPE value)
{

	addLinkBefore( list, list->frontSentinel->next, value );

}

/**
 * Returns 1 if a link with the value is in the bag and 0 otherwise.
 */
int linkedListContains(struct LinkedList* list, TYPE value)
{

	assert( !linkedListIsEmpty(list) );

	struct Link* checkLink = list->frontSentinel->next;
	for(int i=0; i<list->size; i++){

		if( EQ( checkLink->value, value ) ) return 1;

		checkLink = checkLink->next;

	}

	return 0;

}

/**
 * Removes the first occurrence of a link with the given value.
 */
void linkedListRemove(struct LinkedList* list, TYPE value)
{

	assert( !linkedListIsEmpty(list) );

	struct Link* checkLink = list->frontSentinel->next;
	int foundValue = 0;
	int endList = 0;
	while( !foundValue && !endList ){

		// If the link has the value, remove it
		if( EQ( checkLink->value, value ) ) {

			removeLink( list, checkLink );
			foundValue=1;

		} else{

			checkLink = checkLink->next;
			if( EQ(checkLink, list->backSentinel) ) endList=1;

		}

	}

}
