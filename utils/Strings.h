#ifndef __UTILS_STRINGS_H
#define __UTILS_STRINGS_H

#include "Boolean.h"

/**
 * This module contains reusable utility functions that deal with strings.
 */

/**
 * This method copies a string into a new dynamic-allocated variable.
 *
 * Parameters:
 * const char *str - The string that should be copied
 *
 * Preconditions:
 * str != NULL
 *
 * Returns:
 * A pointer to the copy of str.
 */
char *copyString(const char *str);

/**
 * This method receives a string and checks whether it represents a (decimal) unsigned integer.
 * If it does, it returnes that integer, otherwise the returned value is -1.
 *
 * Parameters:
 * const char *str
 *
 * Preconditions:
 * str != NULL
 *
 * Returns:
 * A pointer to the copy of str.
 */
signed int isUInteger(const char *str);

#endif
