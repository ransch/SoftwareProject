/**
 * This is the main C source file of the program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MainAux.h"
#include "Strings.h"

/**
 * The program uses time(0) as a seed for PRNG.
 */
int main() {
	unsigned int seed;
	seed = (unsigned int) time(0);
	srand(seed);

	printf("%s\n", infoMsgBeginning);
	mainLoop();

	return 0;
}