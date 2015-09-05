/*
 * Test_gui_framework.c
 *
 *  Created on: Sep 3, 2015
 *      Author: nirattar
 */

#include "../Gui_framework.h"

void main ()
{


}


//NO TREE YET, JUST DRAWING
void window_2_buttons_stupid()
{
	SDL_Surface screen = init_screen();
	//create objects
	Control * w = CreateWindow ();
	Control * b1 = CreateButton();
	Control * b2 = CreateButton();

	//link the objects
	AddChildElement (w, b1);
	AddChildElement (w, b2);

	//draw the objects.
	//(DFS traverse)
}

//DRAW WITH GUI TREE
void window_2_buttons()
{
	SDL_Surface screen = init_screen();
	//create objects
	Control * w = CreateWindow ();
	Control * b1 = CreateButton();
	Control * b2 = CreateButton();

	//link the objects
	AddChildElement (w, b1);
	AddChildElement (w, b2);

	//draw the objects.
	//(DFS traverse)
}
