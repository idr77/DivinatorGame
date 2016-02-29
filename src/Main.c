#include "Population.h"
#include "Question.h"
#include "DBManager.h"
#include "GUI.h"

#include <stdio.h>
#include <stdlib.h>

#define NB_QUESTIONS 20

/**
 * \brief Initializes the Population and the QuestionArray.
 */
static int initializeGame(Population *population, QuestionArray *questionArray);

/**
 * \brief The game loop.
 */
static void playLoop(GUI *gui, Population *population, QuestionArray *questionArray);

/**
 * \brief Displays the result screen.
 */
static void resultScreen(GUI *gui, Population *population, QuestionArray const *questionArray);

/**
 * \brief Displays the screen where the user is able to add his character and a new question.
 */
static void addDataToBaseScreen(GUI *gui, QuestionArray const *questionArray);

/**
 * \brief Frees the Population and the QuestionArray.
 */
static void freeGame(Population *population, QuestionArray *questionArray);



int main(int argc, char* argv[])
{
	GUI *gui;
	Population population;
	QuestionArray questionArray;
	int play = 1, nextGame;

	if (!(gui = GUI_create())) {
		fprintf(stderr, "Error loading GUI.\n");
		return EXIT_FAILURE;
	}

	while (play && !GUI_isClosed(gui)) {
		GUI_displayMenu(gui);
		nextGame = 0;

		while (!nextGame && !GUI_isClosed(gui)) {
			if (GUI_getMouseClick()) {
				if (GUI_isButtonClicked(BUT_PLAY)) {
					if (initializeGame(&population, &questionArray)) {
						playLoop(gui, &population, &questionArray);
						freeGame(&population, &questionArray);
						nextGame = 1;
					} else {
						GUI_close(gui);
					}
				} 

				else if (GUI_isButtonClicked(BUT_QUIT)) {
					GUI_close(gui);
				}
			}
		}
	}
	
	GUI_free(gui);

	return EXIT_SUCCESS;
}

static int initializeGame(Population *population, QuestionArray *questionArray) {
	FILE *questionsFile, *populationFile;
	
	if ((questionsFile = fopen(PATH_QUESTIONS, "r"))) {
		if (QuestionArray_createFromStream(questionArray, questionsFile)) {
			fclose(questionsFile);
			if ((populationFile = fopen(PATH_POPULATION, "r"))) {
				if (Population_createFromStream(population, populationFile, questionArray)) {
					fclose(populationFile);
					return 1;
				} else {
					fprintf(stderr, "Could not initialize Population.\n");
					fclose(populationFile);
				}
			} else {
				fprintf(stderr, "Error while opening %s.\n", PATH_POPULATION);
				QuestionArray_free(questionArray);
			}
		} else {
			fprintf(stderr, "Could not initialize QuestionArray.\n");
			fclose(questionsFile);
		}		
	} else {
		fprintf(stderr, "Error while opening %s.\n", PATH_QUESTIONS);
	}
	return 0;
}

static void playLoop(GUI *gui, Population *population, QuestionArray *questionArray) {
	int i, nextQuestion;
	Question const *question;
	int answer;

	for (i = 0; i < NB_QUESTIONS; i ++) {
		if (population->found) {
			question = QuestionArray_getUnknownQuestion(questionArray, Population_getBestCharacter(population));
		} else {
			question = QuestionArray_getBestQuestion(questionArray);
		}
		GUI_displayQuestion(gui, question->str);
		nextQuestion = 0;

		while (!nextQuestion) {
			if (GUI_isClosed(gui)) {
				return;
			}

			if (GUI_getMouseClick() && GUI_getAnswer(&answer)) {
				QuestionArray_setAnswer(questionArray, question->number, answer);
				Population_updateMarks(population, question->number, answer, questionArray);
				nextQuestion = 1;
			}
		}
	}	

	resultScreen(gui, population, questionArray);
}

static void resultScreen(GUI *gui, Population *population, QuestionArray const *questionArray) {
	String result;
	Character const *bestCharacter = Population_getBestCharacter(population);
	if (String_initialize(&result, 100)) {
		if (String_append(&result, "Votre personnage est-t-il ", bestCharacter->name, " ?", NULL)) {
			GUI_displayResult(gui, result.str);
			String_free(&result);

			while (1) {
				if (GUI_isClosed(gui)) {
					return;
				}

				if (GUI_getMouseClick()) {
					if (GUI_isButtonClicked(BUT_YES)) {
						updateCharacterAnswersInDB(bestCharacter, questionArray);
						return;
					} else if (GUI_isButtonClicked(BUT_NO)) {
						addDataToBaseScreen(gui, questionArray);
						return;
					}
				}
			}
		} else {
			String_free(&result);
		}
	}
	fprintf(stderr, "Error: the result of the program cannot be displayed.\n");	
}

static void addDataToBaseScreen(GUI *gui, QuestionArray const *questionArray) {
	char *question = NULL;
	char *characterName = NULL;
	int answer;

	GUI_displayAddQuestionWindow(gui, &question, &characterName, &answer);

	if (!GUI_isClosed(gui)) {
		addQuestionInDB(question);
		free(question);
		addCharacterInDB(characterName, questionArray, answer);
		free(characterName);
	}
}

static void freeGame(Population *population, QuestionArray *questionArray) {
	Population_free(population);
	QuestionArray_free(questionArray);
}