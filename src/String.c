#define _DEFAULT_SOURCE

#include "String.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

int String_create(String *string, char const *str)
{
	if (!(string->str = strdup(str))) {
		perror("String_create");
		return 0;
	}
	string->length = strlen(string->str);
	string->size = string->length;
	return 1;
}

int String_initialize(String *string, int size) 
{
	string->str = NULL;
	string->length = 0;
	string->size = 0;
	if (!String_extend(string, size + 1)) {
		return 0;
	}
	string->str[0] = '\0';
	return 1;
}

int String_createFormattedText(String *dest, char const *str, int maxCharPerLine) {
	int lastSepIndex, currentIndex, nbCharOnLine;
	char c;

	if (!String_create(dest, str)) {
		return 0;
	}

	for (lastSepIndex = 0, currentIndex = 0, nbCharOnLine = 0; currentIndex < dest->length; currentIndex ++, nbCharOnLine ++) {
		c = dest->str[currentIndex];
		if (c == ' ' || c == '\t') {
			lastSepIndex = currentIndex;
		}

		if (nbCharOnLine >= maxCharPerLine) {
			dest->str[lastSepIndex] = '\n';
			nbCharOnLine = currentIndex - lastSepIndex - 1;
		}
	}

	return 1;
}

void String_empty(String *string) {
	string->length = 0;
}

int String_extend(String *string, int minSize) 
{
	int newSize =  string->size * 2 > minSize ? string->size * 2 : minSize;
	char *buffer = (char*)realloc(string->str, sizeof(char) * newSize);
	if (!buffer) {
		free(string->str);
		return 0;
	}
	string->str = buffer;
	string->size = newSize;
	return 1;
}

int String_addCharacter(String *string, char c) {
	if (string->length + 1 >= string->size) {
		if (!String_extend(string, 10)) {
			return 0;
		}
	}
	string->str[string->length] = c;
	string->str[string->length + 1] = '\0';
	string->length ++;
	return 1;
}

void String_free(String *string)
{
	if (string->str)
	{
		free(string->str);
	}
	string->size = 0;
}

int String_append(String* result, char const *str1, ...)
{
	va_list ap;
	int length;
	char const* arg = NULL;
	if (!result->str) {
		if (!String_initialize(result, 50)) {
			return 0;
		}
	}
	
	va_start(ap, str1);
	for (arg = str1; arg != NULL; arg = va_arg(ap, char const*)) {
		length = strlen(arg);
		if (!String_extend(result, result->length + length + 1)) {
			return 0;
		}
		strcat(result->str, arg);
		result->length += length;
	}
	va_end(ap);

	return 1;
}

int String_insertAtIndex(String* string, int index, char c)
{
	int i;
	if (index > string->length) {
		return 0;
	}

	if (!String_extend(string, string->length + 2)) {
		return 0;
	}
	
	for (i = index; i < string->length + 2 ; i++) {
		string->str[i + 1] = string->str[i];
	}
	string->str[index] = c;
	return 1;
}

int String_compare(String const *str1, String const *str2)
{
	return strcmp(str1->str, str2->str);
}

int String_parseInt(String const* string, int *value)
{
	char *endStr;
	*value = strtol(string->str, &endStr, 10);
	if (endStr == string->str)
	{
		fprintf(stderr, "String_parseInt() : No digits were found\n");
		return 0;
	}

	return 1;
}

int String_putInteger(String *result, int value)
{
	if (!String_initialize(result, log10(value) + 2)) {
		return 0;
	}
	sprintf(result->str, "%d" , value);
	result->length = log10(value) + 1; /* Nb of digits in a number */
	return 1;
}