#ifndef __STRINGS_H
#define __STRINGS_H
/**
 * This header file defines all the strings in the program.
 */

#include <stdio.h>

#define cmdNameSolve "solve"
#define cmdNameEdit "edit"
#define cmdNameMarkErrors "mark_errors"
#define cmdNamePrintBoard "print_board"
#define cmdNameSet "set"
#define cmdNameValidate "validate"
#define cmdNameGenerate "generate"
#define cmdNameUndo "undo"
#define cmdNameRedo "redo"
#define cmdNameSave "save"
#define cmdNameHint "hint"
#define cmdNameNumSolutions "num_solutions"
#define cmdNameAutofill "autofill"
#define cmdNameReset "reset"
#define cmdNameExit "exit"

#define infoMsgBeginning "Sudoku\n------"
#define infoMsgListening "Enter your command:"
#define infoMsgSavedToFS "Saved to: %s\n"
#define infoMsgExiting "Exiting..."
#define infoMsgHint "Hint: set cell to %d\n"
#define infoMsgValidPuzzle "Validation passed: board is solvable"
#define infoMsgInvalidPuzzle "Validation failed: board is unsolvable"
#define infoMsgCellSet "Cell <%d,%d> set to %d\n"
#define infoMsgSolutionsNum "Number of solutions: %d\n"
#define infoMsgSingleSolution "This is a good board!"
#define infoMsgMultipleSolutions "The puzzle has more than 1 solution, try to edit it further"
#define infoMsgBoardReset "Board reset"
#define infoMsgErrSolution "Puzzle solution erroneous"
#define infoMsgSuccessfulSolution "Puzzle solved successfully"

#define errMsgFatal(failedOper) "Error: "failedOper" has failed"
#define errMsgInvalidCommand "ERROR: invalid command"
#define errMsgNotInRange "Error: value not in range 0-%d\n"
#define errMsgFixedCell "Error: cell is fixed"
#define errMsgErroneousValues "Error: board contains erroneous values"
#define errMsgInvalidBoard "Error: board validation failed"
#define errMsgIOCreationModFailed "Error: File cannot be created or modified"
#define errMsgIOExistOpeningFailed "Error: File doesn't exist or cannot be opened"
#define errMsgIOOpeningFailed "Error: File cannot be opened"
#define errMsgNonEmptyCell "Error: cell already contains a value"
#define errMsgUnsolvablePuzzle "Error: board is unsolvable"
#define errMsgNonEmptyBoard "Error: board is not empty"
#define errMsgErroneousMarkErrorsVal "Error: the value should be 0 or 1"
#define errMsgGenFailed "Error: puzzle generator failed"
#define errMsgCannotRedo "Error: no moves to redo"
#define errMsgCannotUndo "Error: no moves to undo"
#define errMsgGurobi "Gurobi Error: %s\n"
#define errMsgGurobiUnexpected "Gurobi Error: Optimization was stopped early"

#define printInfoMsgUndo(x, y, prev, curr) printf("Undo %d,%d: from ",x,y); if (prev){ printf("%d",prev);}else{printf("_");} printf(" to "); if (curr){ printf("%d",curr);}else{printf("_");}printf("\n")
#define printInfoMsgRedo(x, y, prev,curr) printf("Redo %d,%d: from ",x,y); if (prev){ printf("%d",prev);}else{printf("_");} printf(" to "); if (curr){ printf("%d",curr);}else{printf("_");} printf("\n")

#endif
