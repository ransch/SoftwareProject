#include "Commands.h"
#include "../IO.h"
#include "../Shared.h"
#include "../Strings.h"
#include "../algs/SudokuAlgs.h"
#include "../dataStructures/Activity.h"
#include "../dataStructures/Puzzle.h"
#include "../utils/MemAlloc.h"
#include "../utils/Strings.h"
#include "Parser.h"
#include <stdio.h>
#include <string.h>

SharedBundle bundle;

#define returnGameMode(ret, mode) \
    ret.isFlag = 0;               \
    ret.value = mode;             \
    return ret

#define checkGameEnd if (!bundle.puzzle->zeroCnt) { \
						if (!isPuzzleLegal(bundle.puzzle)) { \
							printf("%s\n", infoMsgErrSolution); \
						} else { \
							printf("%s\n", infoMsgSuccessfulSolution); \
							returnGameMode(ret, gameModeInit); \
						} \
					}

#define printBoardReg printBoard(bundle.puzzle, bundle.markErrorsFlag || getCurrentGameMode() == gameModeEdit)

/**
 * The operation of the "set" command.
 */
static ParserFeedback setOp(LinkedList* args) {
#define finish(gameMode) returnGameMode(ret, gameMode)

	ParserFeedback ret;
	int x, y, v;
	Move* move;
	const char *arg1, *arg2, *arg3;
	int dim = bundle.puzzle->n * bundle.puzzle->m;
	LinkedListElem* elem = args->first;

	arg1 = elem->data;
	elem = elem->next;
	arg2 = elem->data;
	elem = elem->next;
	arg3 = elem->data;

	x = isUInteger(arg2);
	y = isUInteger(arg1);
	v = isUInteger(arg3);

	if ((x == -1 || y == -1 || v == -1) || !(x >= 1 && y >= 1 && v >= 0 && x <= dim && y <= dim && v <= dim)) {
		printf(errMsgNotInRange, dim);
		finish(getCurrentGameMode());
	}

	x -= 1;
	y -= 1;

	if (isCellFixed(bundle.puzzle, (unsigned int) x, (unsigned int) y)) {
		printf("%s\n", errMsgFixedCell);
		finish(getCurrentGameMode());
	}

	move = createMove((unsigned int) x,
		(unsigned int) y,
		((unsigned int) v) - getBoardValue(bundle.puzzle, (unsigned int) x, (unsigned int) y));
	applyActivitySingle(bundle.puzzle, move);
	addMove(bundle.activity, move);

	printBoardReg;

	if (getCurrentGameMode() == gameModeSolve) {
		checkGameEnd;
	}

	finish(getCurrentGameMode());
#undef finish
}

/**
 * The operation of the "hint" command.
 */
static ParserFeedback hintOp(LinkedList* args) {
#define finish returnGameMode(ret, getCurrentGameMode())

	ParserFeedback ret;
	int x, y;
	unsigned int h;
	const char *arg1, *arg2;
	int dim = bundle.puzzle->n * bundle.puzzle->m;
	LinkedListElem* elem = args->first;

	arg1 = elem->data;
	elem = elem->next;
	arg2 = elem->data;

	x = isUInteger(arg2);
	y = isUInteger(arg1);

	if ((x == -1 || y == -1) || !(x >= 1 && y >= 1 && x <= dim && y <= dim)) {
		printf(errMsgNotInRange, dim);
		finish;
	}

	x -= 1;
	y -= 1;

	if (isCellFixed(bundle.puzzle, (unsigned int) x, (unsigned int) y)) {
		printf("%s\n", errMsgFixedCell);
	} else if (!isPuzzleLegal(bundle.puzzle)) {
		printf("%s\n", errMsgErroneousValues);
	} else if (getBoardValue(bundle.puzzle, (unsigned int) x, (unsigned int) y) != 0) {
		printf("%s\n", errMsgNonEmptyCell);
	} else {
		h = calcCellHint(bundle.puzzle, (unsigned int) x, (unsigned int) y);
		if (h == 0) {
			printf("%s\n", errMsgUnsolvablePuzzle);
		} else {
			printf(infoMsgHint, h);
		}
	}

	finish;
#undef finish
}

/**
 * The operation of the "validate" command.
 */
static ParserFeedback validateOp(LinkedList* args) {
	ParserFeedback ret;

	if (args) {
	}
	if (!isPuzzleLegal(bundle.puzzle)) {
		printf("%s\n", errMsgErroneousValues);
	} else if (isPuzzleValid(bundle.puzzle)) {
		printf("%s\n", infoMsgValidPuzzle);
	} else {
		printf("%s\n", infoMsgInvalidPuzzle);
	}

	returnGameMode(ret, getCurrentGameMode());
}

/**
 * The operation of the "exit" command.
 */
ParserFeedback exitOp(LinkedList* args) {
	ParserFeedback ret;
	ret.isFlag = 1;
	ret.value = terminateGameFlag;

	if (args) {
	}
	printf("%s\n", infoMsgExiting);
	return ret;
}

static ParserFeedback solveOp(LinkedList* args) {
	ParserFeedback ret;
	destroyBundle();
	initBundle();

	bundle.puzzle = readPuzzleFromFS((char*) args->first->data, TRUE);
	if (bundle.puzzle == 0) {
		printf("%s\n", errMsgIOExistOpeningFailed);
		returnGameMode(ret, getCurrentGameMode());
	}

	printBoard(bundle.puzzle, bundle.markErrorsFlag);
	returnGameMode(ret, gameModeSolve);
}

static ParserFeedback editOp(LinkedList* args) {
	ParserFeedback ret;
	destroyBundle();
	initBundle();

	if (args->first->data) {
		bundle.puzzle = readPuzzleFromFS((char*) args->first->data, FALSE);
		if (bundle.puzzle == 0) {
			printf("%s\n", errMsgIOOpeningFailed);
			returnGameMode(ret, getCurrentGameMode());
		}
	} else {
		bundle.puzzle = createPuzzle(defaultN, defaultM);
	}

	printBoard(bundle.puzzle, TRUE);
	returnGameMode(ret, gameModeEdit);
}

static ParserFeedback markErrorsOp(LinkedList* args) {
	ParserFeedback ret;
	int x;
	LinkedListElem* elem = args->first;
	const char *arg = elem->data;

	x = isUInteger(arg);

	if (x != 0 && x != 1) {
		printf("%s\n", errMsgErroneousMarkErrorsVal);
	} else {
		bundle.markErrorsFlag = x;
	}

	returnGameMode(ret, getCurrentGameMode());
#undef finish
}

static ParserFeedback printBoardOp(LinkedList* args) {
	ParserFeedback ret;

	if (args) {
	}
	printBoardReg;

	returnGameMode(ret, getCurrentGameMode());
}

/**
 * This method returns TRUE iff the board is empty.
 *
 * Parameters:
 * Puzzle *puzzle - A puzzle
 *
 * Returns:
 * FALSE iff ∃i∃j s.t. 0 ≤ i,j < puzzle->n * puzzle->m ∧ getBoardValue(puzzle,
 * i, j) ≠ 0.
 */
static Bool isEmpty(Puzzle* puzzle) {
	unsigned int i, j;
	unsigned int dim = puzzle->n * puzzle->m;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (getBoardValue(puzzle, i, j) != 0) {
				return FALSE;
			}
		}
	}
	return TRUE;
}

static LinkedList* buildAction(Puzzle* puzzle) {
	LinkedList* res = createList();
	unsigned int i, j;
	unsigned int dim = puzzle->n * puzzle->m;

	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (getBoardValue(bundle.puzzle, i, j)) {
				appendElemToList(res, createListElem(createMove(i, j, getBoardValue(bundle.puzzle, i, j))));
			}
		}
	}

	return res;
}

/**
 * This method counts the number of blank cells in [p].
 *
 * Parameters:
 * Puzzle *p
 *
 * Preconditions:
 * p != 0
 *
 * Returns:
 * # {(x,y) | 0 <= x,y < p->n*p->m ∧ !getBoardValue(p, x, y)}
 */
static unsigned int countZeros(Puzzle *p) {
	unsigned int i, j, dim = p->n * p->m, res = 0;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (!getBoardValue(p, i, j)) {
				res++;
			}
		}
	}
	return res;
}

static ParserFeedback generateOp(LinkedList* args) {
	ParserFeedback ret;
	int x, y;
	const char *arg1, *arg2;
	unsigned int blankCount = countZeros(bundle.puzzle);
	LinkedList* action;
	LinkedListElem* elem = args->first;

	arg1 = elem->data;
	elem = elem->next;
	arg2 = elem->data;

	x = isUInteger(arg1);
	y = isUInteger(arg2);

	if ((x == -1 || y == -1) || !(x >= 0 && y >= 0 && ((unsigned int) x) <= blankCount && ((unsigned int) y) <= blankCount)) {
		printf(errMsgNotInRange, blankCount);
	} else if (!isEmpty(bundle.puzzle)) {
		printf("%s\n", errMsgNonEmptyBoard);
	} else {
		if (y == 0) {
			printBoardReg;
		} else {
			if (generatePuzzle(bundle.puzzle, (unsigned int) x, (unsigned int) y)) {
				action = buildAction(bundle.puzzle);
				addAction(bundle.activity, action);
				printBoardReg;
			} else {
				printf("%s\n", errMsgGenFailed);
			}
		}
	}

	returnGameMode(ret, getCurrentGameMode());
}

/**
 * This method gets an element of a linked-list that wraps a Move,
 * and prints the details of the undo.
 *
 * Preconditions:
 * elem != 0
 * elem->data points at a Move
 */
static void undoPrintActionElem(LinkedListElem* elem) {
	Move* move = (Move*) elem->data;
	unsigned int v = getBoardValue(bundle.puzzle, move->x, move->y);
	char c1 = v + move->delta ? '0' + v + move->delta : '_';
	char c2 = v ? '0' + v : '_';
	printf(infoMsgUndo, move->y + 1, move->x + 1, c1, c2);
}

/**
 * This method gets an element of a linked-list that wraps a Move,
 * and prints the details of the redo.
 *
 * Preconditions:
 * elem != 0
 * elem->data points at a Move
 */
static void redoPrintActionElem(LinkedListElem* elem) {
	Move* move = (Move*) elem->data;
	unsigned int v = getBoardValue(bundle.puzzle, move->x, move->y);
	char c1 = v - move->delta ? '0' + v - move->delta : '_';
	char c2 = v ? '0' + v : '_';
	printf(infoMsgRedo, move->y + 1, move->x + 1, c1, c2);
}

static ParserFeedback undoOp(LinkedList* args) {
	ParserFeedback ret;
	LinkedList* currentAction;

	if (args) {
	}
	if (!canRegressActivity(bundle.activity)) {
		printf("%s\n", errMsgCannotUndo);
	} else {
		currentAction = getCurrentAction(bundle.activity);
		regressActivity(bundle.activity);
		revertActivity(bundle.puzzle, currentAction);
		printBoardReg;
		forEachListElem(currentAction, undoPrintActionElem);
	}

	returnGameMode(ret, getCurrentGameMode());
}

static ParserFeedback redoOp(LinkedList* args) {
	ParserFeedback ret;
	LinkedList* currentAction;

	if (args) {
	}
	if (!canProgressActivity(bundle.activity)) {
		printf("%s\n", errMsgCannotRedo);
	} else {
		progressActivity(bundle.activity);
		currentAction = getCurrentAction(bundle.activity);
		applyActivity(bundle.puzzle, currentAction);
		printBoardReg;
		forEachListElem(currentAction, redoPrintActionElem);
	}

	returnGameMode(ret, getCurrentGameMode());
}

static ParserFeedback saveOp(LinkedList* args) {
#define writePuzzleM(puzz)                         \
    if (writePuzzleToFS(puzz, path)) {             \
        printf(infoMsgSavedToFS, path);            \
    } else {                                       \
        printf("%s\n", errMsgIOCreationModFailed); \
    }
#define finish returnGameMode(ret, getCurrentGameMode())

	ParserFeedback ret;
	Puzzle* clone;
	unsigned int i, j;
	char *path = (char*) args->first->data;

	if (getCurrentGameMode() == gameModeEdit) {
		if (!isPuzzleLegal(bundle.puzzle)) {
			printf("%s\n", errMsgErroneousValues);
			finish;
		} else if (!isPuzzleValid(bundle.puzzle)) {
			printf("%s\n", errMsgInvalidBoard);
			finish;
		}

		clone = clonePuzzle(bundle.puzzle);

		for (i = 0; i < clone->n * clone->m;
			i++) { /* fix all the non-empty cells */
			for (j = 0; j < clone->n * clone->m; j++) {
				if (getBoardValue(clone, i, j)) {
					fixCell(clone, i, j);
				}
			}
		}

		writePuzzleM(clone);
		destroyPuzzle(clone);
		finish;
	}

	writePuzzleM(bundle.puzzle);
	finish;

#undef writePuzzleM
#undef finish
}

static ParserFeedback numSolutionsOp(LinkedList* args) {
#define finish returnGameMode(ret, getCurrentGameMode())
	ParserFeedback ret;
	unsigned int solutionsNum;

	if (args) {
	}

	if (!isPuzzleLegal(bundle.puzzle)) {
		printf("%s\n", errMsgErroneousValues);
		finish;
	}

	solutionsNum = calcSolutionsNum(bundle.puzzle);

	printf(infoMsgSolutionsNum, solutionsNum);

	if (solutionsNum == 1) {
		printf("%s\n", infoMsgSingleSolution);
	} else if (solutionsNum > 1) {
		printf("%s\n", infoMsgMultipleSolutions);
	}

	finish;
#undef finish
}

static ParserFeedback autofillOp(LinkedList* args) {
#define finish(gamemode) returnGameMode(ret, gamemode)
	ParserFeedback ret;
	unsigned int i, j, dim, v, c = 0;
	LinkedList* action = createList();

	if (args) {
	}
	if (!isPuzzleLegal(bundle.puzzle)) {
		printf("%s\n", errMsgErroneousValues);
		destroyList(action);
		finish(getCurrentGameMode());
	}

	dim = bundle.puzzle->n * bundle.puzzle->m;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (!getBoardValue(bundle.puzzle, i, j)) {
				v = isSingleLegalValue(bundle.puzzle, i, j);
				if (v) {
					c++;
					printf(infoMsgCellSet, j + 1, i + 1, v);

					appendElemToList(action,
						createListElem(createMove(
							i, j, v - getBoardValue(bundle.puzzle, i, j))));
				}
			}
		}
	}

	if (!c) {
		destroyList(action);
		printBoardReg;
		finish(getCurrentGameMode());
	}

	addAction(bundle.activity, action);
	applyActivity(bundle.puzzle, action);

	printBoardReg;

	checkGameEnd;

	finish(getCurrentGameMode());
#undef finish
}

static ParserFeedback resetOp(LinkedList* args) {
	ParserFeedback ret;

	if (args) {
	}
	while (canRegressActivity(bundle.activity)) {
		revertActivity(bundle.puzzle, getCurrentAction(bundle.activity));
		regressActivity(bundle.activity);
	}

	destroyActivity(bundle.activity);
	bundle.activity = createActivity();

	printf("%s\n", infoMsgBoardReset);
	returnGameMode(ret, getCurrentGameMode());
}

LinkedList* createCommandsList() {
	LinkedList* commands = createList();

	EnumSubset* allModes = createEnumSubset();
	EnumSubset* editSolveModes = createEnumSubset();
	EnumSubset* editMode = createEnumSubset();
	EnumSubset* solveMode = createEnumSubset();
	EnumSubsetTurnOn(allModes, gameModeInit);
	EnumSubsetTurnOn(allModes, gameModeEdit);
	EnumSubsetTurnOn(allModes, gameModeSolve);
	EnumSubsetTurnOn(editSolveModes, gameModeEdit);
	EnumSubsetTurnOn(editSolveModes, gameModeSolve);
	EnumSubsetTurnOn(editMode, gameModeEdit);
	EnumSubsetTurnOn(solveMode, gameModeSolve);

	appendElemToList(commands, createListElem(createCommand(cmdNameSolve, 1, allModes, solveOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameEdit, 1, allModes, editOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameMarkErrors, 1, solveMode, markErrorsOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNamePrintBoard, 0, editSolveModes, printBoardOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameSet, 3, editSolveModes, setOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameValidate, 0, editSolveModes, validateOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameGenerate, 2, editMode, generateOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameUndo, 0, editSolveModes, undoOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameRedo, 0, editSolveModes, redoOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameSave, 1, editSolveModes, saveOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameHint, 2, solveMode, hintOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameNumSolutions, 0, editSolveModes, numSolutionsOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameAutofill, 0, solveMode, autofillOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameReset, 0, editSolveModes, resetOp)));
	appendElemToList(commands, createListElem(createCommand(cmdNameExit, 0, allModes, exitOp)));

	destroyEnumSubset(allModes);
	destroyEnumSubset(editSolveModes);
	destroyEnumSubset(editMode);
	destroyEnumSubset(solveMode);

	return commands;
}

/**
 * This mathod gets an element of a linked-list that wraps a Command,
 * and destroys that command.
 *
 * Preconditions:
 * elem != 0
 * elem->data points at a Command
 */
static void destroyElemData(LinkedListElem* elem) {
	destroyCommand((Command*) elem->data);
}

void freeCommands(LinkedList* commands) {
	forEachListElem(commands, destroyElemData);
}

#undef returnGameMode
#undef checkGameEnd