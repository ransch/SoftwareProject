#define _CRT_SECURE_NO_WARNINGS

#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Commands.h"
#include "../utils/MemAlloc.h"
#include "../utils/Strings.h"

#define illegalCmd ret.isFlag = 1; ret.value = illegalCommandFlag; return ret

/**
 * The separating chars between the command's name and the arguments in the user's input.
 */
#define delimiter " \t\r\n"

/**
 * This array contains the allowed commands in the program.
 */
LinkedList *commands;

void initParser() {
	commands = createCommandsList();
}

void freeParser() {
	freeCommands(commands);
	destroyList(commands);
}

/**
 * This method gets a command's name, reads the necessary arguments,
 * evaluates the command and returns the appropriate feedback.
 *
 * Parameters:
 * char *command - The name of the command
 *
 * Preconditions:
 * command != 0
 * The name of the command was the last thing that was read using strtok.
 * Therefore, we are able to read the command's arguments by strtok(NULL, ).
 *
 * Returns:
 * ParserFeedback f
 */
static ParserFeedback analyseCommand(char *command) {
	int j = 0;
	ParserFeedback ret;
	LinkedList *args;
	LinkedListElem *elem = commands->first;
	Command *currentCmd;

	while (elem != 0) {
		currentCmd = (Command*) elem->data;
		if (strcmp(currentCmd->name, command) == 0) {
			if (!EnumSubsetIsContained(currentCmd->fromModes, getCurrentGameMode())) {
				illegalCmd;
			}

			args = createList();

			for (; j < currentCmd->argNum; j++) { /* collect the command's arguments from the user */
				char *arg = strtok(NULL, delimiter);
				if ((!arg || strlen(arg) == 0) && strcmp(command, cmdNameEdit) != 0) {
					destroyList(args);
					illegalCmd;
				}
				appendElemToList(args, createListElem(arg));
			}

			ret = (*(currentCmd->operate))(args);
			destroyList(args);
			return ret;
		}

		elem = elem->next;
	}

	illegalCmd;
}

ParserFeedback listen() {
	char *command;
	char input[maxInputLength + 2];
	ParserFeedback ret;

	if (fgets(input, maxInputLength + 2, stdin) != NULL) {
		if (strlen(input) == maxInputLength + 1 && input[maxInputLength] != '\n') {
			illegalCmd;
		}
		command = strtok(input, delimiter);

		if (command == 0) {
			ret.isFlag = 0;
			ret.value = getCurrentGameMode();
			return ret;
		}

		return analyseCommand(command);
	} else {
		/*EOF was encountered*/
		return exitOp(NULL);
	}

	assert(0);
	return ret;
}

Command *createCommand(char *name, int argNum, EnumSubset *fromModes, ParserFeedback(*operate)(LinkedList *args)) {
	Command *memAlloc(res, Command);

	res->name = copyString(name);
	res->argNum = argNum;
	res->fromModes = cloneEnumSubset(fromModes);
	res->operate = operate;

	return res;
}

void destroyCommand(Command *command) {
	destroyEnumSubset(command->fromModes);
	memFree(command->name);
	memFree(command);
}

#undef illegalCmd
#undef delimiter