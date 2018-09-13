#ifndef __PARSER_COMMANDS_H
#define __PARSER_COMMANDS_H
/**
 * This module defines all the commands that are available in the program.
 */

#include "../utils/dataStructures/DoublyLinkedList.h"
#include "../MainAux.h"

/**
 * This method returnes a list that contains all the commands
 * that are available in the program.
 */
LinkedList *createCommandsList();

/**
 * This function is the operation of the "exit" command.
 *
 * Parameters:
 * LinkedList *args - A list that contains the command's arguments, may be empty.
 *
 * Returns:
 * ParserFeedback
 */
ParserFeedback exitOp(LinkedList *args);

/**
 * This method frees the contents in a list of commands.
 * Note that the list should be destroyed seperately.
 *
 * Parameters:
 * LinkedList *commands - A list that contains commands.
 */
void freeCommands(LinkedList *commands);

#endif
