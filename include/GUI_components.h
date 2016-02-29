#ifndef GUI_COMPONENTS_H
#define GUI_COMPONENTS_H

/**
 * \brief Size of window
 */
#define W_WINDOW 1000
#define H_WINDOW 800

#define W_BORDER 10
#define H_BORDER 10

/**********************************
 * Positions of menu's components *
 *********************************/

 /**
 * \brief Position of button PLAY
 */
#define X_PLAY W_BORDER
#define Y_PLAY (H_WINDOW/4)
#define W_PLAY ((W_WINDOW/2) - W_BORDER)
#define H_PLAY (H_WINDOW/4)


 /**
 * \brief Position of button QUIT
 */
#define X_QUIT W_BORDER
#define Y_QUIT ((5*H_WINDOW)/8)
#define W_QUIT ((W_WINDOW/2) - W_BORDER)
#define H_QUIT (H_WINDOW/4)


/**********************************
 * Positions of game's components *
 *********************************/

/**
 * \brief Position of devin
 */
#define X_DEVIN (W_WINDOW/10)
#define Y_DEVIN (H_WINDOW/4)
#define W_DEVIN ((3*W_WINDOW)/10)
#define H_DEVIN ((5*H_WINDOW)/8)

/**
 * \brief Position of question's zone
 */
#define X_QUESTION ((3*W_WINDOW)/10)
#define Y_QUESTION (H_WINDOW/8) - 100
#define W_QUESTION (W_WINDOW/2)
#define H_QUESTION ((3*W_WINDOW)/8)

/**
 * \brief Position of question's text
 */
#define X_QUESTEXT (X_QUESTION + (W_QUESTION/8))
#define Y_QUESTEXT (Y_QUESTION + (H_QUESTION/6))
#define W_QUESTEXT (W_QUESTION - 100)
#define H_QUESTEXT ((3*H_QUESTION)/5)

#define MAX_CHAR_LINE 21

/**
 * \brief Position of answers' zone
 */
#define X_ANSWERS (W_WINDOW/2)
#define Y_ANSWERS (H_WINDOW/2)
#define W_ANSWERS ((4*W_WINDOW)/10)
#define H_ANSWERS (H_WINDOW/2)

/**
 * \brief Position of answer 1's button (Oui)
 */
#define X_ANSWER1 X_ANSWERS
#define Y_ANSWER1 Y_ANSWERS
#define W_ANSWER1 W_ANSWERS
#define H_ANSWER1 (H_ANSWERS/5)

/**
 * \brief Position of answer 2's button (Probablement Oui)
 */
#define X_ANSWER2 X_ANSWERS
#define Y_ANSWER2 (Y_ANSWERS + (H_ANSWERS/5))
#define W_ANSWER2 W_ANSWERS
#define H_ANSWER2 (H_ANSWERS/5)

/**
 * \brief Position of answer 3's button (Ne sais pas)
 */
#define X_ANSWER3 X_ANSWERS
#define Y_ANSWER3 (Y_ANSWERS + ((2*H_ANSWERS)/5))
#define W_ANSWER3 W_ANSWERS
#define H_ANSWER3 (H_ANSWERS/5)

/**
 * \brief Position of answer 4's button (Probablement non)
 */
#define X_ANSWER4 X_ANSWERS
#define Y_ANSWER4 (Y_ANSWERS + ((3*H_ANSWERS)/5))
#define W_ANSWER4 W_ANSWERS
#define H_ANSWER4 (H_ANSWERS/5)

/**
 * \brief Position of answer 5's button (Non)
 */
#define X_ANSWER5 X_ANSWERS
#define Y_ANSWER5 (Y_ANSWERS + ((4*H_ANSWERS)/5))
#define W_ANSWER5 W_ANSWERS
#define H_ANSWER5 (H_ANSWERS/5)


/**
 * \brief Position of yes button
 */
#define X_YES (X_ANSWERS - 50)
#define Y_YES Y_ANSWER2
#define W_YES W_ANSWERS/2
#define H_YES H_ANSWERS/5

 /**
  * \brief Position of no button
  */ 
#define X_NO (X_ANSWERS + (W_ANSWERS/2) + 50)
#define Y_NO Y_ANSWER2
#define W_NO W_ANSWERS/2
#define H_NO H_ANSWERS/5


/********************************************************************
 * Components' position of the window Add a question to a character *
 *******************************************************************/

/**
 * \brief Position of the name's TextBox
 */
#define X_TEXTBOXNAME W_BORDER
#define Y_TEXTBOXNAME ((2 * H_WINDOW)/10)
#define W_TEXTBOXNAME (((7*W_WINDOW)/10) - W_BORDER)
#define H_TEXTBOXNAME (H_WINDOW/10)

/**
 * \brief Position of the question's TextBox
 */
#define X_TEXTBOXQUEST X_TEXTBOXNAME
#define Y_TEXTBOXQUEST ((4 * H_WINDOW)/10)
#define W_TEXTBOXQUEST (W_TEXTBOXNAME)
#define H_TEXTBOXQUEST H_TEXTBOXNAME



/**************************
 * Resources localisation *
 *************************/

#define PATH_BACKGROUND "data/background.png"
#define PATH_DEVIN "data/devin.png"
#define PATH_MENU "data/menu.png"
#define PATH_ANSWERS "data/answers.png"
#define PATH_QUESTIONBUBBLE "data/questionBubble.png"
#define PATH_PREVIOUSBUTTON "data/previousButton.png"
#define PATH_OKBUTTON "data/OKButton.png"
#define PATH_YESBUTTON "data/yesButton.png"
#define PATH_NOBUTTON "data/noButton.png"



#endif