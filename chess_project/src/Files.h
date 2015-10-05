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

#define CHESS_XML_NODE_MAIN "game"
#define CHESS_XML_NODE_BOARD "board"
#define CHESS_XML_NODE_ROW_FORMAT "row_%d"
#define CHESS_XML_NODE_ROW_EMPTY_PIECE '_'

void LoadGame(game_state_t * game,char * FileName);
void SaveGame(const game_state_t * game,char * FileName);
static void GetRowAsString (const game_state_t * game, int row_index, char * row_content_chars);

#endif /* SRC_FILES_H_ */

