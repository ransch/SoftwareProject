#include "exhBacktr.h"
#include <assert.h>
#include "SudokuAlgs.h"
#include "../utils/dataStructures/Stack.h"
#include "../utils/MemAlloc.h"

typedef struct {

	/**
	  *The row we are in
	  */
	unsigned int i;

	/**
	 *The column we are in
	 */
	unsigned int j;

	/**
	 *The puzzle right now
	 */
	Puzzle *puzzle;
} PuzzleState;

/**
  *This method creates a new puzzle state.
  *
  *Returns:
  *A pointer to a new dynamically allocated puzzle state.
  */
static PuzzleState *createPuzzleState(unsigned int i, unsigned int j, Puzzle *puzzle) {
	PuzzleState *memAlloc(ps, PuzzleState);
	ps->i = i;
	ps->j = j;
	ps->puzzle = puzzle;
	return ps;
}

/**
  *This method destroys a puzzle state.
  *
  *Parameters:
  *PuzzleState *ps
  *
  *Preconditions:
  *ps != 0
 */
static void destroyPuzzleState(PuzzleState *ps) {
	assert(ps != 0);
	memFree(ps);
}

/**
  *This method pushes the value of i, j and the address of puzzle to the stack s
  *
  *Parameters:
  *Stack *s
  *unsigned int i - The row
  *unsigned int j - The column
  *Puzzle *puzzle
  *
  *Preconditions:
  *puzzle != 0, s != 0
  *i,j < n*m
  */
static void pushPuzzleToStack(Stack *s, unsigned int i, unsigned int j, Puzzle *puzzle) {
	PuzzleState *ps;
	Puzzle *localP;

	localP = copyPuzzle(puzzle);
	ps = createPuzzleState(i, j, localP);
	pushToStack(s, (void*) ps);
}

/**
  *This method pops the value of i, j and the address of puzzle from the stack s into the pointers i, j and puzzle
  *
  *Parameters:
  *Stack *s
  *unsigned int *i - The row
  *unsigned int *j - The column
  *Puzzle *puzzle
  *
  *Preconditions:
  *s != 0
  *i,j < n*m
  */
static void popPuzzleFromStack(Stack *s, unsigned int *i, unsigned int *j, Puzzle *puzzle) {
	unsigned int a, b, c, size;
	PuzzleState *ps = stackTop(s);

	pop(s);
	size = ps->puzzle->m*ps->puzzle->n;
	*i = ps->i;
	*j = ps->j;
	puzzle->m = ps->puzzle->m;
	puzzle->n = ps->puzzle->n;
	puzzle->zeroCnt = ps->puzzle->zeroCnt;
	for (a = 0; a < size; a++)
		for (b = 0; b < size; b++)
			for (c = 0; c <= 1; c++)
				puzzle->board[a][b][c] = ps->puzzle->board[a][b][c];
	destroyPuzzle(ps->puzzle);
	destroyPuzzleState(ps);
}

int exhaustiveBacktracking(Puzzle *localP, unsigned int i, unsigned int j) {
	unsigned int size = localP->m*localP->n, value, solutionCount = 0;
	Stack *s = createStack();
	unsigned int lastI = size - 1, lastJ = size - 1;
	if (isCellFixed(localP, lastI, lastJ) == TRUE)
		precede(localP, &lastI, &lastJ, size);

	pushPuzzleToStack(s, i, j, localP);

	while (stackTop(s) != 0) {
		popPuzzleFromStack(s, &i, &j, localP);
		if (i == lastI && j == lastJ) {
			for (value = 1; value <= size; value++) {
				if (isCellValueLegal(localP, i, j, value)) {
					solutionCount++;
				}
			}
		}

		else {
			for (value = 1; value <= size; value++) {
				setBoardValue(localP, i, j, 0);
				if (isCellValueLegal(localP, i, j, value)) {
					setBoardValue(localP, i, j, value);
					proceed(localP, &i, &j, size);
					pushPuzzleToStack(s, i, j, localP);
					setBoardValue(localP, i, j, 0);
					precede(localP, &i, &j, size);
				}
			}
		}

		setBoardValue(localP, i, j, 0);
	}
	destroyStack(s);
	return solutionCount;
}