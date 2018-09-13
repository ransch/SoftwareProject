#ifndef __UTILS_DATASTRUCTURES_STACK_H
#define __UTILS_DATASTRUCTURES_STACK_H
/**
 * This module defines the stack data structure.
 */

#include "DoublyLinkedList.h"

/**
 * This struct defines a stack data structure.
 */
typedef struct {
	/**
	 * The implementation of the stack.
	 */
	LinkedList *list;
} Stack;

/**
 * This method pushes some new data into the stack.
 *
 * Parameters:
 * Stack *stack
 * void *data - The data that would be pushed into the stack
 *
 * Preconditions:
 * stack, data != 0
 *
 * Postconditions:
 * stackTop(stack) == data
 */
void pushToStack(Stack *stack, void *data);

/**
 * This method retrieves the top of the stack.
 *
 * Parameters:
 * Stack *stack
 *
 * Preconditions:
 * stack != 0
 *
 * Returns:
 * The top of the stack. If it is empty, 0 is returned.
 */
void *stackTop(Stack *stack);

/**
 * This method deletes the top of the stack.
 *
 * Parameters:
 * Stack *stack
 *
 * Preconditions:
 * stack, stackTop(stack) != 0
 *
 * Postconditions:
 * The top of the stack has been removed
 */
void pop(Stack *stack);

/**
 * This method creates a new empty stack.
 *
 * Returns:
 * A pointer to a new dynamically allocated empty stack.
 */
Stack *createStack();

/**
 * This method destroys a stack.
 *
 * Parameters:
 * Stack *stack
 *
 * Preconditions:
 * stack != 0
 *
 * Postconditions:
 * [stack] and all its element have been freed.
 */
void destroyStack(Stack *stack);

#endif