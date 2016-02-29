#define _DEFAULT_SOURCE

#include "Question.h"
#include "Stream.h"
#include "Character.h"

#include <stdlib.h>
#include <string.h>

#define MIN_ALLOCSIZE 100


/**
 * \brief Initializes questionArray to an empty buffer able to contain at least MIN_ALLOCSIZE questions.
 * \return 0 is allocation failed, 1 otherwise.
 */
static int QuestionArray_initialize(QuestionArray *questionArray);

/**
 * \brief If the buffer is full, multiplies its size by 2.
 * \return 0 if allocation failed, 1 otherwise.
 */
static int QuestionArray_extendIfFull(QuestionArray *questionArray);

/**
 * \brief Creates a question.
 * \return 0 if allocation failed, 1 otherwise.
 */
static int Question_create(Question *question, char const *string, int number);

/**
 * \brief Releases the memory used by question.
 */
static void Question_free(Question *question);

/**
 * \brief Returns an evaluation for this question (the higher, the better).
 */
static int Question_getMark(Question const *question);



int QuestionArray_createFromStream(QuestionArray *questionArray, FILE *stream) {
	if (!QuestionArray_initialize(questionArray)) {
		return 0;
	}

	String question;
	if (!String_initialize(&question, MIN_ALLOCSIZE)) {
		QuestionArray_free(questionArray);
		return 0;
	}

	int isEOF = 0;
	while (!isEOF) {
		if (!Stream_getLine(stream, &question, &isEOF)) {
			String_free(&question);
			QuestionArray_free(questionArray);
			return 0;
		}

		if (!isEOF) {
			if (!QuestionArray_extendIfFull(questionArray) || !Question_create(&questionArray->questions[questionArray->nbQuestions], question.str, questionArray->nbQuestions)) {
				String_free(&question);
				QuestionArray_free(questionArray);
				return 0;
			}
			questionArray->nbQuestions ++;
		}
	}
	
	String_free(&question);
	if (questionArray->nbQuestions == 0) {
		QuestionArray_free(questionArray);
		return 0;
	}
	return 1;
}

void QuestionArray_addRepresentantToQuestion(QuestionArray *questionArray, int questionNumber, int value) {
	if (value) {
		questionArray->questions[questionNumber].cardinals[value - 1] ++;
	}
}

void QuestionArray_setAnswer(QuestionArray *questionArray, int questionNumber, int value) {
	questionArray->questions[questionNumber].answer = value;
}

int QuestionArray_getAnswer(QuestionArray const *questionArray, int questionNumber) {
	return questionArray->questions[questionNumber].answer;
}

void QuestionArray_removeRepresentantToQuestion(QuestionArray *questionArray, int questionNumber, int value) {
	if (value) {
		questionArray->questions[questionNumber].cardinals[value - 1] --;
	}
}

Question const* QuestionArray_getBestQuestion(QuestionArray *questionArray) {
	int bestMark, bestQuestion, current, mark;
	for (bestMark = 0, bestQuestion = -1, current = 0; current < questionArray->nbQuestions; current ++) {
		mark = Question_getMark(&questionArray->questions[current]);
		if (mark > bestMark && !questionArray->questions[current].alreadyAsked) {
			bestMark = mark;
			bestQuestion = current;
		}
	}

	if (bestQuestion > -1) {
		questionArray->questions[bestQuestion].alreadyAsked = 1;
		return &questionArray->questions[bestQuestion];
	} else {
		for (current = 0; current < questionArray->nbQuestions; current ++) {
			questionArray->questions[current].alreadyAsked = 0;
		}
		return QuestionArray_getBestQuestion(questionArray);
	}
}

Question const* QuestionArray_getUnknownQuestion(QuestionArray *questionArray, Character const *character) {
	int i;
	for (i = 0; i < questionArray->nbQuestions; i ++) {
		if (!questionArray->questions[i].alreadyAsked && Character_getAnswer(character, i) == 0) {
			questionArray->questions[i].alreadyAsked = 1;
			return &questionArray->questions[i];
		}
	}
	return QuestionArray_getBestQuestion(questionArray);
}

void QuestionArray_free(QuestionArray *questionArray) {
	int i;
	for (i = 0; i < questionArray->nbQuestions; i ++) {
		Question_free(&questionArray->questions[i]);
	}
	free(questionArray->questions);
}


static int QuestionArray_initialize(QuestionArray *questionArray) {
	questionArray->questions = (Question*)malloc(sizeof(Question) * MIN_ALLOCSIZE);
	if (!questionArray->questions) {
		return 0;
	}

	questionArray->nbQuestions = 0;
	questionArray->allocSize = MIN_ALLOCSIZE;
	return 1;
}

static int QuestionArray_extendIfFull(QuestionArray *questionArray) {
	if (questionArray->nbQuestions < questionArray->allocSize) {
		return 1;
	}

	Question *buffer = (Question*)malloc(sizeof(Question) * questionArray->allocSize * 2);
	if (!buffer) {
		return 0;
	}

	questionArray->questions = buffer;
	questionArray->allocSize *= 2;
	return 1;
}

static int Question_create(Question *question, char const *string, int number) {
	question->str = strdup(string);
	if (!question->str) {
		return 0;
	}

	int i;
	for (i = 0; i < 5; i ++) {
		question->cardinals[i] = 0;
	}
	question->alreadyAsked = 0;
	question->number = number;
	question->answer = 0;
	return 1;
}

static void Question_free(Question *question) {
	free(question->str);
}

static int Question_getMark(Question const *question) {
	int i, mark;
	for (i = 0, mark = 1; i < 5; i ++) {
		mark *= (question->cardinals[i] + 1);
	}
	return mark;
}