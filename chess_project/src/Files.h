/*
 * Files.h
 *
 *  Created on: Oct 2, 2015
 *      Author: nirattar
 */

#ifndef SRC_FILES_H_
#define SRC_FILES_H_


#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Game.h"

#define CHESS_XML_NODE_MAIN 		"game"
#define CHESS_XML_NODE_NEXT_TURN 	"next_turn"
#define CHESS_XML_NODE_GAME_MODE 	"game_mode"
#define CHESS_XML_NODE_DIFFICULTY 	"difficulty"
#define CHESS_XML_NODE_USER_COLOR 	"user_color"
#define CHESS_XML_NODE_BOARD 		"board"
#define CHESS_XML_NODE_ROW_FORMAT 	"row_%d"
#define CHESS_XML_NODE_ROW_EMPTY_PIECE '_'


#define NUM_SAVE_SLOTS 7
#define SLOTS_IMG_FILE_FORMAT "imgs/slot_%d.bmp"
#define SLOTS_XML_FILE_FORMAT "saves/slot_%d.xml"

//load the game stored in file "filename", into game struct.
//returns: 1 on success, 0 on failure.
int LoadGame(game_state_t * game,char * filename);

//save the game in game struct, into file "filename".
//returns: 1 on success, 0 on failure.
int SaveGame(const game_state_t * game,char * FileName);

static void BoardRowToString (const game_state_t * game, int row_index, char * row_content_chars);
static void BoardRowFromString (game_state_t * game, int row_index, const char * row_content_chars);
#endif /* SRC_FILES_H_ */

