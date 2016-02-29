#include "GUI.h"
#include "GUI_components.h"
#include <MLV/MLV_all.h>

/**
 * \brief Description of resources' indexes
 */
enum ResourcesIndexes
{
	RSC_BACKGROUND,
	RSC_DEVIN,
	RSC_ANSWERS,
	RSC_QUESTION_BUBBLE,
	RSC_MENU,
	RSC_YES,
	RSC_NO,
	NB_RSC
};

/**
 * \struct GUI
 * \brief  Structure used to represent the GUI.
 */
typedef struct _GUI
{
	/**
	 * \brief Pictures used by the game
	 */
	MLV_Image *resources[NB_RSC];

	/**
	 * \brief Font used by the text
	 */
	MLV_Font *font;

	/**
	 * \brief Boolean stating that the GUI has been closed (by clicking on the cross).
	 */
	int isClosed;

} GUI;

/**
 * \struct Button
 * \brief A clickable button.
 */
typedef struct _Button {

	/**
	 * \brief Coordinates and size of the button.
	 */
	int x, y, w, h;

} Button;

/**
 * \brief The different buttons.
 */
static Button buttons[BUT_COUNT];

/**
 * \struct Input
 * \brief Structure used to manage the mouse
 */
 typedef struct _Input
 {
 	/**
 	 * \brief Coordinates of the mouse clicks
 	 */
 	int clickX, clickY;

 	/**
 	 * \brief State of the button (pressed or released)
 	 */
 	MLV_Button_state stateButton;

 	/**
 	 * \brief Button clicked (left, right, ...)
 	 */
 	MLV_Mouse_button currentButton;

 	/**
 	 * @brief The input box.
 	 */
 	MLV_Input_box *box;

 	/**
 	 * @brief Text entered in the input box.
 	 */
 	char *text;

 	/**
 	 * @brief Event type.
 	 */
 	int type;

 } Input;

/**
 * \brief Input var
 */
static Input input;


/**
 * \brief Releases memory from resources in case of failure loading resources.
 */
static void GUI_releaseResourcesAllocated(GUI *gui);

/**
 * \fn static int GUI_addResource(GUI *gui, int index, char const *rscPath)
 * \brief Loads an image located at rscPath into gui->resources[index].
 * \return 0 if the loading failed, 1 otherwise.
 */
static int GUI_addResource(GUI *gui, int index, char const *rscPath);

/**
 * \fn static int GUI_loadResources(GUI* gui)
 * \brief Loads images' resources needed for the game
 * \return 0 if the loading fails, 1 otherwise
 */
static int GUI_loadResources(GUI* gui);

/**
 * \fn static int GUI_displayGameStructure(GUI const *gui)
 * \brief Displays the avatar of Devin and the question's zone.
 */
static void GUI_displayGameStructure(GUI const *gui);

/**
 * \fn static int GUI_displayBubble(GUI const *GUI, char const *message)
 * \brief Displays a message in the question zone.
 * \return 0 if the allocation of the message failed.
 */
static int GUI_displayBubble(GUI const *GUI, char const *message);

/**
 * \fn static int GUI_displayDevin(GUI const *gui)
 * \brief Displays the avatar of the Devin.
 */
static void GUI_displayDevin(GUI const* gui);

/**
 * \fn static int GUI_displayAnswersButton(GUI const* gui)
 * \brief Displays answers' buttons.
 */
static void GUI_displayAnswersButton(GUI const *gui);

/**
 * \fn static int GUI_displayQuestionBubble(GUI const* gui)
 * \brief Displays a bubble near the Devin.
 */
static void GUI_displayQuestionBubble(GUI const *gui);

/**
 * \fn static void GUI_displayYesNoButtons(GUI const *gui)
 * \brief Display yes button and no button
 */
static void GUI_displayYesNoButtons(GUI const *gui);

/**
 * \fn static void initializeButtons(void)
 * \brief Initializes the buttons.
 */
static void initializeButtons(void);

/**
 * \fn static void Button_initialize(Button *button, int x, int y, int w, int h)
 * \brief Initializes a button with his coodinates and size.
 */
static void Button_initialize(Button *button, int x, int y, int w, int h);

/**
 * @fn static int GUI_getEvent(void)
 * @brief Gets an event in input.
 */
static int GUI_getEvent(void);




GUI* GUI_create(void) {
	GUI *gui = (GUI*)malloc(sizeof(GUI));
	if (!gui) {
		return NULL;
	}
	MLV_execute_at_exit((void(*)(void*))GUI_close, gui);
	MLV_create_window("Devin", "Devin", W_WINDOW, H_WINDOW);
	initializeButtons();
	if (!GUI_loadResources(gui)) {
		MLV_free_window();
		free(gui);
		return NULL;
	}
	gui->isClosed = 0;
	return gui;
}

void GUI_displayMenu(GUI const *gui) {
	MLV_resize_image(gui->resources[RSC_MENU], W_WINDOW, H_WINDOW);
	MLV_draw_image(gui->resources[RSC_MENU], 0, 0);
	GUI_refresh();
}

int GUI_displayQuestion(GUI const* gui, char const *question) {
	if (GUI_displayBubble(gui, question)) {
		GUI_displayAnswersButton(gui);
		GUI_refresh();
		return 1;
	}
	return 0;
}

int GUI_displayResult(GUI const *gui, char const *finalQuestion) {
	if (GUI_displayBubble(gui, finalQuestion)) {
		GUI_displayYesNoButtons(gui);
		GUI_refresh();
		return 1;
	}
	return 0;	
}

static int GUI_displayBubble(GUI const *gui, char const *message) {
	MLV_Color bubble = MLV_rgba(0,0,0,0);
	String mess;
	if (!String_createFormattedText(&mess, message, MAX_CHAR_LINE)) {
		return 0;
	}
	GUI_displayGameStructure(gui);
	MLV_draw_filled_rectangle(X_QUESTEXT, Y_QUESTEXT, W_QUESTEXT, H_QUESTEXT, bubble);
	MLV_draw_text_box_with_font(
		X_QUESTEXT, 
		Y_QUESTEXT,
		W_QUESTEXT,
		H_QUESTEXT,
		mess.str,
		gui->font,
		1,
		MLV_rgba(255, 240, 190, 0),
		MLV_COLOR_BLACK,
		bubble,
		MLV_TEXT_CENTER,
		MLV_HORIZONTAL_CENTER,
		MLV_VERTICAL_CENTER
		);
	String_free(&mess);
	return 1;
}

void GUI_displayAddQuestionWindow(GUI const* gui, char **question, char **character, int *answer) {
	MLV_Input_box *questionBox = NULL, *characterBox = NULL;
	char *questionStr = NULL, *characterName = NULL;
	MLV_Color inputBoxBorderColor = MLV_COLOR_WHITE, inputBoxBckgrdColor = MLV_COLOR_WHITE;

	/* Clear GUI */
	GUI_clear(gui);

	/* Draw Informations */
	MLV_draw_text(X_TEXTBOXQUEST, Y_TEXTBOXNAME - 60, "Je n'ai pas pu trouver votre personnage. Veuillez entrer son nom, ainsi qu'une question qui m'aurait aider à le découvrir !", MLV_COLOR_BROWN);
	MLV_draw_text(X_TEXTBOXQUEST, Y_TEXTBOXNAME - 40, "Appuyez sur entrée lorsque vous avez entré votre texte dans les boites ci-dessous.", MLV_COLOR_BROWN);
	
	/* Input box for question */
	questionBox = MLV_create_input_box_with_font(
		X_TEXTBOXQUEST,
		Y_TEXTBOXQUEST,
		W_TEXTBOXQUEST,
		H_TEXTBOXQUEST,
		inputBoxBorderColor,
		MLV_COLOR_BLACK,
		inputBoxBckgrdColor,
		"Question : ",
		gui->font
	);

	/* Input box for name */
	characterBox = MLV_create_input_box_with_font(
		X_TEXTBOXNAME,
		Y_TEXTBOXNAME,
		W_TEXTBOXNAME,
		H_TEXTBOXNAME,
		inputBoxBorderColor,
		MLV_COLOR_BLACK,
		inputBoxBckgrdColor,
		"Personnage : ",
		gui->font
	);

	GUI_displayAnswersButton(gui);

	while (!GUI_isClosed(gui)) {
		if (!characterName) {
			MLV_draw_input_box(characterBox);
		}
		if (!questionStr) {
			MLV_draw_input_box(questionBox);
		}
		GUI_refresh();

		if (GUI_getEvent()) {
			if (input.type == MLV_MOUSE_BUTTON) {
				if (GUI_isButtonClicked(BUT_INPUTBOXQUEST)) {
					free(questionStr);
					questionStr = NULL;
					MLV_activate_input_box(questionBox);
				} else if (GUI_isButtonClicked(BUT_INPUTBOXNAME)) {
					free(characterName);
					characterName = NULL;
					MLV_activate_input_box(characterBox);
				} else {
					MLV_desactivate_input_box();
				}

				if (GUI_getAnswer(answer) && questionStr != NULL && questionStr[0] != '\0' && characterName != NULL && characterName[0] != '\0') {
					MLV_free_input_box(characterBox);
					MLV_free_input_box(questionBox);
					*question = questionStr;
					*character = characterName;
					return;
				}
			} else if (input.type == MLV_INPUT_BOX) {
				if (input.box == characterBox) {
					characterName = input.text;
				} else {
					questionStr = input.text;
				}
			}
		}		
	}

	MLV_free_input_box(characterBox);
	MLV_free_input_box(questionBox);
	free(questionStr);
	free(characterName);
}

int GUI_getMouseClick(void) {	
	MLV_Mouse_button mb;
	MLV_Button_state bs;
	if (MLV_get_event(NULL, NULL, NULL, NULL, NULL, &input.clickX, &input.clickY, &mb, &bs) == MLV_MOUSE_BUTTON) {
		return mb == MLV_BUTTON_LEFT && bs == MLV_RELEASED;
	}
	return 0;
}

int GUI_getAnswer(int *answer) {
	if (GUI_isButtonClicked(BUT_ANSWER1)) {
		*answer = 1;
		return 1;
	} else if (GUI_isButtonClicked(BUT_ANSWER2)) {
		*answer = 2;
		return 1;
	} else if (GUI_isButtonClicked(BUT_ANSWER3)) {
		*answer = 3;
		return 1;
	} else if (GUI_isButtonClicked(BUT_ANSWER4)) {
		*answer = 4;
		return 1;
	} else if (GUI_isButtonClicked(BUT_ANSWER5)) {
		*answer = 5;
		return 1;
	}
	return 0;
}


int GUI_isButtonClicked(ButtonName button) {
	return (buttons[button].x <= input.clickX && input.clickX <= buttons[button].x + buttons[button].w) 
		&& (buttons[button].y <= input.clickY && input.clickY <= buttons[button].y + buttons[button].h);
}

int GUI_isClosed(GUI const *gui) {
	return gui->isClosed;
}

void GUI_close(GUI *gui) {
	gui->isClosed = 1;
}

void GUI_free(GUI *gui) {
	GUI_releaseResourcesAllocated(gui);
	if (gui->font) {
		MLV_free_font(gui->font);
	}
	MLV_free_window();	
	free(gui);
}

void GUI_refresh(void) {
	MLV_actualise_window();
}

void GUI_clear(GUI const* gui) {
	MLV_draw_image(gui->resources[RSC_BACKGROUND], 0, 0);
	GUI_refresh();
}

static void GUI_releaseResourcesAllocated(GUI *gui) {
	int i;
	for (i = 0; i < NB_RSC ; i++) {
		if (gui->resources[i]) {
			MLV_free_image(gui->resources[i]);
		}
	}
}

static int GUI_loadResources(GUI* gui) {
	int i;
	for (i = 0; i < NB_RSC; i ++) {
		gui->resources[i] = NULL;
	}

	if (!GUI_addResource(gui, RSC_BACKGROUND, PATH_BACKGROUND) ||
		!GUI_addResource(gui, RSC_DEVIN, PATH_DEVIN) ||
		!GUI_addResource(gui, RSC_MENU, PATH_MENU) ||
		!GUI_addResource(gui, RSC_ANSWERS, PATH_ANSWERS) ||
		!GUI_addResource(gui, RSC_QUESTION_BUBBLE, PATH_QUESTIONBUBBLE) ||
		!GUI_addResource(gui, RSC_YES, PATH_YESBUTTON) ||
		!GUI_addResource(gui, RSC_NO, PATH_NOBUTTON)
		) {
		GUI_releaseResourcesAllocated(gui);
		return 0;
	}

	if (!(gui->font = MLV_load_font("data/Font.ttf", 45))) {
		GUI_releaseResourcesAllocated(gui);
		return 0;
	}

	return 1;
}

static int GUI_addResource(GUI *gui, int index, char const *rscPath) {
	if (!(gui->resources[index] = MLV_load_image(rscPath))) {
		fprintf(stderr, "Error while loading %s image.\n", rscPath);
		return 0;
	}
	return 1;
}

static void GUI_displayGameStructure(GUI const *gui) {
	GUI_clear(gui);
	GUI_displayDevin(gui);
	GUI_displayQuestionBubble(gui);
	GUI_refresh();
}

static void GUI_displayDevin(GUI const* gui) {
	MLV_resize_image(gui->resources[RSC_DEVIN], W_DEVIN, H_DEVIN);
	MLV_draw_image(gui->resources[RSC_DEVIN], X_DEVIN, Y_DEVIN);
}

static void GUI_displayQuestionBubble(GUI const *gui) {
	MLV_resize_image(gui->resources[RSC_QUESTION_BUBBLE], W_QUESTION, H_QUESTION);
	MLV_draw_image(gui->resources[RSC_QUESTION_BUBBLE], X_QUESTION, Y_QUESTION);
}

static void GUI_displayAnswersButton(GUI const *gui) {
	MLV_resize_image(gui->resources[RSC_ANSWERS], W_ANSWERS, H_ANSWERS);
	MLV_draw_image(gui->resources[RSC_ANSWERS], X_ANSWERS, Y_ANSWERS);
}

static void GUI_displayYesNoButtons(GUI const *gui) {
	MLV_resize_image(gui->resources[RSC_YES], W_YES, H_YES);
	MLV_resize_image(gui->resources[RSC_NO], W_NO, H_NO);
	MLV_draw_image(gui->resources[RSC_YES], X_YES, Y_YES);
	MLV_draw_image(gui->resources[RSC_NO], X_NO, Y_NO);
	GUI_refresh();
}

static void initializeButtons(void) {
	Button_initialize(&buttons[BUT_ANSWER1], X_ANSWER1, Y_ANSWER1, W_ANSWER1, H_ANSWER1);
	Button_initialize(&buttons[BUT_ANSWER2], X_ANSWER2, Y_ANSWER2, W_ANSWER2, H_ANSWER2);
	Button_initialize(&buttons[BUT_ANSWER3], X_ANSWER3, Y_ANSWER3, W_ANSWER3, H_ANSWER3);
	Button_initialize(&buttons[BUT_ANSWER4], X_ANSWER4, Y_ANSWER4, W_ANSWER4, H_ANSWER4);
	Button_initialize(&buttons[BUT_ANSWER5], X_ANSWER5, Y_ANSWER5, W_ANSWER5, H_ANSWER5);
	Button_initialize(&buttons[BUT_PLAY], X_PLAY, Y_PLAY, W_PLAY, H_PLAY);
	Button_initialize(&buttons[BUT_QUIT], X_QUIT, Y_QUIT, W_QUIT, H_QUIT);
	Button_initialize(&buttons[BUT_YES], X_YES, Y_YES, W_YES, H_YES);
	Button_initialize(&buttons[BUT_NO], X_NO, Y_NO, W_NO, H_NO);
	Button_initialize(&buttons[BUT_INPUTBOXNAME], X_TEXTBOXNAME, Y_TEXTBOXNAME, W_TEXTBOXNAME, H_TEXTBOXNAME);
	Button_initialize(&buttons[BUT_INPUTBOXQUEST], X_TEXTBOXQUEST, Y_TEXTBOXQUEST, W_TEXTBOXQUEST, H_TEXTBOXQUEST);
}

static void Button_initialize(Button *button, int x, int y, int w, int h) {
	button->x = x;
	button->y = y;
	button->w = w;
	button->h = h;
}

static int GUI_getEvent(void) {
	MLV_Button_state bs;
	MLV_Mouse_button mb;
	input.type = MLV_NONE;
	int type = MLV_get_event(NULL, NULL, NULL, &input.text, &input.box, &input.clickX, &input.clickY, &mb, &bs);
	if ((type == MLV_MOUSE_BUTTON && mb == MLV_BUTTON_LEFT && bs == MLV_RELEASED) || type == MLV_INPUT_BOX) {
		input.type = type;
		return 1;
	} 
	return 0;
}