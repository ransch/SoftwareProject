#include "MainAux.h"
#include <stdio.h>
#include <assert.h>
#include "Shared.h"
#include "Strings.h"
#include "dataStructures/Puzzle.h"
#include "parser/Parser.h"

/**
 * The current game mode.
 */
static GameMode currentMode;

SharedBundle bundle;

void mainLoop() {
	ParserFeedback c;
	currentMode = gameModeInit;

	initBundle();
	bundle.markErrorsFlag = 1;
	initParser();

	while (1) {
		printf("%s\n", infoMsgListening);
		c = listen();

		if (c.isFlag) {
			if (c.value == illegalCommandFlag) {
				printf("%s\n", errMsgInvalidCommand);
				continue;
			}
			assert(c.value == terminateGameFlag);
			break;
		}

		currentMode = c.value;
	}

	destroyBundle();
	freeParser();
}

GameMode getCurrentGameMode() {
	return currentMode;
}
