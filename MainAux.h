#ifndef __MAINAUX_H
#define __MAINAUX_H
/**
 * This module defines auxiliary functions that would be used by main.c.
 */

/*
 * There total number of bits that are sufficient to represent a value
 * from ParserFlags or from GameMode.
 */
#define ParserFlags_GameMode_Bits 2

/**
 * The default number of rows in a block.
 */
#define defaultN 3

/**
 *The default number of columns in a block.
 */
#define defaultM 3

/**
 * This enum defined the parser flags, which indicate unusual behaviors.
 */
typedef enum {
	illegalCommandFlag, terminateGameFlag
} ParserFlags;

/**
 * This enum defines the game modes, which define the current functionality and
 * the available commands.
 */
typedef enum {
	gameModeInit, gameModeEdit, gameModeSolve
} GameMode;

/**
 * This struct defined the feedback that is returned after parsing a command.
 */
typedef struct {

	/**
	 * This bit == 1 iff some unusual behavior sould be indicated.
	 * For example, the parsing has been failed, or the game should be terminated.
	 * If the bit == 0, the value is the next GameMode that should be set.
	 */
	unsigned int isFlag : 1;

	/**
	 * The value of the feedback - it may be the next GameMode that should be set,
	 * or some ParserFlags value.
	 */
	unsigned int value : ParserFlags_GameMode_Bits;
} ParserFeedback;

/**
 * This method returns the current mode of the game.
 */
GameMode getCurrentGameMode();

/**
 * This method is the main loop of the game.
 * In each iteration, a new command is read.
 *
 * Postconditions:
 * All the memory that was allocated during function's lifetime was freed.
 */
void mainLoop();

#endif
