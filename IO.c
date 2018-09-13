#define _CRT_SECURE_NO_WARNINGS

#include "IO.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "utils/MemAlloc.h"

/**
 * This method returns the number of digits of an unsigned (decimal) integer.
 *
 * Parameters:
 * unsigned int d
 *
 * Returns:
 * The number of digit in the decimal representation of d
 */
static unsigned int digitNum(unsigned int d) {
	if (d == 0) {
		return 1;
	}
	return (unsigned int) floor(log10(d)) + 1;
}

/**
 * This method calculates the number of characters that are needed
 * for the string representation of the puzzle, not including the '\0' byte.
 *
 * Parameters:
 * Puzzle *puzzle
 *
 * Preconditions:
 * puzzle != 0
 *
 * Returns:
 * The number of characters that may represent the puzzle
 */
static unsigned int calcFileContentLength(Puzzle *puzzle) {
	unsigned int i, j, dim = puzzle->n * puzzle->m;
	unsigned int res;

	res = 2 + digitNum(puzzle->n) + digitNum(puzzle->m);

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			res += digitNum(getBoardValue(puzzle, i, j));
			res += isCellFixed(puzzle, i, j) ? 2 : 1;
		}
	}

	return res;
}

/**
 * This method returns a string representation of a puzzle.
 * The string is dynamically allocated.
 *
 * Parameters:
 * Puzzle *puzzle
 *
 * Preconditions:
 * puzzle != 0
 */
static char *genFileContent(Puzzle *puzzle) {
	unsigned int i, j, dim = puzzle->n * puzzle->m;
	char *p;
	char *memAllocN(res, char, calcFileContentLength(puzzle) + 1);
	p = res;

	p = res + sprintf(res, "%d %d\n", puzzle->m, puzzle->n);

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			p += sprintf(p, "%d", getBoardValue(puzzle, i, j));

			if (isCellFixed(puzzle, i, j)) {
				*p++ = '.';
				*p++ = ' ';
			} else {
				*p++ = ' ';
			}
		}
		*(p - 1) = '\n';
	}

	*p = '\0';

	return res;
}

Bool writePuzzleToFS(Puzzle *puzzle, char *filepath) {
	int i;
	char *fileContent;

	FILE *fp = fopen(filepath, "w");
	if (fp == NULL) {
		return FALSE;
	}

	fileContent = genFileContent(puzzle);
	i = fprintf(fp, "%s", fileContent);

	memFree(fileContent);
	fclose(fp);
	if (i <= 0) {
		return FALSE;
	}
	return TRUE;
}

Puzzle *readPuzzleFromFS(char *filepath, Bool fix) {
	Puzzle *res;
	unsigned int i, j, dim, s;
	int n, m, v;

	FILE *fp = fopen(filepath, "r");
	if (fp == NULL) {
		return 0;
	}

	assert(fscanf(fp, "%d %d", &n, &m) == 2);
	res = createPuzzle((unsigned int) n, (unsigned int) m);
	dim = n * m;

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim;) {
			s = fscanf(fp, "%d", &v);
			if (s == 1) {
				setBoardValue(res, i, j, (unsigned int) v);
				j++;
			} else {
				assert(fgetc(fp) == '.');
				if (fix) {
					if (j) {
						fixCell(res, i, j - 1);
					} else {
						fixCell(res, i - 1, dim - 1);
					}
				}
			}
		}
	}

	fclose(fp);
	return res;
}