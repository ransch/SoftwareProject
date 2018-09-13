#ifndef __IO_H
#define __IO_H
/**
 * This module contains all the IO operations of the program.
 */

#include "utils/Boolean.h"
#include "dataStructures/Puzzle.h"

/**
 * This method writes a puzzle to the filesystem.
 *
 * Parameters:
 * Puzzle *puzzle
 * char *filepath
 *
 * Preconditions:
 * puzzle, filepath != 0
 *
 * Returns:
 * TRUE iff the operation succeeded.
 */
Bool writePuzzleToFS(Puzzle *puzzle, char *filepath);

/**
 * This method reads a puzzle from the filesystem. The file access mode that is used is "r".
 *
 * Parameters:
 * char *filepath
 * Bool fix - Should the "fix" chars be read
 *
 * Preconditions:
 * filepath != 0
 *
 * Returns:
 * A pointer to a dynamically allocated puzzle that is represented by
 * the file [filepath]. If fix == FALSE, each cell is not-fixed.
 */
Puzzle *readPuzzleFromFS(char *filepath, Bool fix);

#endif