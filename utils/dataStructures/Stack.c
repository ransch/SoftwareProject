#include "Stack.h"
#include <assert.h>
#include "../MemAlloc.h"

void pushToStack(Stack *stack, void *data) {
	assert(stack != 0 && data != 0);
	appendElemToList(stack->list, createListElem(data));
}

void *stackTop(Stack *stack) {
	assert(stack != 0);
	return stack->list->last ? stack->list->last->data : 0;
}

void pop(Stack *stack) {
	assert(stack != 0 && stackTop(stack) != 0);
	removeLastElemFromList(stack->list);
}

Stack *createStack() {
	Stack *memAlloc(res, Stack);
	res->list = createList();
	return res;
}

void destroyStack(Stack *stack) {
	assert(stack != 0);
	destroyList(stack->list);
	memFree(stack);
}