/*
 * Files.c
 *
 *  Created on: Oct 4, 2015
 *      Author: nirattar
 */


#include "Files.h"
#include "Game.h"
#include "Game_Mgr.h"

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */


//static func. declarations
static void BoardRowToString (const game_state_t * game, int row_index, char * row_content_chars);
static void BoardRowFromString (game_state_t * game, int row_index, const char * row_content_chars);


//will load data into game. from open xml object.
static int XML_GameNodeDataLoad(game_state_t * game, xmlDoc * doc)
{
	if (!doc)
	{
		return 0;
	}

	//process it.
	/*Get the root element node */
    xmlNode * cur_node = xmlDocGetRootElement(doc);


    //root node must be "game"
    if (cur_node->type == XML_ELEMENT_NODE)
    {
    	if (!xmlStrEqual(cur_node->name, (const xmlChar *) CHESS_XML_NODE_MAIN))
    	{
    		//not supposed to happen (expecting legal xml structure)
    		return 0;
    	}
    }

    //loop through all children of root and extract data.
    for (cur_node=cur_node->children; cur_node!=NULL; cur_node = cur_node->next)
    {
    	if (cur_node->type != XML_ELEMENT_NODE)
    	{
    		//expecting only "element" type nodes
    		continue;
    	}

    	//retrieve next turn
        if (xmlStrEqual(cur_node->name, (const xmlChar *) CHESS_XML_NODE_NEXT_TURN))
        {
        	xmlChar * value = xmlNodeGetContent(cur_node); //free later
        	if (xmlStrEqual(value, (const xmlChar *) "White"))
        	{
        		Settings_NextPlayer_Set(COLOR_WHITE);
            	DEBUG_PRINT(("next turn white\n"));
        	}
        	else if (xmlStrEqual(value, (const xmlChar *) "Black"))
        	{
        		Settings_NextPlayer_Set(COLOR_BLACK);
            	DEBUG_PRINT(("next turn black\n"));
        	}

        	//if not exist, do not change settings.

        	xmlFree(value);

        }

        //retrieve game mode
        else if (xmlStrEqual(cur_node->name, (const xmlChar *) CHESS_XML_NODE_GAME_MODE))
        {

        	xmlChar * value = xmlNodeGetContent(cur_node); //free later
        	int mode = strtol((char *) value,NULL,0);

        	//check game mode is legal
        	if (mode==GAME_MODE_PLAYER_VS_PLAYER || mode==GAME_MODE_PLAYER_VS_CPU)
        	{
        		Settings_GameMode_Set(mode);
            	DEBUG_PRINT(("game mode %d\n", mode));
        	}
			//if not legal, do not change settings.

			xmlFree(value);

        }

        //retrieve difficulty
        else if (xmlStrEqual(cur_node->name, (const xmlChar *) CHESS_XML_NODE_DIFFICULTY))
        {
            //relevant only on mode cpu, otherwise read next tag.
        	if (Settings_GameMode_Get()==GAME_MODE_PLAYER_VS_PLAYER)
        	{
        		continue;
        	}

        	xmlChar * value = xmlNodeGetContent(cur_node); //free later

        	//best difficulty
        	if (xmlStrEqual(value, (const xmlChar *) "best"))
        	{
        		Settings_MaxDepth_Set(MAX_DEPTH_BEST_VALUE);
				DEBUG_PRINT(("difficulty best\n"));
        	}
        	else
        	{
				//constant difficulty
				int difficulty = strtol((char *) value,NULL,0);

				if (difficulty>=MAX_DEPTH_MIN_VALUE && difficulty <= MAX_DEPTH_MAX_VALUE)
				{
					Settings_MaxDepth_Set(difficulty);
					DEBUG_PRINT(("difficulty %d\n", difficulty));
				}
        	}
        	//if not legal, do not change settings.

			xmlFree(value);
        }

    	//retrieve user color
        else if (xmlStrEqual(cur_node->name, (const xmlChar *) CHESS_XML_NODE_USER_COLOR))
        {

            //relevant only on mode cpu, otherwise read next tag.
        	if (Settings_GameMode_Get()==GAME_MODE_PLAYER_VS_PLAYER)
        	{
        		continue;
        	}

        	xmlChar * value = xmlNodeGetContent(cur_node); //free later
        	if (xmlStrEqual(value, (const xmlChar *) "White"))
        	{
        		Settings_UserColor_Set(COLOR_WHITE);
        		DEBUG_PRINT(("user color white\n"));
        	}
        	else if (xmlStrEqual(value, (const xmlChar *) "Black"))
        	{
        		Settings_UserColor_Set(COLOR_BLACK);
        		DEBUG_PRINT(("user color black\n"));
        	}

        	//if not exist, do not change settings.

        	xmlFree(value);

        }
        //retrieve board
        else if (xmlStrEqual(cur_node->name, (const xmlChar *) CHESS_XML_NODE_BOARD))
        {
        	//loop through rows.
        	xmlNode * row_node = cur_node->children;
			for (; row_node!=NULL; row_node = row_node->next)
			{
				if (row_node->type != XML_ELEMENT_NODE)
				{
					//expecting only "element" type nodes
					continue;
				}

				//get row number from node name
				char * row_name = (char *) row_node->name;
				int row_num = 0;
				sscanf (row_name, CHESS_XML_NODE_ROW_FORMAT, &row_num);

				xmlChar * value = xmlNodeGetContent(row_node); //free later
				//update row based on node content
				BoardRowFromString (game, row_num, (const char *) value);
				xmlFree(value);
			}

        	DEBUG_PRINT(("updated board. \n"));
        }

        //TODO load from "general" node.
    }

    return 1;

}

//will save data from game, intop open xml object.
static int XML_GameNodeDataSave(const game_state_t * game, xmlDocPtr doc)
{


	if (!doc)
	{
		return 0;
	}


	//create root node.
	xmlNodePtr root_node = xmlNewNode(NULL, BAD_CAST CHESS_XML_NODE_MAIN);
	xmlDocSetRootElement(doc, root_node);

	//create nodes based on current settings.

	//next turn
	color_t next_player = Settings_NextPlayer_Get();
	const char * next_turn_string = 0;
	if (next_player==COLOR_WHITE)
	{
		next_turn_string= "White";
	}
	else
	{
		next_turn_string= "Black";
	}
    xmlNewChild(root_node, NULL,
    		BAD_CAST CHESS_XML_NODE_NEXT_TURN, BAD_CAST next_turn_string);

	//game mode.
	//get from settings, convert to string
    int mode = Settings_GameMode_Get();
    char mode_string [2];
	sprintf(mode_string, "%d", mode);
	mode_string[1] = 0;
    xmlNewChild(root_node, NULL,
    		BAD_CAST CHESS_XML_NODE_GAME_MODE, BAD_CAST mode_string);


	//difficulty.
	//get from settings, convert to string
	char difficulty_string [5]; difficulty_string[0]=0; //null terminate string.
    int difficulty = Settings_MaxDepth_Get();
	//update to real value only in cpu mode.
    if (Settings_GameMode_Get()==GAME_MODE_PLAYER_VS_CPU)
    {
    	if (difficulty==MAX_DEPTH_BEST_VALUE)
    	{
    		//best difficulty
    		sprintf(difficulty_string, "best"); //null terminated.
    	}
    	else if (difficulty>=MAX_DEPTH_MIN_VALUE && difficulty <= MAX_DEPTH_MAX_VALUE)
    	{
    		//constant difficulty
    		sprintf(difficulty_string, "%d", difficulty);//null terminated.
    	}
    }
    xmlNewChild(root_node, NULL,
    		BAD_CAST CHESS_XML_NODE_DIFFICULTY, BAD_CAST difficulty_string);

	//user color
	color_t user_color = Settings_UserColor_Get();
	const char * user_color_string = "";
	//update to real value only in cpu mode.
	if (Settings_GameMode_Get()==GAME_MODE_PLAYER_VS_CPU)
	{
		if (user_color==COLOR_WHITE)
		{
			user_color_string= "White";
		}
		else
		{
			user_color_string= "Black";
		}
	}
    xmlNewChild(root_node, NULL,
    		BAD_CAST CHESS_XML_NODE_USER_COLOR, BAD_CAST user_color_string);

	//create board node, link it to root
	xmlNodePtr board_node = xmlNewNode(NULL, BAD_CAST CHESS_XML_NODE_BOARD);
	xmlAddChild(root_node, board_node);

	//fill board content
	//will generate row node for each row on board.
	for (int i = BOARD_SIZE; i>0; i--)
	{
		char row_node_name [6];  //TODO could be bug if board-size > 9
		//compose row name from number
		sprintf(row_node_name, CHESS_XML_NODE_ROW_FORMAT, i);
		xmlNodePtr row_node = xmlNewNode(NULL, BAD_CAST row_node_name);

		//add value to row.

		//build row as string from game
		char row_content_chars [BOARD_SIZE+1];
		BoardRowToString (game, i, row_content_chars);
		//update node's content to our string
		xmlNodePtr row_content = xmlNewText(BAD_CAST row_content_chars);
		xmlAddChild(row_node, row_content);

		//link row node to board.
		xmlAddChild(board_node, row_node);

	}

	//TODO create "general" node and add data to it.

	return 1;
}


int LoadGame(game_state_t * game,char * filename)
{
    xmlDoc *doc = NULL;

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /*parse the file and get the DOM */
	//call with XML_PARSE_NOWARNING to avoid errors by libxml
	//(assuming correct xml structure).
    doc = xmlReadFile(filename, NULL, XML_PARSE_NOWARNING);

    //hold status and return it at the end of func.
    int status = 0;
    if (doc != NULL)
    {
    	//parsed the file. save status
    	//now process the xml content into game.
    	status = XML_GameNodeDataLoad(game, doc);
    }

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    return status;

}


int SaveGame(const game_state_t * game,char * FileName)
{

	xmlDocPtr doc = NULL;       /* document pointer */

	LIBXML_TEST_VERSION;

	doc = xmlNewDoc(BAD_CAST "1.0");

	//update doc with game data.
	XML_GameNodeDataSave(game, doc);

	/*
	 * Dumping document to stdio or file
	 */
	//go and save.
	//in case of failure will return -1. (on success- number of bytes)
	int result = xmlSaveFormatFileEnc(FileName, doc, "UTF-8", 1);

	/*free the document */
	xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	/*
	 * this is to debug memory for regression tests
	 */
	xmlMemoryDump();

	//giving result code 1 on success, 0 on failure.
	if (result==-1)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}


//for load.
//will update the appropriate row inside game,
//based on given string.
static void BoardRowFromString (game_state_t * game, int row_index, const char * row_content_chars)
{
	//avoid accessing null string.
	if (!row_content_chars)
	{
		return;
	}

	int column = 0;

	//do not exceed board size / given char buffer
	while (*row_content_chars!=0 && column < BOARD_SIZE)
	{
		char identity = *row_content_chars;
		//empty has different represenataion in xml
		if (identity==CHESS_XML_NODE_ROW_EMPTY_PIECE)	{ identity = EMPTY;};
		//set piece on board.
		SetPiece(Position('a'+column, row_index), identity, game);
		//move on to next char
		row_content_chars++;
		column++;
	}
}


//for save.
//will update the string row_content_chars,
//according to the pieces in row row_index in given game.
static void BoardRowToString (const game_state_t * game, int row_index, char * row_content_chars)
{

	//iterate on all columns to get their pieces
	for (int j=0; j<BOARD_SIZE; j++)
	{
		char column = 'a' + j;
		//TODO fix const with game functions
		char piece_identity = GetPiece(Position(column, row_index), (game_state_t *) game);

		//empty has different represenataion in xml
		if (piece_identity==EMPTY)	{ piece_identity = CHESS_XML_NODE_ROW_EMPTY_PIECE;};

		//add piece's char to string.
		row_content_chars[j] = piece_identity;
	}

	//append null byte to end string
	row_content_chars [BOARD_SIZE] = 0;
}
