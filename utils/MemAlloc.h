#ifndef __UTILS_MEMALLOC_H
#define __UTILS_FATALERROR_H
/**
 * This header file defines reusable utility macros that deal with
 * dynamically allocated memory. All allocation in the program should be
 * done using these macros.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../Strings.h"

#define fatalError(failedOper) printf("%s\n", errMsgFatal(failedOper)); exit(-1)

/**
 * This macro should be used in order to allocate a variable named [varName] of type [type].
 * There is no initialization of the allocated bytes.
 * Usage: [type] *memAlloc([varName], [type]);
 */
#define memAlloc(varName, type) varName = (type*) malloc(sizeof(type)); if (!varName) { fatalError("malloc"); }

/**
 * This macro should be used in order to allocate [n] elements of type [type] for a variable named [varName].
 * Usage: [type] *memAllocN([varName], [type], [n]);
 */
#define memAllocN(varName, type, n) varName = (type*) calloc(n, sizeof(type)); if (!varName) { fatalError("calloc"); }

/**
 * This macro should be used in order to free a dynamically allocated variable named [varName].
 * Preconditions: [varName] != 0
 * Usage: freeMem([varName]);
 */
#define memFree(varName) assert(varName != 0); free(varName)

#endif