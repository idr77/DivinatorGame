#include "DBManager.h"
#include "Stream.h"
#include "String.h"

#define PATH_TMPFILE ".mydevin.tmp"

/**
 * \brief Adds at the end of each answer line a 0.
 * \return 0 if the addition failed, 1 otherwise.
 */
static int addCharacterZeros(void);

/**
 * \brief If the last line of stream does not end with a '\n', then adds it.
 */
static void addEndLineCharacter(FILE *stream);

/**
 * \brief Puts the cursor of stream at the beginning of this character's answers.
 */
static void findCharacterLine(char const *character, FILE *stream);


int addQuestionInDB(char const* question) {
	FILE *streamQuestions;
	if ((streamQuestions = fopen(PATH_QUESTIONS, "a+"))) {
		if (addCharacterZeros()) {
			addEndLineCharacter(streamQuestions);
			Stream_writeLine(streamQuestions, question);
			fclose(streamQuestions);
			return 1;
		} else {
			fclose(streamQuestions);
		}
	}
	fprintf(stderr, "Unable to update questions.txt.\n");
	return 0;
}

int addCharacterInDB(char const *character, QuestionArray const *questionArray, int lastAnswer) {
	int i;
	FILE* stream;
	if ((stream = fopen(PATH_POPULATION, "a+"))) {
		addEndLineCharacter(stream);
		Stream_writeLine(stream, character);
		for (i = 0; i < questionArray->nbQuestions; i ++) {
			fprintf(stream, "%d ", QuestionArray_getAnswer(questionArray, i));
		}
		fprintf(stream, "%d -1\n", lastAnswer);
		fclose(stream);
		return 1;
	}

	fprintf(stderr, "Unable to update character.txt.\n");
	return 0;
}

int updateCharacterAnswersInDB(Character const *character, QuestionArray const *questionArray) {
	int i;
	FILE* stream;
	if ((stream = fopen(PATH_POPULATION, "r+"))) {
		findCharacterLine(character->name, stream);
		for (i = 0; i < questionArray->nbQuestions; i ++) {
			if (character->answers[i] == 0) {
				fprintf(stream, "%d ", QuestionArray_getAnswer(questionArray, i));
			} else {
				fseek(stream, 2, SEEK_CUR);
			}
		}
		fclose(stream);
		return 1;
	}

	fprintf(stderr, "Unable to update character.txt.\n");
	return 0;
}

static int addCharacterZeros(void) {
	int c, v;
	FILE *stream, *tmp;
	if ((stream = fopen(PATH_POPULATION, "r+"))) {
		if ((tmp = fopen(PATH_TMPFILE, "w+"))) {
			while ((c = fgetc(stream)) != -1) {
				fputc(c, tmp);
			}
			rewind(stream);
			rewind(tmp);

			while ((c = fgetc(tmp)) != -1) {
				fputc(c, stream);
				if (c == '\n') {
					while (1) {
						Stream_getInteger(tmp, &v);
						if (v != -1) {
							fprintf(stream, "%d ", v);
						} else {
							fprintf(stream, "0 -1\n");
							break;
						}
					}
				}
			}
			fclose(tmp);
			remove(PATH_TMPFILE);
			fclose(stream);
			return 1;
		}
	} else {
		fclose(stream);
	}
	return 0;	
}

static void findCharacterLine(char const *character, FILE *stream) {
	int c, i;
	int nbLines;

	do {
		for (i = 0, c = fgetc(stream); character[i] != '\0'; i ++, c = fgetc(stream)) {
			if (character[i] != c) {
				break;
			}
		}

		if (c == '\n') {
			if (character[i] == '\0') {
				return;
			} else {
				nbLines = 1;
			}
		} else {
			nbLines = 2;
		}

		for (i = 0; i < nbLines; i ++) {
			while ((c = fgetc(stream)) != '\n') {
				if (c == EOF) {
					return;
				}
			}
		}
	} while (c != EOF);	
}

static void addEndLineCharacter(FILE *stream) {
	fseek(stream, -1, SEEK_END);
	if (fgetc(stream) != '\n') {
		fputc('\n', stream);
	}
}