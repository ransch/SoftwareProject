#ifndef __SHARED_H
#define __SHARED_H
/**
 * This module defines the SharedBundle struct.
 */

#include "dataStructures/Puzzle.h"
#include "utils/Boolean.h"
#include "dataStructures/Activity.h"

/**
 * This struct bundles the variables that should be shared by
 * the different components of the program during its lifetime.
 */
typedef struct {
	Puzzle *puzzle;
	Bool markErrorsFlag;
	Activity *activity;
} SharedBundle;

/**
 * This method initializes the extern "bundle" variable.
 */
void initBundle();

/**
 * This method destroys the extern bundle variable,
 * but doesn't affect the markErrorsFlag variable.
 */
void destroyBundle();

#endif