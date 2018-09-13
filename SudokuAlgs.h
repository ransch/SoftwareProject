#ifndef __SUDOKUALGS_H
#define __SUDOKUALGS_H

#include "utils/Boolean.h"
#include "dataStructures/Puzzle.h"
#include "utils/dataStructures/Stack.h"

/**
  *This method returns true iff every cell in the puzzle contains a legal value
  *
  *Parameters:
  *Puzzle *p
  *
  *Preconditions:
  *p != 0
 */
Bool isPuzzleLegal(Puzzle *p);

/**
 * This method
 */
Bool isPuzzleValid(Puzzle *p);

/**
 * This method
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

#endif