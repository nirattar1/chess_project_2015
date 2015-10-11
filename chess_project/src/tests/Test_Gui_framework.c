/*
 * Test_gui_framework.c
 *
 *  Created on: Sep 3, 2015
 *      Author: nirattar
 */

#include "../Gui_framework.h"
#include "Test_Gui_framework.h"

Control * TestNotifyRelevantControl (SDL_Event * e,
		Control * b1, Control * b2);

void TestHandleEvents(Control * b1,Control * b2);



//test
void Test_Gui_framework ()
{

	//window_2_buttons_stupid();

	window_buttons_tree();

	//test_panels();
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
	//(screen is a global surface).
	//(will be freed by SDL_Quit).
	SDL_Surface * screen = init_screen("Chess");

	//create objects (not attached to each other).
	//(window has no rect).
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
	TestHandleEvents(b1, b2);

	//clear controls resources
	ControlFree(w1);
	ControlFree(b1);
	ControlFree(b2);

}

//DRAW WITH GUI TREE
void window_buttons_tree()
{
	//init SDL
	if (init_sdl() != 0)
	{
		exit (1);
	}

	//init screen
	//(screen is a global surface).
	//(will be freed by SDL_Quit).
	SDL_Surface * screen = init_screen("Chess");

	//create objects (not attached to each other).
	//(window has no rect).
	Control * w1 = WindowCreate("imgs/background.bmp", NULL);

	SDL_Rect b1Rect = {20, 100, 20, 30};
	Control * b1 = ButtonCreate("imgs/new_g.bmp", &b1Rect, testfunction2);

	SDL_Rect b2Rect = {180, 100, 20, 30};
	Control * b2 = ButtonCreate("imgs/load_g.bmp", &b2Rect, testfunction1);

	//children of b1
	SDL_Rect b3Rect = {0, 200, 20, 30};
	Control * b3 = ButtonCreate("imgs/load_g.bmp", &b3Rect, testfunction2);

	SDL_Rect b4Rect = {40, 200, 20, 30};
	Control * b4 = ButtonCreate("imgs/load_g.bmp", &b4Rect, testfunction2);

	//children of b2
	SDL_Rect b5Rect = {160, 200, 20, 30};
	Control * b5 = ButtonCreate("imgs/load_g.bmp", &b5Rect, testfunction2);

	SDL_Rect b6Rect = {200, 200, 20, 30};
	Control * b6 = ButtonCreate("imgs/load_g.bmp", &b6Rect, testfunction2);

	//button with clipping rect
	SDL_Rect b7Rect = {200, 300, 20, 30};
	Control * b7 = ButtonCreate("imgs/load_g.bmp", &b7Rect, testfunction2);
//	//set clipping rect for button
//	SDL_Rect b7ClipRect = {200, 300, 15, 15};
//	SDL_SetClipRect(screen, &b7ClipRect);

	//link the objects
	ControlAddChild(w1, b1);
	ControlAddChild(w1, b2);
	ControlAddChild(b1, b3);
	ControlAddChild(b1, b4);
	ControlAddChild(b2, b5);
	ControlAddChild(b2, b6);
	ControlAddChild(b2, b7);

	//draw the objects.
	//(DFS traverse)
	DFSTraverseDraw (w1, screen);


	//handle events until exit.
	TestHandleEvents(b1, b2);

	//clear controls resources
	ControlFree(w1);

}

void test_panels ()
{
	//create 2 panels with buttons inside them.
	//init SDL
	if (init_sdl() != 0)
	{
		exit (1);
	}

	//init screen
	//(screen is a global surface).
	//(will be freed by SDL_Quit).
	SDL_Surface * screen = init_screen("Chess");

	//create objects (not attached to each other).
	//(window has no rect).
	Control * w1 = WindowCreate("imgs/background.bmp", NULL);

	SDL_Rect p1Rect = {20, 100, 0, 0};
	Control * p1 = PanelCreate("imgs/misgarat.bmp", &p1Rect);

	SDL_Rect b1Rect = {20, 140, 0, 0};
	Control * b1 = ButtonCreate("imgs/new_g.bmp", &b1Rect, testfunction2);

	SDL_Rect b2Rect = {20, 220, 0, 0};
	Control * b2 = ButtonCreate("imgs/load_g.bmp", &b2Rect, testfunction1);


	//link them together
	ControlAddChild(w1, p1);
	//buttons are children of panel
	ControlAddChild(p1, b1);
	ControlAddChild(p1, b2);

	//draw the objects.
	//(DFS traverse)
	//DFSTraverseDraw (w1, screen);

	//draw window
	w1->Draw(w1, screen);

	//draw panel
	p1->Draw(p1, screen);

	//draw buttons inside their panel.
	b1->Draw(b1, screen);
	b2->Draw(b2, screen);

//
//	//draw button inside panel's rect.
//    /* Blit onto the screen surface */
//	SDL_SetClipRect(screen, &p1Rect);
//    if(SDL_BlitSurface(b1->surface, NULL, screen, b1->rect) < 0)
//    {
//        fprintf(stderr, "BlitSurface error: %s\n", SDL_GetError());
//    }
//    SDL_SetClipRect(screen, NULL);
//    //SDL_UpdateRect(dest, 0, 0, src->w, src->h);
//    //flip instead of update
//    SDL_Flip(screen);


	//handle events
	HandleEventsLoop(w1);

	//get rid of everything.
	//ControlFree((void *) w1);

}

//find and notify relevant control of event.
//simple test version
Control * TestNotifyRelevantControl (SDL_Event * e,
		//DEBUG parameters
		Control * b1, Control * b2)
{
	Control * found = NULL;
	SDL_Rect * b1Rect = b1->rect;
	SDL_Rect * b2Rect = b2->rect;

	//check boundaries of the control
	if ((e->button.x > b1Rect->x) && (e->button.x < b1Rect->x + b1Rect->w) && (e->button.y > b1Rect->y) && (e->button.y < b1Rect->y+b1Rect->h))
	{
		//mark control as found.
		found = b1;
	}
	if ((e->button.x > b2Rect->x) && (e->button.x < b2Rect->x + b2Rect->w) && (e->button.y > b2Rect->y) && (e->button.y < b2Rect->y+b2Rect->h))
	{
		found = b2;
	}


	//real life - loop through all controls.


	//call the function of control ( if exists ).
	if (found)
	{
		found->HandleEvents(e);
	}
	return found;
}



//a loop for handling events.
//simple test version.
void TestHandleEvents(
		Control * b1,Control * b2
)
{
	int quit = 0;

	while (!quit)
	{
		/* Poll for keyboard & mouse events*/
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
				case (SDL_QUIT):
					quit = 1;
					break;
				case (SDL_KEYUP):
					if (e.key.keysym.sym == SDLK_ESCAPE)
					{
						quit = 1;
					}
					break;
				case (SDL_MOUSEBUTTONUP):
					//send the event to the relevant control.
					;
					Control * matched = TestNotifyRelevantControl (&e, b1, b2);
					if (matched)
					{
						printf ("matched control!\n");
					}
					break;
				default:
					break;
			}
		}
		/* Wait a little before redrawing*/
		SDL_Delay(200);
	}
}
