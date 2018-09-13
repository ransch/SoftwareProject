#ifndef __PARSER_PARSER_H
#define __PARSER_PARSER_H
/**
 * This module reads and interprets the user input.
 */

#include "../utils/EnumSubset.h"
#include "../utils/dataStructures/DoublyLinkedList.h"
#include "../MainAux.h"

/**
 * The maximal length of user input that may be read.
 */
#define maxInputLength 256

/**
 * This struct defines a command that may be entered by the user.
 */
typedef struct {

	/**
	 * This command's name.
	 */
	char *name;

	/**
	 * The command's arity.
	 */
	int argNum;

	/**
	 * A non-empty subset from the GameMode enum that defines the modes in which this
	 * command is permitted.
	 */
	EnumSubset *fromModes;

	/**
	 * This command's operation function.
	 * Parameters:
	 * LinkedList *args - A list that contains the command's arguments
	 *
	 * Preconditions:
	 * args != 0
	 * The length of args <= argNum
	 *
	 * Returns:
	 * ParserFeedback
	 */
	ParserFeedback(*operate)(LinkedList *args);
} Command;

/**
 * This method returns a pointer to a new variable whose type is Command.
 *
 * Parameters:
 * char *name - The command's name
 * int argNum - The command's arity
 * EnumSubset *fromModes - A non-empty subset from the GameMode enum that defines the modes in which this
 *		command is permitted. Note that the enum is cloned.
 * ParserFeedback (*operate)(LinkedList*) - The command's operation function as described in "Command" struct.
 */
Command *createCommand(char *name, int argNum, EnumSubset *fromModes, ParserFeedback(*operate)(LinkedList *args));

/**
 * This method frees a command from the memory.
 *
 * Arguments:
 * Command *command - A pointer to a command that should be freed from the memory.
 *
 * Preconditions:
 * command != NULL
 */
void destroyCommand(Command *command);

/**
 * This method reads and interprets a single line of input.
 *
 * Returns:
 * ParserFeedback f
 */
ParserFeedback listen();

/**
 * This method initializes the parser.
 */
void initParser();

/**
 * This method frees the memory allocated by the parser.
 */
void freeParser();

#endif
