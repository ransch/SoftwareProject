#ifndef __DATASTRUCTURES_ACTIVITY_H
#define __DATASTRUCTURES_ACTIVITY_H
/**
 * This module deals with board actions:
 * It defines the Move struct that represents a single move in the board,
 * and the Activity struct that keeps track of the actions in the board
 * (an action is a linked list that contains Move variables, non of them == 0).
 */

#include "../utils/dataStructures/DoublyLinkedList.h"
#include "../utils/Boolean.h"

/**
 * This struct defines a single move.
 */
typedef struct {

	/**
	 * The cell's row.
	 */
	unsigned int x;

	/**
	 * The cell's column.
	 */
	unsigned int y;

	/**
	 * The change in the cell's value.
	 */
	signed int delta;
} Move;

/**
 * This struct defines an activity in the sudoku board.
 */
typedef struct {

	/**
	 * A pointer to an element of [actions] that is the current action.
	 */
	LinkedListElem *current;

	/**
	 * A linked list that contains linked lists that contain Move variables.
	 * Every linked list is a single action.
	 */
	LinkedList *actions;
} Activity;

/**
 * This method creates a new empty activity.
 *
 * Returns:
 * A pointer to a dynamically allocated activity
 */
Activity *createActivity();

/**
 * This method destroys an activity.
 *
 * Preconditions:
 * activity != 0
 */
void destroyActivity(Activity *activity);

/**
 * This method creates a new move.
 *
 * Parameters:
 * unsigned int x - The cell's row
 * unsigned int y - The cell's column
 * signed int delta - The change in the cell's value
 *
 * Returns:
 * A pointer to a dynamically allocated move
 */
Move *createMove(unsigned int x, unsigned int y, signed int delta);

/**
 * This method destroys a move.
 *
 * Parameters:
 * Move *move
 *
 * Preconditions:
 * move != 0
 */
void destroyMove(Move *move);

/**
 * This method returns the current action.
 *
 * Parameters:
 * Activity *activity
 *
 * Preconditions:
 * activity != 0
 *
 * Postconditions:
 * [ret] == activity->current != 0 ? activity->current->data : 0
 */
LinkedList *getCurrentAction(Activity *activity);

/**
 * This method progresses the activity.
 *
 * Parameters:
 * Activity *activity
 *
 * Preconditions:
 * activity->current->next != 0
 *
 * Postconditions:
 * [activity after]->current == [activity before]->current == 0 ? [activity before]->actions->first : [activity before]->current->next
 */
void progressActivity(Activity *activity);

/**
 * This method regresses the activity.
 *
 * Parameters:
 * Activity *activity
 *
 * Preconditions:
 * activity->current != 0
 *
 * Postconditions:
 * [activity after]->current = [activity before]->current->prev
 */
void regressActivity(Activity *activity);

/**
 * This method adds a single move to the activity.
 *
 * Parameters:
 * Activity *activity
 * Move *move
 *
 * Preconditions:
 * activity != 0
 * move != 0
 */
void addMove(Activity *activity, Move *move);

/**
 * This method adds a single action to the activity.
 *
 * Parameters:
 * Activity *activity
 * LinkedList *action
 *
 * Preconditions:
 * activity != 0
 * action != 0
 * action is an activity, i.e. a list of Move variables
 */
void addAction(Activity *activity, LinkedList *action);

/**
 * This method returns true iff the activity may be progressed.
 *
 * Parameters:
 * Activity *activity
 *
 * Preconditions:
 * activity != 0
 *
 * Returns:
 * TRUE iff (activity->current == 0 && activity->actions->first != 0) || activity->current->next != 0
 */
Bool canProgressActivity(Activity *activity);

/**
 * This method returns true iff the activity may be regressed.
 *
 * Parameters:
 * Activity *activity
 *
 * Preconditions:
 * activity != 0
 *
 * Returns:
 * TRUE iff activity->current != 0
 */
Bool canRegressActivity(Activity *activity);

#endif