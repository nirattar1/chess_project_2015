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


void LoadGame(game_state_t * game,char * FileName);
void SaveGame(const game_state_t * game,char * FileName);


#endif /* SRC_FILES_H_ */

