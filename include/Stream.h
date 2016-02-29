#ifndef STREAM_H
#define STREAM_H

#include "String.h"
#include <stdio.h>


/**
 * \brief Gets in line the next line from stream (the end of line character is not added). If there is no more line, then isEOF is set to 1.
 * \return 0 if allocation failed, 1 otherwise.
 */
int Stream_getLine(FILE *stream, String *line, int *isEOF);

/**
 * \brief Gets in value the next integer from stream. First spacing characters are skipped.
 * \return 0 if non digital characters are encountered, 1 if the value has been correctly read.
 */
int Stream_getInteger(FILE *stream, int *value);

/**
 * \brief Write a line to the stream.
 */
void Stream_writeLine(FILE *stream, char const *line);

/**
 * \brief Write an integer to the stream.
 * \return 0 if the writing failed, 1 otherwise.
 */
int Stream_writeInteger(FILE *stream, int value);

#endif