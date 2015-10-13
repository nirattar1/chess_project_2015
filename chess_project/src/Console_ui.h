/*
 * Console_ui.h
 *
 *  Created on: Jun 18, 2015
 *      Author: nirattar
 */

#ifndef CONSOLE_UI_H_
#define CONSOLE_UI_H_


int Menu_Settings(game_state_t * game, char ** board);
int Menu_PlayUser(game_state_t * game, move_t * move);

//handles end of game. will print all the appropriate messages.
void ConsoleUI_GameHandleEnd
	(game_state_t * game, play_status_t play_status, color_t next_player);

//prints a message if there is a check (only) for next player.
void ConsoleUI_GameHandleCheck (play_status_t play_status, color_t next_player);


//get string representing identity (such "white knight").
//return the appropriate char representation.
char GetIdentityByString (char * line);

#endif /* CONSOLE_UI_H_ */
