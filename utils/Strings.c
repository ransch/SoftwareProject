#define _CRT_SECURE_NO_WARNINGS

#include "Strings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "MemAlloc.h"

char *copyString(const char *str) {
	unsigned int length = strlen(str) + 1;
	char *memAllocN(res, char, length);
	strcpy(res, str);
	res[length - 1] = '\0';
	return res;
}

signed int isUInteger(const char *str) {
	const char *ptr = str;
	char *p;
	unsigned int res = -1;

	while (isdigit(*(ptr++)));
	if (!(*(--ptr))) {
		res = strtoul(str, &p, 10);
	}

	return res;
}