#define _DEFAULT_SOURCE

#include "Character.h"
#include "Stream.h"

#include <stdlib.h>
#include <string.h>


int Character_create(Character *character, char const *name, int nbQuestions) {
	character->name = strdup(name);
	if (!character->name) {
		return 0;
	}

	character->answers = (int*)malloc(sizeof(int) * nbQuestions);
	if (!character->answers) {
		free(character->name);
		return 0;
	}

	return 1;
}

int Character_readAnswersFromStream(Character *character, FILE *stream, int nbQuestions) {
	int value, nbAnswers;
	for (nbAnswers = 0; Stream_getInteger(stream, &value) && nbAnswers <= nbQuestions; nbAnswers ++) {
		if (value == -1) {
			return nbAnswers == nbQuestions ? 1 : 0;
		}

		if (value < 0 || value > 5) {
			return 0;
		}

		character->answers[nbAnswers] = value;
	}
	return 0;
}

void Character_addAnswers(Character const *character, QuestionArray *questionArray) {
	int i;
	for (i = 0; i < questionArray->nbQuestions; i ++) {
		QuestionArray_addRepresentantToQuestion(questionArray, i, character->answers[i]);
	}
}

int Character_getAnswer(Character const *character, int question) {
	return character->answers[question];
}

void Character_removeAnswers(Character const *character, QuestionArray *questionArray) {
	int i;
	for (i = 0; i < questionArray->nbQuestions; i ++) {
		QuestionArray_removeRepresentantToQuestion(questionArray, i, character->answers[i]);
	}
}

void Character_free(Character *character) {
	free(character->name);
	free(character->answers);
}