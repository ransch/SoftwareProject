#include "DoublyLinkedList.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../MemAlloc.h"

static void destroyListElem(LinkedListElem *elem) {
	memFree(elem);
}

void destroyList(LinkedList *list) {
	assert(list != 0);
	if (list->first != 0) {
		assert(list->last != 0);
		forEachListElemRev(list, destroyListElem);
	}
	memFree(list);
}

void forEachListElemElems(LinkedListElem *elem, void(*operation)(LinkedListElem*)) {
	assert(elem != 0);
	operation(elem);
	if (elem->next != 0) {
		forEachListElemElems(elem->next, operation);
	}
}

void forEachListElem(LinkedList *list, void(*operation)(LinkedListElem*)) {
	assert(list != 0 && list->first != 0 && list->last != 0);
	forEachListElemElems(list->first, operation);
}

/**
 * This method applies [operation] to every element in the sublist of [elem]'s list that begins with [elem],
 * beginning with ([elem]'s list -> last) and progressing in descending order.
 *
 * Parameters:
 * LinkedListElem *elem
 * void(*operation)(LinkedListElem*)
 */
static void forEachListElemRevElems(LinkedListElem *elem, void(*operation)(LinkedListElem*)) {
	assert(elem != 0);
	if (elem->next != 0) {
		forEachListElemRevElems(elem->next, operation);
	}
	operation(elem);
}

void forEachListElemRev(LinkedList *list, void(*operation)(LinkedListElem*)) {
	assert(list != 0 && list->first != 0 && list->last != 0);
	forEachListElemRevElems(list->first, operation);
}

LinkedListElem *createListElem(void *data) {
	LinkedListElem *memAlloc(res, LinkedListElem);
	res->prev = 0;
	res->next = 0;
	res->data = data;
	return res;
}

LinkedList *createList() {
	LinkedList *memAlloc(res, LinkedList);
	res->first = 0;
	res->last = 0;
	return res;
}

void appendElemToList(LinkedList *list, LinkedListElem *elem) {
	assert(list != 0 && elem != 0 && elem->prev == 0 && elem->next == 0);
	if (list->first == 0) {
		assert(list->last == 0);
		list->first = elem;
		list->last = elem;
		return;
	}

	assert(list->last != 0 && list->last->next == 0);
	list->last->next = elem;
	elem->prev = list->last;
	list->last = elem;
}

void removeLastElemFromList(LinkedList *list) {
	assert(list != 0 && list->first != 0 && list->last != 0);

	if (list->last->prev == 0) {
		list->first = 0;
		destroyListElem(list->last);
		list->last = 0;
	} else {
		list->last = list->last->prev;
		destroyListElem(list->last->next);
		list->last->next = 0;
	}
}