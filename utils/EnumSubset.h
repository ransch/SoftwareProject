#ifndef __UTILS_ENUMSUBSET_H
#define __UTILS_ENUMSUBSET_H
/**
 * This module defines a subset of an Enum's values.
 */

#include "Boolean.h"

/**
 * This struct defines an enum subset.
 * Every subset should be used for one Enum type,
 * and the Enum's values should be the default ones (0, 1, ...)
 * Also, 2 ^ |Enum| must be <= max value of {unsigned long}.
 */
typedef struct {
	unsigned long flags;
} EnumSubset;

/**
 * This method adds a value to a subset.
 *
 * Parameters:
 * EnumSubset *subset
 * unsigned int i - the value that should be inserted to [subset]
 *
 * Preconditions:
 * subset != 0
 *
 * Postconditions:
 * EnumSubsetIsContained(subset, i)
 */
void EnumSubsetTurnOn(EnumSubset *subset, unsigned int i);

/**
 * This method removes a value from a subset.
 *
 * Parameters:
 * EnumSubset *subset
 * unsigned int i - the value that should be removed from [subset]
 *
 * Preconditions:
 * subset != 0
 *
 * Postconditions:
 * !EnumSubsetIsContained(subset, i)
 */
void EnumSubsetTurnOff(EnumSubset *subset, unsigned int i);

/**
 * This method checks if a value is contained in a subset.
 *
 * Parameters:
 * EnumSubset *subset
 * unsigned int i - the value whose membership is checked
 *
 * Preconditions:
 * subset != 0
 *
 * Returns:
 * TRUE iff
 */
Bool EnumSubsetIsContained(EnumSubset *subset, unsigned int i);

/**
 * This method creates a new enum subset.
 *
 * Returns:
 * A pointer to a new empty dynamically allocated subset.
 */
EnumSubset *createEnumSubset();

/**
 * This method clones a subset.
 *
 * Preconditions:
 * subset != 0
 *
 * Returns:
 * A pointer to a new dynamically allocated subset that is a clone
 * of the argument
 */
EnumSubset *cloneEnumSubset(const EnumSubset *subset);

/**
 * This method destroys an enum subset.
 *
 * Preconditions:
 * subset != 0
 */
void destroyEnumSubset(EnumSubset *subset);

#endif
