#ifndef _DB_MANAGER_H
#define _DB_MANAGER_H

#include "Question.h"
#include "Character.h"
#include <stdio.h>

#define PATH_QUESTIONS "data/questions.txt"
#define PATH_POPULATION "data/population.txt"

/**
 * \brief Adds a question into the database.
 * \return 0 if the adding failed, 1 otherwise.
 */
int addQuestionInDB(char const *question);

/**
 * \brief Adds a character into the database.
 * \return 0 if the adding failed, 1 otherwise.
 */
int addCharacterInDB(char const *character, QuestionArray const *questionArray, int lastAnswer);

/**
 * \brief Updates the character's answers that are not referenced already in the database.
 * \return 0 if the updating failed, 1 otherwise.
 */
int updateCharacterAnswersInDB(Character const *character, QuestionArray const *questionArray);

#endif