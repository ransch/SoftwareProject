#ifndef __ALGS_EXHBACKTR_H
#define __ALGS_EXHBACKTR_H
/**
 * This module implements the exhaustive backtracking algorithm.
 */

#include "../dataStructures/Puzzle.h"

/**
  * This method implements the exhaustive backtracking algorithm
  * used in the calcSolutionsNum function in the SudokuAlgs module.
  *
  * Parameters:
  * Puzzle *p
  * unsigned int i - The row
  * unsigned int j - The column
  *
  * Preconditions:
  * p != 0
  * i,j < p->n * p->m
  */
int exhaustiveBacktracking(Puzzle *p, unsigned int i, unsigned int j);

#endif