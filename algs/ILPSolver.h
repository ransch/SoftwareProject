#ifndef __ALGS_ILPSOLVER_H
#define __ALGS_ILPSOLVER_H
/**
 * This module comprises the ILP based sudoku solver.
 */

#include "../dataStructures/Puzzle.h"

/**
  * This method solves p using ILP.
  *
  * Parameters:
  * Puzzle *p
  *
  * Preconditions:
  * p != 0
  *
  * Returns:
  * TRUE iff [p] is solvable
  *
  * Postconditions:
  * [p] is filled with values iff TRUE is returned
  */
Bool ILPSolver(Puzzle *p);

#endif