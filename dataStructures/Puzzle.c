#include "Puzzle.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../utils/MemAlloc.h"
#include "../MainAux.h"
#include "../algs/SudokuAlgs.h"

/**
 * This method dynamically allocates a n*n*2 zeros tensor.
 */
static unsigned int ***allocateTensor(unsigned int n) {
	unsigned int i, j;
	unsigned int ***memAllocN(tensor, unsigned int**, n);

	for (i = 0; i < n; i++) {
		memAllocN(tensor[i], unsigned int*, n);

		for (j = 0; j < n; j++) {
			memAllocN(tensor[i][j], unsigned int, 2);
		}
	}

	return tensor;
}

/**
 * This method clones a n*n*2 tensor.
 */
static unsigned int ***cloneTensor(unsigned int ***tensor, unsigned int n) {
	unsigned int*** clone = allocateTensor(n);
	unsigned int i, j;

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			clone[i][j][0] = tensor[i][j][0];
			clone[i][j][1] = tensor[i][j][1];
		}
	}

	return clone;
}

/**
 * This method frees a n*n*2 tensor.
 */
static void freeTensor(unsigned int ***tensor, unsigned int n) {
	unsigned int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			memFree(tensor[i][j]);
		}
		memFree(tensor[i]);
	}
	memFree(tensor);
}

Puzzle *createPuzzle(unsigned int n, unsigned int m) {
	unsigned int*** board = allocateTensor(n * m);
	Puzzle *memAlloc(res, Puzzle);
	res->n = n;
	res->m = m;
	res->board = board;
	res->zeroCnt = n * m*n*m;
	return res;
}

void destroyPuzzle(Puzzle *puzzle) {
	freeTensor(puzzle->board, puzzle->n * puzzle->m);
	memFree(puzzle);
}

Puzzle *clonePuzzle(Puzzle *puzzle) {
	Puzzle *memAlloc(res, Puzzle);
	res->n = puzzle->n;
	res->m = puzzle->m;
	res->board = cloneTensor(puzzle->board, puzzle->n * puzzle->m);
	res->zeroCnt = puzzle->zeroCnt;
	return res;
}

Bool isCellFixed(Puzzle *puzzle, unsigned int x, unsigned int y) {
	assert(x < puzzle->n*puzzle->m && y < puzzle->n*puzzle->m);
	return ((puzzle->board)[x][y][1]);
}

void fixCell(Puzzle *puzzle, unsigned int x, unsigned int y) {
	assert(x < puzzle->n*puzzle->m && y < puzzle->n*puzzle->m);
	(puzzle->board)[x][y][1] = 1;
}

void unFixCell(Puzzle *puzzle, unsigned int x, unsigned int y) {
	assert(x < puzzle->n*puzzle->m && y < puzzle->n*puzzle->m);
	(puzzle->board)[x][y][1] = 0;
}

unsigned int getBoardValue(Puzzle *puzzle, unsigned int x, unsigned int y) {
	assert(x < puzzle->n*puzzle->m && y < puzzle->n*puzzle->m);
	return (puzzle->board)[x][y][0];
}

void setBoardValue(Puzzle *puzzle, unsigned int x, unsigned int y, unsigned int v) {
	assert(x < puzzle->n*puzzle->m && y < puzzle->n*puzzle->m && v <= puzzle->n*puzzle->m);
	if (getBoardValue(puzzle, x, y) && !v) {
		puzzle->zeroCnt++;
	} else if (!getBoardValue(puzzle, x, y) && v) {
		puzzle->zeroCnt--;
	}
	(puzzle->board)[x][y][0] = v;
}

void applyActivitySingle(Puzzle *puzzle, Move *move) {
	LinkedList *action = createList();
	appendElemToList(action, createListElem(move));
	applyActivity(puzzle, action);
	destroyList(action);
}

void revertActivitySingle(Puzzle *puzzle, Move *move) {
	LinkedList *action = createList();
	appendElemToList(action, createListElem(move));
	revertActivity(puzzle, action);
	destroyList(action);
}

static void applyActivityElem(Puzzle* puzzle, LinkedListElem *elem) {
	Move *move;
	assert(elem != 0);
	move = ((Move*) elem->data);
	setBoardValue(puzzle, move->x, move->y, getBoardValue(puzzle, move->x, move->y) + move->delta);
	if (elem->next != 0) {
		applyActivityElem(puzzle, elem->next);
	}
}

static void revertActivityElem(Puzzle* puzzle, LinkedListElem *elem) {
	Move *move;
	assert(elem != 0);
	move = ((Move*) elem->data);
	setBoardValue(puzzle, move->x, move->y, getBoardValue(puzzle, move->x, move->y) - move->delta);
	if (elem->next != 0) {
		revertActivityElem(puzzle, elem->next);
	}
}

void applyActivity(Puzzle *puzzle, LinkedList *action) {
	assert(action != 0 && action->first != 0);
	applyActivityElem(puzzle, action->first);
}

void revertActivity(Puzzle *puzzle, LinkedList *action) {
	assert(action != 0 && action->first != 0);
	revertActivityElem(puzzle, action->first);
}

static char *seperatorStr(unsigned int n, unsigned int m) {
	unsigned int i;
	unsigned int len = 4 * n * m + n + 1;
	char *memAllocN(res, char, len + 1);
	for (i = 0; i < len; i++) {
		res[i] = '-';
	}
	return res;
}

void printBoard(Puzzle *puzzle, Bool markErrorsFlag) {
	char *sep = seperatorStr(puzzle->n, puzzle->m);
	unsigned int i, j, k, l, v, x, y;

	printf("%s\n", sep);
	for (i = 0; i < puzzle->m; i++) {
		for (j = 0; j < puzzle->n; j++) {
			printf("|");
			for (l = 0; l < puzzle->n; l++) {
				for (k = 0; k < puzzle->m; k++) {
					x = i * puzzle->n + j;
					y = l * puzzle->m + k;
					v = getBoardValue(puzzle, x, y);

					if (v == 0) {
						printf("   ");
					} else {
						printf(" %2d", v);
					}
					if (isCellFixed(puzzle, x, y)) {
						printf(".");
					} else {
						if (markErrorsFlag && !isCellLegal(puzzle, x, y)) {
							printf("*");
						} else {
							printf(" ");
						}
					}
				}
				printf("|");
			}
			printf("\n");
		}
		printf("%s\n", sep);
	}

	memFree(sep);
}