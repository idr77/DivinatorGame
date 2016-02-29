#include "Population.h"
#include "Character.h"
#include "Stream.h"

#include <stdlib.h>

/** 
 * \brief Characters are stored in a Population into a linked list in order to ease deletions.
 */
typedef struct _CharacterCell {

	/** 
	 * \brief The character this cell is bound to.
	 */
	Character character;

	/**
	 * \brief A mark stating how much this character is likely to be the one we are searching for.
	 */
	int mark;

	/** 
	 * \brief Next cell.
	 */
	struct _CharacterCell *next;

} Cell;

/** 
 * \brief Initializes the population.
 */
static void Population_initialize(Population *population);

/**
 * \brief Fills population using the content of stream.
 * \return 0 if allocation failed or stream was not correctly formatted, 1 otherwise.
 */
static int Population_readCharactersFromStream(Population *population, FILE *stream, QuestionArray *questionArray);

/**
 * \brief Adds a Cell in the population.
 */
static void Population_addCell(Population *population, Cell *cell);

/**
 * \brief Extracts the Cell following prev from population (careful, the topCharacter must be not updated manually).
 */
static Cell* Population_extractCell(Population *population, Cell *prev);

/** 
 * \brief Creates a new Cell.
 */
static int Cell_create(Cell **cell);

/**
 * \brief Frees a Cell and all its followers recursively.
 */
static void Cell_free(Cell *cell);

/**
 * \brief Returns the amount that should be added to the current mark of the character depending on the given answer.
 */
static int getMarkModifier(int answer, int expected);



int Population_createFromStream(Population *population, FILE *stream, QuestionArray *questionArray) {
	Population_initialize(population);
	if (!Population_readCharactersFromStream(population, stream, questionArray)) {
		Population_free(population);
		return 0;
	}
	return 1;
}

void Population_updateMarks(Population *population, int questionNumber, int expectedAnswer, QuestionArray *questionArray) {
	int bestMark = ELIMINATINGMARK - 1;
	int gap = 0;
	Cell *cell, *prev, *top;
	for (prev = NULL, cell = population->head; cell != NULL; ) {
		cell->mark += getMarkModifier(cell->character.answers[questionNumber], expectedAnswer);

		if (cell->mark < ELIMINATINGMARK && population->size > 1) {
			cell = Population_extractCell(population, prev);
			Character_removeAnswers(&cell->character, questionArray);
			Cell_free(cell);
			cell = (prev == NULL) ? population->head : prev->next;
		} 

		else {
			if (cell->mark > bestMark) {
				top = cell;
				gap = cell->mark - bestMark;
				bestMark = cell->mark;
			} else {
				int newGap = bestMark - cell->mark;
				gap = gap < newGap ? gap : newGap;
			}
			prev = cell;
			cell = cell->next;
		}
	}

	population->topCharacter = top;
	population->found = gap > GAPMAX ? 1 : 0;
}

Character const* Population_getBestCharacter(Population *population) {
	return &population->topCharacter->character;
}

void Population_free(Population *population) {
	Cell_free(population->head);
}

static void Population_initialize(Population *population) {
	population->head = NULL;
	population->topCharacter = NULL;
	population->size = 0;
	population->found = 0;
}

static int Population_readCharactersFromStream(Population *population, FILE *stream, QuestionArray *questionArray) {
	String name;
	if (!String_initialize(&name, 20)) {
		return 0;
	}

	int isEOF = 0;
	Cell *cell;
	while (!isEOF) {
		if (!Stream_getLine(stream, &name, &isEOF)) {
			String_free(&name);
			return 0;
		}

		if (!isEOF) {
			if (!Cell_create(&cell)) {
				String_free(&name);
				return 0;
			}

			if (!Character_create(&cell->character, name.str, questionArray->nbQuestions)) {
				String_free(&name);
				free(cell);
				return 0;
			} 

			if (!Character_readAnswersFromStream(&cell->character, stream, questionArray->nbQuestions)) {
				String_free(&name);
				Cell_free(cell);
				return 0;
			}

			Character_addAnswers(&cell->character, questionArray);
			Population_addCell(population, cell);
		}
	}
	
	String_free(&name);
	if (population->size == 0) {
		return 0;
	}
	return 1;
}

static void Population_addCell(Population *population, Cell *cell) {
	cell->next = population->head;
	population->head = cell;
	population->size ++;

	if (!population->topCharacter || population->topCharacter->mark < cell->mark) {
		population->topCharacter = cell;
	}
}

static Cell* Population_extractCell(Population *population, Cell *prev) {
	Cell *cell;
	if (prev) {
		cell = prev->next;
		prev->next = cell->next;
	} else {
		cell = population->head;
		population->head = cell->next;
	}
	cell->next = NULL;
	population->size --;
	return cell;
}

	
static int Cell_create(Cell **cell) {
	*cell = (Cell*)malloc(sizeof(Cell));
	if (!(*cell)) {
		return 0;
	}
	(*cell)->mark = 0;
	(*cell)->next = NULL;
	return 1;
}

static void Cell_free(Cell *cell) {
	if (cell) {
		Cell_free(cell->next);
		Character_free(&cell->character);
		free(cell);
	}
}

static int getMarkModifier(int answer, int expected) {
	if (answer == 0) {
		return 0;
	} else if (answer == expected) {
		return 3;
	} else if (answer == expected + 1 || answer == expected - 1) {
		return 1;
	} else if (answer == expected + 2 || answer == expected - 2) {
		return -1;
	} else if (answer == expected + 3 || answer == expected - 3) {
		return -2;
	} else {
		return -3;
	}
}

