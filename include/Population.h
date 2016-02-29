#ifndef POPULATION_H
#define POPULATION_H

#include "Question.h"
#include <stdio.h>

/**
 * \brief If a character if attributed less than this mark, he is eliminated.
 */
#define ELIMINATINGMARK -6

/**
 * \brief If the top character and the second one have marks that are separated from this gap then we can consider the top character is the one searched for.
 */
#define GAPMAX 6


struct _CharacterCell;

/**
 * \brief A Population is a list of Characters.
 */
typedef struct _CharacterList {

	/** 
	 * \brief The head of the list.
	 */
	struct _CharacterCell *head;

	/**
	 * \brief The character that is the most likely to fit the description of the player.
	 */
	struct _CharacterCell *topCharacter;

	/**
	 * \brief The size of the Population.
	 */
	int size;

	/**
	 * \brief States that the top character has a great probability of being the solution.
	 */
	int found;

} Population;


/**
 * \brief Creates a population using the content of stream.
 * \return 0 if allocation failed or stream was not correctly formatted, 1 otherwise.
 */
int Population_createFromStream(Population *population, FILE *stream, QuestionArray *questionArray);

/**
 * \brief Updates the marks of the characters in population.
 */
void Population_updateMarks(Population *population, int questionNumber, int expectedAnswer, QuestionArray *questionArray);

/**
 * \brief Gets the best character of the population.
 * \return The top character of the population.
 */
Character const* Population_getBestCharacter(Population *population);

/**
 * \brief Releases the memory allocated for population.
 */
void Population_free(Population *population);

#endif