#include "EnumSubset.h"
#include <stdlib.h>
#include <stdio.h>
#include "MemAlloc.h"

void EnumSubsetTurnOn(EnumSubset *subset, unsigned int i) {
	unsigned long mask = 1 << i;
	subset->flags = subset->flags | mask;
}

void EnumSubsetTurnOff(EnumSubset *subset, unsigned int i) {
	unsigned long mask = 1 << i;
	mask = ~mask;
	subset->flags = subset->flags & mask;
}

Bool EnumSubsetIsContained(EnumSubset *subset, unsigned int i) {
	unsigned long mask = 1 << i;
	return (subset->flags & mask) != 0;
}

EnumSubset *createEnumSubset() {
	EnumSubset *memAlloc(ret, EnumSubset);
	ret->flags = 0;
	return ret;
}

EnumSubset *cloneEnumSubset(const EnumSubset *subset) {
	EnumSubset *ret = createEnumSubset();
	ret->flags = subset->flags;
	return ret;
}

void destroyEnumSubset(EnumSubset *subset) {
	memFree(subset);
}