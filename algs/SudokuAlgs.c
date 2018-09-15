#include "SudokuAlgs.h"
#include "exhBacktr.h"
#include "ILPSolver.h"
#include "../utils/MemAlloc.h"

#define generateMaxTrials 1000

Puzzle* copyPuzzle(Puzzle* puzzle) {
	unsigned int i, j;
	unsigned int size = puzzle->m * puzzle->n;
	Puzzle* localP = createPuzzle(puzzle->n, puzzle->m);
	localP->zeroCnt = puzzle->zeroCnt;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			localP->board[i][j][0] = puzzle->board[i][j][0];
			if (getBoardValue(puzzle, i, j)) {
				fixCell(localP, i, j);
			}
		}
	}
	return localP;
}


/**
  *This method sets RowValues to contain the values in the same row of (x,y), excluding (x,y) itself.
  *
  *Parameters:
  *Puzzle *puzzle
  *unsigned int x - The row
  *unsigned int y - The column
  *unsigned int *RowValues - An array whose size is n*m - 1
  *
  *Preconditions:
  *puzzle != 0
  *x,y < n*m
  *RowValues is an array of size n*m - 1
  *
  *Postconditions:
  *RowValues contains all the values in the row x, excluding the value of cell (x,y)
  */
static void getRowValues(Puzzle *puzzle, unsigned int x, unsigned int y, unsigned int *RowValues) {
	unsigned int i;
	unsigned int count = 0;
	for (i = 0; i < puzzle->m*puzzle->n; i++) {
		if (i != y)
			RowValues[count++] = getBoardValue(puzzle, x, i);
	}
}

/**
  *This method sets ColValues to contain the values in the same column of (x,y), excluding (x,y) itself.
  *
  *Parameters:
  *Puzzle *puzzle
  *unsigned int x - The row
  *unsigned int y - The column
  *unsigned int *ColValues - An array whose size is n*m - 1
  *
  *Preconditions:
  *puzzle != 0
  *x,y < n*m
  *ColValues is an array of size n*m - 1
  *
  *Postconditions:
  *ColValues contains all the values in the column y, excluding the value of cell (x,y)
  */
static void getColValues(Puzzle *puzzle, unsigned int x, unsigned int y, unsigned int *ColValues) {
	unsigned int i;
	unsigned int count = 0;
	for (i = 0; i < puzzle->m*puzzle->n; i++) {
		if (i != x)
			ColValues[count++] = getBoardValue(puzzle, i, y);
	}
}

/**
  *This method sets BlockValues to contain the values in the same block of (x,y), excluding (x,y) itself.
  *
  *Parameters:
  *Puzzle *puzzle
  *unsigned int x - The row
  *unsigned int y - The column
  *unsigned int *BlockValues - An array whose size is n*m - 1
  *
  *Preconditions:
  *puzzle != 0
  *x,y < n*m
  *BlockValues is an array of size n*m - 1
  *
  *Postconditions:
  *BlockValues contains all the values in the block of the cell (x,y), excluding the value of cell (x,y)
  */
static void getBlockValues(Puzzle *puzzle, unsigned int x, unsigned int y, unsigned int* BlockValues) {
	unsigned int BlockRow = x / puzzle->n;
	unsigned int BlockCol = y / puzzle->m;
	unsigned int i, j;
	int count = 0;
	for (i = BlockRow * puzzle->n; i < (BlockRow + 1)*puzzle->n; i++)
		for (j = BlockCol * puzzle->m; j < (BlockCol + 1)*puzzle->m; j++)
			if (i != x || j != y)
				BlockValues[count++] = getBoardValue(puzzle, i, j);
}

Bool isCellValueLegal(Puzzle *p, unsigned int x, unsigned int y, unsigned int CellValue) {
	unsigned int BlockSize = p->m  *p->n;
	unsigned int i;
	unsigned int *memAllocN(Values, unsigned int, BlockSize - 1);
	getRowValues(p, x, y, Values);
	for (i = 0; i < BlockSize - 1; i++)
		if (Values[i] == CellValue) {
			memFree(Values);
			return FALSE;
		}
	getColValues(p, x, y, Values);
	for (i = 0; i < BlockSize - 1; i++)
		if (Values[i] == CellValue) {
			memFree(Values);
			return FALSE;
		}
	getBlockValues(p, x, y, Values);
	for (i = 0; i < BlockSize - 1; i++)
		if (Values[i] == CellValue) {
			memFree(Values);
			return FALSE;
		}
	memFree(Values);
	return TRUE;
}

Bool isPuzzleLegal(Puzzle *p) {
	unsigned int i, j;
	for (i = 0; i < p->m*p->n; i++)
		for (j = 0; j < p->m*p->n; j++) {
			if (!isCellLegal(p, i, j))
				return FALSE;
		}
	return TRUE;
}

Bool isPuzzleValid(Puzzle *p) {
	Puzzle *pclone = clonePuzzle(p);
	Bool solvable = ILPSolver(pclone);
	destroyPuzzle(pclone);
	return solvable;
}

unsigned int calcCellHint(Puzzle *p, unsigned int x, unsigned int y) {
	Puzzle *pclone = clonePuzzle(p);
	Bool solvable = ILPSolver(pclone);
	unsigned int ret = 0;
	if (solvable) {
		ret = getBoardValue(pclone, x, y);
	}
	destroyPuzzle(pclone);
	return ret;
}

/**
 * This method zeros all the values in a given puzzle.
 *
 * Parameters:
 * Puzzle *p - A pointer to the puzzle that would be cleared
 *
 * Postconditions:
 * ∀i,j: 0 <= i,j <= p->n * p->m → getBoardValue(p, i, j) == 0
 */
static void clearBoard(Puzzle *p) {
	unsigned int i, j, dim;
	dim = p->n * p->m;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			setBoardValue(p, i, j, 0);
		}
	}
}

/**
 * This method returns a pointer to a dynamically allocated array
 * that contains all the legal values for a certain cell.
 *
 * Parameters:
 * Puzzle *p
 * unsigned int x - the row
 * unsigned int y - the column
 *
 * Preconditions:
 * x,y <= p->n * p->m
 *
 * Returns:
 * A pointer to a dynamically allocated array that contains all the legal
 * values for the cell (x,y). The last cell in the array is 0.
 */
static unsigned int *possibleVals(Puzzle *p, unsigned int x, unsigned int y) {
	unsigned int *rowValues, *colValues, *blockValues, *ret;
	unsigned char *options;
	unsigned int dim = p->n * p->m, count = dim, i, j;
	memAllocN(rowValues, unsigned int, dim);
	memAllocN(colValues, unsigned int, dim);
	memAllocN(blockValues, unsigned int, dim);
	memAllocN(options, unsigned char, dim);
	getRowValues(p, x, y, rowValues);
	getColValues(p, x, y, colValues);
	getBlockValues(p, x, y, blockValues);

	for (i = 0; i < dim; i++) { /* options[i] == 1 iff (i - 1) is illegal */
		if (rowValues[i] != 0 && !options[rowValues[i] - 1]) {
			options[rowValues[i] - 1] = 1;
			count--;
		}
		if (colValues[i] != 0 && !options[colValues[i] - 1]) {
			options[colValues[i] - 1] = 1;
			count--;
		}
		if (blockValues[i] != 0 && !options[blockValues[i] - 1]) {
			options[blockValues[i] - 1] = 1;
			count--;
		}
	}

	memAllocN(ret, unsigned int, count + 1);
	for (i = 0, j = 0; i < dim; i++) {
		if (!options[i]) {
			ret[j++] = i + 1;
		}
	}

	free(rowValues);
	free(colValues);
	free(blockValues);
	free(options);

	return ret;
}

/**
 * This method receives an empty puzzle and an unsigned integer x,
 * and randomly fills [x] cells with legal values.
 * If one of the randomly selected cells cannot be filled,
 * the board is cleaned and the returned value is FALSE.
 *
 * Parameters:
 * Puzzle *p
 * unsigned int x - the number of the cells to fill
 *
 * Preconditions:
 * p points an empty board
 * x <= p->n * p->m
 *
 * Returns:
 * TRUE iff the board has been filled successfully
 */
static Bool fillRndVals(Puzzle *p, unsigned int x) {
	unsigned int i, j, count = 0, vind;
	unsigned int dim = p->n * p->m;
	unsigned int *vals, valslen = 0;

	if (!x) {
		return TRUE;
	}

	while (count < x) {
		i = rand() % dim;
		j = rand() % dim;
		if (!getBoardValue(p, i, j)) {
			vals = possibleVals(p, i, j);
			valslen = 0;
			while (vals[valslen++]); valslen--;

			if (!valslen) { /* no possible values for this cell */
				free(vals);
				clearBoard(p);
				return FALSE;
			} else {
				vind = rand() % valslen;
				setBoardValue(p, i, j, vals[vind]);
				count++;
			}

			free(vals);
		}
	}

	return TRUE;
}

/**
 * This method randomly zeros [(p->n * p->m) ^ 2 - y] cells from [p];
 *
 * Parameters:
 * Puzzle *p
 * unsigned int y - number of cells that would remain non-zero
 */
static void clearBut(Puzzle *p, unsigned int y) {
	unsigned int dim = p->n * p->m, dim2 = dim * dim, i, j, count;

	if (y == dim2) {
		return;
	}

	for (count = 0; count < dim2 - y; count++) {
		do {
			i = rand() % dim;
			j = rand() % dim;
		} while (!getBoardValue(p, i, j));

		setBoardValue(p, i, j, 0);
	}
}

Bool generatePuzzle(Puzzle *p, unsigned int x, unsigned int y) {
	unsigned int t;

	if (y == 0) {
		return TRUE;
	}

	for (t = 0; t < generateMaxTrials; t++) {
		if (!fillRndVals(p, x) || !ILPSolver(p)) {
			continue;
		} else {
			clearBut(p, y);
			return TRUE;
		}
	}

	return FALSE;
}

Bool proceed(Puzzle *puzzle, unsigned int *i, unsigned int *j, unsigned int puzzleSize) {
	unsigned int a = *i;
	unsigned int b = *j;
	if (*i == puzzleSize - 1 && *j == puzzleSize - 1)
		return FALSE;
	while (a < puzzleSize - 1 || b < puzzleSize - 1) {
		if (b == puzzleSize - 1) {
			b = 0;
			a++;
		} else
			b++;
		if (!isCellFixed(puzzle, a, b)) {
			*i = a;
			*j = b;
			return TRUE;
		}
	}
	return FALSE;
}

Bool precede(Puzzle *puzzle, unsigned int *i, unsigned int *j, unsigned int puzzleSize) {
	unsigned int a = *i;
	unsigned int b = *j;
	if (*i == 0 && *j == 0)
		return FALSE;
	while (a > 0 || b > 0) {
		if (b == 0) {
			b = puzzleSize - 1;
			a--;
		} else
			b--;
		if (!isCellFixed(puzzle, a, b)) {
			*i = a;
			*j = b;
			return TRUE;
		}
	}
	return FALSE;
}

unsigned int calcSolutionsNum(Puzzle *p) {
	unsigned int i = 0, j = 0;
	unsigned int size = p->m*p->n;
	int ret;
	Puzzle *localP;

	localP = copyPuzzle(p);
	if (isCellFixed(localP, i, j))
		if (proceed(localP, &i, &j, size) == FALSE) {
			destroyPuzzle(localP);
			return 1;
		}
	ret = exhaustiveBacktracking(localP, i, j);
	destroyPuzzle(localP);
	return ret;
}

unsigned int isSingleLegalValue(Puzzle *p, unsigned int x, unsigned int y) {
	unsigned int i, value = 0;
	for (i = 1; i <= p->m*p->n; i++) {
		if (isCellValueLegal(p, x, y, i)) {
			if (value == 0)
				value = i;
			else
				return 0;
		}
	}
	return value;
}

Bool isCellLegal(Puzzle *p, unsigned int x, unsigned int y) {
	unsigned int i;
	unsigned int BlockSize = p->m  *p->n;
	unsigned int CellValue = getBoardValue(p, x, y);
	unsigned int *memAllocN(Values, unsigned int, BlockSize - 1);
	if (getBoardValue(p, x, y) == 0) {
		memFree(Values);
		return TRUE;
	}
	getRowValues(p, x, y, Values);
	for (i = 0; i < BlockSize - 1; i++)
		if (Values[i] == CellValue) {
			memFree(Values);
			return FALSE;
		}
	getColValues(p, x, y, Values);
	for (i = 0; i < BlockSize - 1; i++)
		if (Values[i] == CellValue) {
			memFree(Values);
			return FALSE;
		}
	getBlockValues(p, x, y, Values);
	for (i = 0; i < BlockSize - 1; i++)
		if (Values[i] == CellValue) {
			memFree(Values);
			return FALSE;
		}
	memFree(Values);
	return TRUE;
}

#undef generateMaxTrials