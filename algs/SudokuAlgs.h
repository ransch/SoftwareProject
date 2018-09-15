#ifndef __ALGS_SUDOKUALGS_H
#define __ALGS_SUDOKUALGS_H
/**
 * This module comprises the basic sudoku-related algorithms in the project.
 */

#include "../utils/Boolean.h"
#include "../dataStructures/Puzzle.h"

/**
  *This method returns true iff every cell in the puzzle contains a legal value.
  *
  * Parameters:
  * Puzzle *p
  *
  * Preconditions:
  * p != 0
  *
  * Returns:
  * TRUE iff ∀x∀y: 0 <= x,y < p->n * p->m → (getBoardValue(p, x, y) is legal)
 */
Bool isPuzzleLegal(Puzzle *p);

/**
 * This method returns true iff the board can be solved.
 *
 * Parameters:
 * Puzzle *p
 *
 * Preconditions:
 * p != 0
 *
 * Returns:
 * TRUE iff the board is solvable
 */
Bool isPuzzleValid(Puzzle *p);

/**
 * This method returns a hint for a specific cell in the board.
 * A hint is a legal value that preserves the validity of the puzzle.
 * In particular, if [p] cannot be solved, there does not exist any hint for any cell.
 *
 * Parameters:
 * Puzzle *p
 * unsigned int x - the row
 * unsigned int y - the column
 *
 * Preconditions:
 * p != 0
 * 0 <= x,y < p->n * p->m
 * !isCellFixed(p, x, y)
 * isPuzzleLegal(p)
 * !getBoardValue(p, x, y)
 *
 * Returns:
 * A hint for the cell (x,y) if the board is solvable, 0 otherwise.
 */
unsigned int calcCellHint(Puzzle *p, unsigned int x, unsigned int y);

/**
 * This method generates a new sudoku puzzle.
 *
 * Parameters:
 * Puzzle *p - An empty puzzle
 * unsigned int x, unsigned int y - randomization parameters
 *
 * Returns:
 * TRUE iff the puzzle was generated successfully.
 */
Bool generatePuzzle(Puzzle *p, unsigned int x, unsigned int y);

/**
  *This method returns the number of possible solutions for the given puzzle
  *
  *Parameters:
  *Puzzle *puzzle
  *
  *Preconditions:
  *puzzle != 0
  */
unsigned int calcSolutionsNum(Puzzle *p);

/**
  *This method checks if there's only a single value that would be legal to assign to cell (x,y) and returns it
 *
  *Parameters:
  *Puzzle *p
  *unsigned int x - The row
  *unsigned int y - The column
  *
  *Preconditions:
  *p != 0
  *x,y < n*m
  *
  *Returns:
  *The single legal value. if there's none, or there's more than one value, then it returns 0
  */
unsigned int isSingleLegalValue(Puzzle *p, unsigned int x, unsigned int y);

/**
  *This method returns true iff the cell (x,y) contains a legal value
  *
  *Parameters:
  *Puzzle *puzzle
  *unsigned int x - The row
  *unsigned int y - The column
  *
  *Preconditions:
  *p != 0
  *x,y < n*m
  */
Bool isCellLegal(Puzzle *p, unsigned int x, unsigned int y);

/**
 * This method clones a puzzle, and fixes all the non-empty cells in the clone.
 */
Puzzle* copyPuzzle(Puzzle* puzzle);

/**
  *This method returns true iff the assignment of value CellValue to cell (x,y) would be legal
  *
  *Parameters:
  *Puzzle *p
  *unsigned int x - The row
  *unsigned int y - The column
  *unsigned int CellValue
  *
  *Preconditions:
  *p != 0
  *x,y,CellValue < n*m
  */
Bool isCellValueLegal(Puzzle *p, unsigned int x, unsigned int y, unsigned int CellValue);

/**
  *This method gets an index of a cell in the board and makes it point to the next unfixed cell
  *
  *Parameters:
  *Puzzle *puzzle
  *unsigned int *i - Pointer to the row
  *unsigned int *j - Pointer to the column
  *unsigned int puzzleSize - The size of the puzzle
  *
  *Preconditions:
  *puzzle != 0
  **i,*j < n*m
  *puzzleSize = n*m
  *
  *Returns:
  *1 iff an unfixed cell was found
  */
Bool proceed(Puzzle *puzzle, unsigned int *i, unsigned int *j, unsigned int puzzleSize);

/**
  *This method gets an index of a cell in the board and makes it point to the previous unfixed cell
  *
  *Parameters:
  *Puzzle *puzzle
  *unsigned int *i - Pointer to the row
  *unsigned int *j - Pointer to the column
  *unsigned int puzzleSize - The size of the puzzle
  *
  *Preconditions:
  *puzzle != 0
  **i,*j < n*m
  *puzzleSize = n*m
  *
  *Returns:
  *1 iff an unfixed cell was found
  */
Bool precede(Puzzle *puzzle, unsigned int *i, unsigned int *j, unsigned int puzzleSize);

#endif