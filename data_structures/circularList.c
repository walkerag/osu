/* CS261- Assignment 3 Part 2*/
/* Name: Adam Walker
 * Date: July 21 2018
 * Solution description: Implement linked list deque and bag
 * */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "circularList.h"

// Double link
struct Link
{
	TYPE value;
	struct Link * next;
	struct Link * prev;
};

struct CircularList
{
	int size;
	struct Link* sentinel;
};

/**
 * Allocates the list's sentinel and sets the size to 0.
 * The sentinel's next and prev should point to the sentinel itself.
 */
static void init(struct CircularList* list)
{

	list->sentinel = (struct Link *) malloc(sizeof(struct Link));
	assert(list->sentinel != 0);
	list->sentinel->next = list->sentinel;
	list->sentinel->prev = list->sentinel;
	list->size = 0;

}

/**
 * Creates a link with the given value and NULL next and prev pointers.
 */
static struct Link* createLink(TYPE value)
{

	struct Link* newLink = (struct Link *) malloc(sizeof(struct Link));
	newLink->value = value;
	newLink->next = NULL;
	newLink->prev = NULL;
	return newLink;

}

/**
 * Adds a new link with the given value after the given link and
 * increments the list's size.
 */
static void addLinkAfter(struct CircularList* list, struct Link* link, TYPE value)
{

	// Create the new link
	struct Link* newLink = createLink(value);

	// Update the new link's next
	newLink->next=link->next;
	link->next->prev  = newLink;

	// Update the previous value
	link->next = newLink;
	newLink->prev = link;

	// Update size
	list->size++;

}

/**
 * Removes the given link from the list and
 * decrements the list's size.
 */
static void removeLink(struct CircularList* list, struct Link* link)
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
struct CircularList* circularListCreate()
{
	struct CircularList* list = malloc(sizeof(struct CircularList));
	init(list);
	return list;
}

/**
 * Deallocates every link in the list and frees the list pointer.
 */
void circularListDestroy(struct CircularList* list)
{

	while (!circularListIsEmpty(list))
	{
		circularListRemoveFront(list);
	}
	free(list->sentinel);
	free(list);

}

/**
 * Adds a new link with the given value to the front of the deque.
 */
void circularListAddFront(struct CircularList* list, TYPE value)
{

	// Adding after sentinel
	addLinkAfter( list, list->sentinel, value );

}

/**
 * Adds a new link with the given value to the back of the deque.
 */
void circularListAddBack(struct CircularList* list, TYPE value)
{

	// Adding after link before sentinel
	addLinkAfter( list, list->sentinel->prev, value );

}

/**
 * Returns the value of the link at the front of the deque.
 */
TYPE circularListFront(struct CircularList* list)
{

	assert( !circularListIsEmpty(list) );
	TYPE returnVal = list->sentinel->next->value;
	return returnVal;

}

/**
 * Returns the value of the link at the back of the deque.
 */
TYPE circularListBack(struct CircularList* list)
{

	assert( !circularListIsEmpty(list) );
	TYPE returnVal = list->sentinel->prev->value;
	return returnVal;

}

/**
 * Removes the link at the front of the deque.
 */
void circularListRemoveFront(struct CircularList* list)
{

	assert( !circularListIsEmpty(list) );
	removeLink( list, list->sentinel->next);

}

/**
 * Removes the link at the back of the deque.
 */
void circularListRemoveBack(struct CircularList* list)
{

	assert( !circularListIsEmpty(list) );
	removeLink( list, list->sentinel->prev);

}

/**
 * Returns 1 if the deque is empty and 0 otherwise.
 */
int circularListIsEmpty(struct CircularList* list)
{

	return list->size == 0;

}

/**
 * Prints the values of the links in the deque from front to back.
 */
void circularListPrint(struct CircularList* list)
{

	assert( !circularListIsEmpty(list) );
	struct Link* currentLink = list->sentinel->next;

	// Stop when sentinel is reached
	while( !EQ(currentLink, list->sentinel) ){

		printf("%g\n",currentLink->value);
		currentLink = currentLink->next;

	}

}

/**
 * Reverses the deque.
 */
void circularListReverse(struct CircularList* list)
{

	assert( !circularListIsEmpty(list) );

	// Stop when sentinel is reached
	struct Link* currentLink = list->sentinel->next;
	while( !EQ(currentLink, list->sentinel) ){

		// Reverse previous and next
		struct Link* temp = currentLink->prev;
		currentLink->prev = currentLink->next;
		currentLink->next = temp;

		// Now go to the (former) next link
		currentLink = currentLink->prev;

	}

	// Update sentinel
	struct Link* temp = list->sentinel->prev;
	list->sentinel->prev = list->sentinel->next;
	list->sentinel->next = temp;

}
