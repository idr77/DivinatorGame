#ifndef QUESTION_H
#define QUESTION_H

#include <stdio.h>
typedef struct _Character Character;

/**
 * \brief A representation of a question.
 */
typedef struct _Question {

	/**
	 * \brief The actual question.
	 */
	char *str;

	/**
	 * \brief A boolean stating that this question has already been asked.
	 */
	int alreadyAsked;

	/**
	 * \brief The number of characters for each possible answer.
	 */
	int cardinals[5];

	/**
	 * \brief The number of this question.
	 */
	int number;

	/**
	 * \brief The given answer to this question.
	 */
	int answer;

} Question;

/**
 * \brief The list of questions.
 */
typedef struct _QuestionArray {

	/**
	 * \brief The buffer containing all the possible questions.
	 */
	Question *questions;

	/**
	 * \brief The total number of questions.
	 */
	int nbQuestions;

	/**
	 * \brief The real size of the buffer.
	 */
	int allocSize;

} QuestionArray;


/**
 * \brief Creates a QuestionArray using the content of stream.
 * \return 0 if allocation failed or stream was not correctly formatted, 1 otherwise.
 */
int QuestionArray_createFromStream(QuestionArray *questionArray, FILE *stream);

/**
 * \brief Upgrades the number of characters that replied 'value' to the given question.
 */
void QuestionArray_addRepresentantToQuestion(QuestionArray *questionArray, int questionNumber, int value);

/**
 * \brief Removes one of the characters that replied 'value' to the given question.
 */
void QuestionArray_removeRepresentantToQuestion(QuestionArray *questionArray, int questionNumber, int value);

/**
 * \brief Sets the answer given by the user to the question.
 */
void QuestionArray_setAnswer(QuestionArray *questionArray, int questionNumber, int value);

/**
 * \brief Gets the answer given by the user to the question.
 */
int QuestionArray_getAnswer(QuestionArray const *questionArray, int questionNumber);

/**
 * \brief Gets the question that potentially eliminates the biggest number characters from the Population.
 * \return The best evaluated question.
 */
Question const* QuestionArray_getBestQuestion(QuestionArray *questionArray);

/**
 * \brief Searches a question that the character has no answer for (in order to learn answers from the character).
 * \return The question found, or the best evaluated question if all the questions have already been asked or if the character already have all the answers.
 */
Question const* QuestionArray_getUnknownQuestion(QuestionArray *questionArray, Character const *character);

/**
 * \brief Releases the memory allocated for questionArray.
 */
void QuestionArray_free(QuestionArray *questionArray);

#endif