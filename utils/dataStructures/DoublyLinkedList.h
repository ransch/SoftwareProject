#ifndef __UTILS_DATASTRUCTURES_DOUBLYLINKEDLIST_H
#define __UTILS_DATASTRUCTURES_DOUBLYLINKEDLIST_H
/**
 * This module defines a doubly linked list.
 */

/**
 * This struct defines an element of a list.
 *
 * Invariants:
 * A LinkedListElem is always a member of at most one list.
 */
typedef struct linkedlistelem_struct {

	/**
	 * A pointer to the element's data.
	 */
	void *data;

	/**
	 * A pointer to the previous element in the list.
	 */
	struct linkedlistelem_struct *prev;

	/**
	 * A pointer to the next element in the list.
	 */
	struct linkedlistelem_struct *next;
} LinkedListElem;

/**
 * This struct defines a linked-list.
 */
typedef struct {

	/**
	 * A pointer to the first element of the list.
	 */
	LinkedListElem *first;

	/**
	 * A pointer to the last element of the list.
	 */
	LinkedListElem *last;
} LinkedList;

/**
 * This method creates a new list element that wraps the given data.
 *
 * Returns:
 * A pointer to a new dynamically allocated list element.
 *
 * Postconsitions:
 * res->prev = 0
 * res->next = 0
 * res->data = data
 */
LinkedListElem *createListElem(void *data);

/**
 * This method creates a new empty list.
 *
 * Returns:
 * A pointer to a new dynamically allocated empty list.
 *
 * Postconsitions:
 * ret->first == ret->last == 0
 */
LinkedList *createList();

/**
 * This method gets a list and an element and appends this element to the list.
 *
 * Parameters:
 * LinkedList *list
 * LinkedListElem *elem
 *
 * Preconditions:
 * list != 0
 * elem != 0
 * elem->prev == 0
 * elem->next == 0

 * Postconditions:
 * The last element in [list] is [elem]
 */
void appendElemToList(LinkedList *list, LinkedListElem *elem);

/**
 * This method removes the last element of [list].
 *
 * Parameters:
 * LinkedList *list
 *
 * Postconditions:
 * list != 0
 * list->last != 0
 *
 * Postconditions:
 * [before list->last]->prev == 0
 * [after list] does not contain [before list->last]
 * The removed element is freed from the heap
 */
void removeLastElemFromList(LinkedList *list);

/**
 * This method destroys a list.
 *
 * Parameters:
 * LinkedList *list
 *
 * Preconditions:
 * list != 0
 *
 * Postconditions:
 * [list] and all its element have been freed.
 */
void destroyList(LinkedList *list);

/**
 * This method applies [operation] to every element of [list], beginning with the list->first
 * and progressing in ascending order.
 *
 * Parameters:
 * LinkedList *list
 * void(*operation)(LinkedListElem*)
 */
void forEachListElem(LinkedList *list, void(*operation)(LinkedListElem*));

/**
 * This method applies [operation] to every element of [list], beginning with the list->last
 * and progressing in descending order.
 *
 * Parameters:
 * LinkedList *list
 * void(*operation)(LinkedListElem*)
 */
void forEachListElemRev(LinkedList *list, void(*operation)(LinkedListElem*));

/**
 * This method applies [operation] to every element of [elem]'s list, beginning with the [elem]
 * and progressing in ascending order.
 *
 * Parameters:
 * LinkedListElem *elem
 * void(*operation)(LinkedListElem*)
 */
void forEachListElemElems(LinkedListElem *elem, void(*operation)(LinkedListElem*));

#endif
