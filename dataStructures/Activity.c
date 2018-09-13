#include "Activity.h"
#include <assert.h>
#include "../utils/MemAlloc.h"

Activity *createActivity() {
	Activity *memAlloc(res, Activity);
	res->actions = createList();
	res->current = 0;
	return res;
}

void destroyMove(Move *move) {
	memFree(move);
}

static void destroyElemData2(LinkedListElem *elem) {
	assert((Move*) elem->data != 0);
	destroyMove((Move*) elem->data);
}

/**
 * This method destroys an action.
 *
 * Parameters:
 * LinkedList *action
 *
 * Preconditions:
 * action != 0
 * action is a list of Move variables, non of them == 0
 */
static void destroyAction(LinkedList *action) {
	assert(action != 0);

	forEachListElem(action, destroyElemData2);
	destroyList(action);
}

static void destroyElemData1(LinkedListElem *elem) {
	assert((LinkedList*) elem->data != 0);
	destroyAction((LinkedList*) elem->data);
}

void destroyActivity(Activity *activity) {
	assert(activity != 0);
	if (activity->actions->first) {
		forEachListElem(activity->actions, destroyElemData1);
	}
	destroyList(activity->actions);
	memFree(activity);
}

Move *createMove(unsigned int x, unsigned int y, int delta) {
	Move *memAlloc(res, Move);
	res->x = x;
	res->y = y;
	res->delta = delta;
	return res;
}

LinkedList *getCurrentAction(Activity *activity) {
	assert(activity != 0);
	if (activity->current != 0) {
		return (LinkedList*) activity->current->data;
	}
	return 0;
}

void progressActivity(Activity *activity) {
	assert((activity->current == 0 && activity->actions->first != 0) || activity->current->next != 0);
	activity->current = activity->current == 0 ? activity->actions->first : activity->current->next;
}

void regressActivity(Activity *activity) {
	assert(activity->current != 0);
	activity->current = activity->current->prev;
}

Bool canProgressActivity(Activity *activity) {
	if (activity->current == 0) {
		if (activity->actions->first != 0) {
			return TRUE;
		}
	} else if (activity->current->next != 0) {
		return TRUE;
	}
	return FALSE;
}

Bool canRegressActivity(Activity *activity) {
	if (activity->current != 0) {
		return TRUE;
	}
	return FALSE;
}

void addMove(Activity *activity, Move *move) {
	LinkedList *action = createList();
	appendElemToList(action, createListElem(move));
	addAction(activity, action);
}

void addAction(Activity *activity, LinkedList *action) {
	while (activity->actions->last != activity->current) { /*Delete all the actions beyond activity->current*/
		destroyElemData1(activity->actions->last);
		removeLastElemFromList(activity->actions);
	}

	appendElemToList(activity->actions, createListElem(action));
	activity->current = activity->actions->last;
}