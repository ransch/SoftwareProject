#ifndef __DATASTRUCTURES_PUZZLE_H
#define __DATASTRUCTURES_PUZZLE_H
/**
 * This module defines the Puzzle struct which represents a Sudoku board.
 */

#include "../utils/Boolean.h"
#include "Activity.h"

/**
 * This struct represents a Sudoku board.
 */
typedef struct {

	/**
	 * Number of empty cells in the board
	 */
	unsigned int zeroCnt;

	/**
	 * The number of rows in each block.
	 */
	unsigned int n;

	/**
	 * The number of columns in each block.
	 */
	unsigned int m;

	/**
	 * This is a (n*m) * (n*m) * 2 tensor.
	 * each 2 cells represent a cell in the board - The first
	 * cell is the value, the second may be 0 or 1, and is 1 iff
	 * the cell is fixed.
	 */
	unsigned int ***board;
} Puzzle;

/**
 * This method creates a new (zeros) puzzle.
 *
 * Parameters:
 * unsigned int n - The number of rows in each block
 * unsigned int m - The number of columns in each block
 *
 * Preconditions:
 * n,m ≥ 1
 *
 * Returns:
 * A pointer to a new dynamically allocated puzzle
 *
 * Postconditions:
 * ∀i∀j, 0 ≤ i,j < n*m → getBoardValue([ret], i, j) == 0 ∧ isCellFixed([ret], i, j)) == FALSE
 */
Puzzle *createPuzzle(unsigned int n, unsigned int m);

/**
 * This method destroys a puzzle.
 *
 * Preconditions:
 * puzzle != 0
 */
void destroyPuzzle(Puzzle *puzzle);

/**
 * This method clones a puzzle.
 */
Puzzle *clonePuzzle(Puzzle *puzzle);

/**
 * This method returns true iff the cell (x,y) is fixed.
 *
 * Parameters:
 * Puzzle *puzzle
 * unsigned int x - The row
 * unsigned int y - The column
 *
 * Preconditions:
 * puzzle != 0
 * 0 ≤ x,y < n*m
 */
Bool isCellFixed(Puzzle *puzzle, unsigned int x, unsigned int y);

/**
 * This method fixes the cell (x,y).
 *
 * Parameters:
 * Puzzle *puzzle
 * unsigned int x - The row
 * unsigned int y - The column
 *
 * Preconditions:
 * puzzle != 0
 * 0 ≤ x,y < n*m
 *
 * Postconditions:
 * isCellFixed(puzzle, x, y)
 */
void fixCell(Puzzle *puzzle, unsigned int x, unsigned int y);

/**
 * This method unfixes the cell (x,y).
 *
 * Parameters:
 * Puzzle *puzzle
 * unsigned int x - The row
 * unsigned int y - The column
 *
 * Preconditions:
 * puzzle != 0
 * 0 ≤ x,y < n*m
 *
 * Postconditions:
 * !isCellFixed(puzzle, x, y)
 */
void unFixCell(Puzzle *puzzle, unsigned int x, unsigned int y);

/**
 * This method returns the value in the (x,y) cell of the board.
 *
 * Parameters:
 * Puzzle *puzzle
 * unsigned int x - The row
 * unsigned int y - The column
 *
 * Preconditions:
 * puzzle != 0
 * 0 ≤ x,y < n*m
 *
 * Returns:
 * The value in the (x,y) cell in the board
 */
unsigned int getBoardValue(Puzzle *puzzle, unsigned int x, unsigned int y);

/**
 * This method sets the value in the (x,y) cell of the board to [v].
 * [v] == 0 means an erasing the cell's value.
 *
 * Parameters:
 * Puzzle *puzzle
 * unsigned int x - The row
 * unsigned int y - The column
 * unsigned int v - The value
 *
 * Preconditions:
 * puzzle != 0
 * 0 ≤ x,y < n*m
 * 0 ≤ v ≤ n*m
 *
 * Postconditions:
 * The value in the (x,y) cell in the board is [v].
 */
void setBoardValue(Puzzle *puzzle, unsigned int x, unsigned int y, unsigned int v);

/**
 * This method applies a single move.
 *
 * Parameters:
 * Puzzle *puzzle
 * Move *move
 *
 * Preconditions:
 * puzzle, move != 0
 */
void applyActivitySingle(Puzzle *puzzle, Move *move);

/**
 * This method applies an activity.
 *
 * Parameters:
 * Puzzle *puzzle
 * LinkedList *action
 *
 * Preconditions:
 * puzzle, action, action->first != 0
 * action is a list of moves
 */
void applyActivity(Puzzle *puzzle, LinkedList *action);

/**
 * This method reverts a single move.
 *
 * Parameters:
 * Puzzle *puzzle
 * Move *move
 *
 * Preconditions:
 * puzzle, move != 0
 */
void revertActivitySingle(Puzzle *puzzle, Move *move);

/**
 * This method reverts an activity.
 *
 * Parameters:
 * Puzzle *puzzle
 * LinkedList *action
 *
 * Preconditions:
 * puzzle, action, action->first != 0
 * action is a list of moves
 */
void revertActivity(Puzzle *puzzle, LinkedList *action);

/**
 * This method prints [puzzle] to the screen.
 *
 * Parameters:
 * Puzzle *puzzle
 * Bool markErrorsFlag - whether the erroneous values should be indicated
 *
 * Preconditions:
 * puzzle != 0
 */
void printBoard(Puzzle *puzzle, Bool markErrorsFlag);

#endif