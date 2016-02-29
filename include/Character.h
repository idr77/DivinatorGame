#ifndef CHARACTER_H
#define CHARACTER_H

#include "Question.h"
#include <stdio.h>


typedef struct _Character {

	/**
	 * \brief Name of the character.
	 */
	char *name;

	/**
	 * \brief The marks attributed to each questions.
	 */
	int *answers;

} Character;

/**
 * \brief Creates the character.
 * \return 0 if the allocation failed, 1 otherwise.
 */
int Character_create(Character *character, char const *name, int nbQuestions);

/**
 * \brief Fills the answer array by reading a series of integer from an opened stream.
 * \return 0 if the format of the file is not correct, and 1 otherwise.
 */
int Character_readAnswersFromStream(Character *character, FILE *stream, int nbQuestions);

/**
 * \brief Increments in questionArray all the cardinals for the answers this character has.
 */
void Character_addAnswers(Character const *character, QuestionArray *questionArray);

/** 
 * \brief Gets the answer of the question for this character.
 * \return The answer.
 */
int Character_getAnswer(Character const *character, int question);

/**
 * \brief Decrements in questionArray all the cardinals for the answers this character has.
 */
void Character_removeAnswers(Character const *character, QuestionArray *questionArray);

/**
 * \brief Frees the memory allocated for this character.
 */
void Character_free(Character *character);

#endif