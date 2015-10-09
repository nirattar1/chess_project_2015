/*
 * Files.c
 *
 *  Created on: Oct 4, 2015
 *      Author: nirattar
 */


#include "Files.h"
#include "Game.h"

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
static void
print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }

        print_element_names(cur_node->children);
    }
}

int LoadGame(game_state_t * game,char * filename)
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

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
    if (doc == NULL)
    {
        status = 0;
    }
    else
    {
    	status = 1; //parsed the file.

    	//TODO file load - do it properly.

    	//process it.
    	/*Get the root element node */
        root_element = xmlDocGetRootElement(doc);

        print_element_names(root_element);
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


void SaveGame(const game_state_t * game,char * FileName)
{

	xmlDocPtr doc = NULL;       /* document pointer */
	xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
	char buff[256];
	int i, j;

	LIBXML_TEST_VERSION;

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST CHESS_XML_NODE_MAIN);
	xmlDocSetRootElement(doc, root_node);

	//create board node (later - fill and link to root)
	xmlNodePtr board_node = xmlNewNode(NULL, BAD_CAST CHESS_XML_NODE_BOARD);
	xmlAddChild(root_node, board_node);

	//generate board content
	//generate row node for each row on board.
	for (int i = BOARD_SIZE; i>0; i--)
	{
		char row_node_name [6];  //TODO could be bug if board-size > 9
		//compose row name from number
		sprintf(row_node_name, CHESS_XML_NODE_ROW_FORMAT, i);
		xmlNodePtr row_node = xmlNewNode(NULL, BAD_CAST row_node_name);

		//add value to row.

		//build row as string from game
		char row_content_chars [BOARD_SIZE+1];
		GetRowAsString (game, i, row_content_chars);
		//update node's content to our string
		xmlNodePtr row_content = xmlNewText(BAD_CAST row_content_chars);
		xmlAddChild(row_node, row_content);

		//link row node to board.
		xmlAddChild(board_node, row_node);

	}



	/*
	 * Dumping document to stdio or file
	 */
	xmlSaveFormatFileEnc(FileName, doc, "UTF-8", 1);

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

	//TODO error codes

}

//will update the string row_content_chars,
//according to the pieces in row row_index in given game.
static void GetRowAsString (const game_state_t * game, int row_index, char * row_content_chars)
{

	//iterate on all columns to get their pieces
	for (int j=0; j<BOARD_SIZE; j++)
	{
		char column = 'a' + j;
		//TODO fix const with game functions
		char piece_identity = GetPiece(Position(column, row_index), (game_state_t *) game);
		if (piece_identity==EMPTY)
		{
			//empty has different represenataion in xml
			piece_identity = CHESS_XML_NODE_ROW_EMPTY_PIECE;
		}
		//add piece's char to string.
		row_content_chars[j] = piece_identity;
	}

	//append null byte to end string
	row_content_chars [BOARD_SIZE] = 0;
}
