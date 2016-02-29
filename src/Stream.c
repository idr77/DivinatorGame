#include "Stream.h"


/**
 * \brief Returns 1 if c equals '\n', '\r' or EOF.
 */
static int isLineEndingCharacter(char c);

/**
 * \brief Returns 1 if c is a tab or a space.
 */
static int isSpacingCharacter(char c);

/**
 * \brief Returns 1 if c is a figure.
 */
static int isDigitCharacter(char c);



int Stream_getLine(FILE *stream, String *line, int *isEOF) {
	char c;
	if ((c = fgetc(stream)) == EOF) {
		*isEOF = 1;
		return 1;
	}

	for (*isEOF = 0, String_empty(line); !isLineEndingCharacter(c); c = fgetc(stream)) {
		if (!String_addCharacter(line, c)) {
			return 0;
		}
	}
	return 1;
}

int Stream_getInteger(FILE *stream, int *value) {
	char c;
	do {
		c = fgetc(stream);
		if (c == EOF) {
			return 0;
		}
	} while (isLineEndingCharacter(c) || isSpacingCharacter(c));

	int isNegative = 0;
	if (c == '-') {
		isNegative = 1;
		if (!isDigitCharacter((c = fgetc(stream)))) {
			return 0;
		}
	}

	for (*value = 0; isDigitCharacter(c); c = fgetc(stream)) {
		(*value) *= 10;
		(*value) += (c - '0');
	}

	if (!(isLineEndingCharacter(c) || isSpacingCharacter(c))) {
		return 0;
	}

	if (isNegative) {
		(*value) *= -1;
	}
	return 1;
}

void Stream_writeLine(FILE *stream, char const *line) {
	int i;
	for (i = 0 ; line[i] != '\0' ; i ++) {
		fputc(line[i], stream);
	}
	fputc('\n', stream);
}

int Stream_writeInteger(FILE *stream, int value) {
	String integer;
	int i;

	if (!String_putInteger(&integer, value)) {
		return 0;
	}

	for (i = 0 ; integer.str[i] != '\0' ; i++) {
		fputc(integer.str[i], stream);
	}
	return 1;
}

static int isLineEndingCharacter(char c) {
	return c == '\r' || c == '\n' || c == EOF;
}

static int isSpacingCharacter(char c) {
	return c == ' ' || c == '\t';
}

static int isDigitCharacter(char c) {
	return c >= '0' && c <= '9';
}