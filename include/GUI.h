#ifndef GUI_H
#define GUI_H

#include "String.h"

/**
 * \enum ButtonName
 * \brief A enumeration through the different buttons.
 */
typedef enum {

	BUT_ANSWER1, BUT_ANSWER2, BUT_ANSWER3, BUT_ANSWER4, BUT_ANSWER5, BUT_PLAY, BUT_QUIT, BUT_YES, BUT_NO, BUT_INPUTBOXNAME, BUT_INPUTBOXQUEST, BUT_COUNT

} ButtonName;

/**
 * \brief The GUI structure.
 */
typedef struct _GUI GUI;

/**
 * \fn GUI* GUI_create(void)
 * \brief Create the Graphical User Interface.
 * \return NULL if the creation failed, the created GUI otherwise.
 */
GUI* GUI_create(void);

/**
 * \fn void GUI_displayMenu(GUI const *gui)
 * \brief Displays the menu on the main window.
 */
void GUI_displayMenu(GUI const *gui);

/**
 * \fn int GUI_displayQuestion(GUI const* GUI, char const *question)
 * \brief Displays the question on the window.
 * \return 0 if the formatting of the question fails, 1 otherwise.
 */
int GUI_displayQuestion(GUI const *gui, char const *question);

/**
 * \fn void GUI_displayResult(GUI const *GUI, char const *finalQuestion)
 * \brief Display the result screen: the devin asked if the character he found is the one the user was thinking of.
 */
int GUI_displayResult(GUI const *gui, char const *finalQuestion);

/**
 * \fn int GUI_displayAddQuestionWindow(GUI const *gui, char **question, char **character, int *answer)
 * \brief Display the window which enable the user to add a question.
 */
void GUI_displayAddQuestionWindow(GUI const* gui, char **question, char **character, int *answer);

/**
 * \fn int GUI_getMouse(void)
 * \brief Gets the position of the click.
 * \return 1 if there was a click, 0 otherwise.
 */
int GUI_getMouseClick(void);

/**
 * \fn int GUI_isButtonClicked(ButtonName button)
 * \brief Checks whether the last click is one the given button.
 * \return 0 if the click is out of bounds, 1 otherwise.
 */
int GUI_isButtonClicked(ButtonName button);

/**
 * \fn int GUI_getAnswer(int *answer) 
 * \brief Puts in answer the answer given by the user, if there is any.
 * \return 1 the user clicked on an answer button, 0 otherwise.
 */
int GUI_getAnswer(int *answer);

/**
 * \fn int GUI_refresh()
 * \brief Actualises the window.
 */
void GUI_refresh();

/**
 * \fn int GUI_clear(GUI const* gui)
 * \brief Clear the window.
 */
void GUI_clear(GUI const* gui);

/**
 * \fn GUI_isClosed(GUI const *gui)
 * \brief Indicates if the GUI has been closed.
 * \return 1 if closed, 0 otherwise.
 */
int GUI_isClosed(GUI const *gui);

/**
 * \fn int GUI_close(GUI* gui)
 * \brief Closes the gui has been closed.
 */
void GUI_close(GUI *gui);

/**
 * \fn int GUI_free(GUI* gui)
 * \brief Releases memory.
 */
void GUI_free(GUI *gui);

#endif