/*
 * Console_ui.h
 *
 *  Created on: Jun 18, 2015
 *      Author: nirattar
 */

#ifndef CONSOLE_UI_H_
#define CONSOLE_UI_H_


int Menu_Settings(game_state_t * game, char ** board);
move_t Menu_PlayUser(game_state_t * game);

//get string representing identity (such "white knight").
//return the appropriate char representation.
char GetIdentityByString (char * line);

#endif /* CONSOLE_UI_H_ */
