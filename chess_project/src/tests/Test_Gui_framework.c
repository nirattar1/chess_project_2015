/*
 * Test_gui_framework.c
 *
 *  Created on: Sep 3, 2015
 *      Author: nirattar
 */

#include "../Gui_framework.h"

void main ()
{

	window_2_buttons_stupid();
}

void testfunction1()
{
	printf ("1st button!\n");
}

void testfunction2()
{
	printf ("2nd button!\n");
}

//just a couple of objects on screen. no tree relations.
//-create background, 2 distinct buttons.
//-handle buttons actions.
//-exit gracefully
void window_2_buttons_stupid()
{
	//init SDL
	if (init_sdl() != 0)
	{
		exit (1);
	}

	//init screen
	//(screen is a global surface)
	SDL_Surface * screen = init_screen();

	//create objects (not attached to each other).
	Control * w1 = WindowCreate("imgs/background.bmp", NULL);

	SDL_Rect b1Rect = {10, 10, 20, 30};
	Control * b1 = ButtonCreate("imgs/new_g.bmp", &b1Rect, testfunction1);

	SDL_Rect b2Rect = {10, 100, 20, 30};
	Control * b2 = ButtonCreate("imgs/load_g.bmp", &b2Rect, testfunction2);

	//draw them
	w1->Draw(w1, screen);
	b1->Draw(b1, screen);
	b2->Draw(b2, screen);

	//handle events until exit.
	HandleEvents(b1, b2);

	//clear controls resources
	//...
}

////DRAW WITH GUI TREE
//void window_2_buttons()
//{
//	SDL_Surface * screen = init_screen();
//	//create objects
//	Control * w = CreateWindow ();
//	Control * b1 = CreateButton();
//	Control * b2 = CreateButton();
//
//	//link the objects
//	AddChildElement (w, b1);
//	AddChildElement (w, b2);
//
//	//draw the objects.
//	//(DFS traverse)
//}
