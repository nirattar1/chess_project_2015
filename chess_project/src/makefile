all: chessprog 

CFLAGS=-std=c99 -pedantic-errors -Wall -g -lm 
XML_CFLAGS=`xml2-config --cflags` 
SDL_CFLAGS=`sdl-config --cflags`
LIBS=`xml2-config --libs` `sdl-config --libs`

O_FILES=Game_Mgr.o Game.o ListNode.o Memory.o Minimax.o Console_ui.o Files.o Gui_framework.o Gui.o
TEST_O_FILES=Test_Main.o Test_GameBasic.o Test_Files.o Test_Gui_framework.o

clean:
	-rm chessprog $(O_FILES) $(TEST_O_FILES)

chessprog: $(O_FILES) $(TEST_O_FILES)
	gcc $(CFLAGS) -o $@ $^ $(LIBS)

Game_Mgr.o: Game_Mgr.h Game_Mgr.c
	gcc -c $(CFLAGS) $*.c $(SDL_CFLAGS)

Game.o: Game.h Game.c
	gcc -c $(CFLAGS) $*.c
	
ListNode.o: ListNode.h ListNode.c
	gcc -c $(CFLAGS) $*.c

Memory.o: Memory.h Memory.c
	gcc -c $(CFLAGS) $*.c
	
Minimax.o: Minimax.h Minimax.c
	gcc -c $(CFLAGS) $*.c
	
Console_ui.o: Console_ui.h Console_ui.c
	gcc -c $(CFLAGS) $*.c $(XML_CFLAGS)

Files.o: Files.h Files.c
	gcc -c $(CFLAGS) $*.c $(XML_CFLAGS)

Gui_framework.o: Gui_framework.h Gui_framework.c
	gcc -c $(CFLAGS) $*.c $(SDL_CFLAGS)

Gui.o: Gui.h Gui.c
	gcc -c $(CFLAGS) $*.c $(SDL_CFLAGS)


#TODO makefile - from here only for testing - not needed

Test_GameBasic.o: tests/Test_GameBasic.h tests/Test_GameBasic.c
	gcc -c $(CFLAGS) tests/Test_GameBasic.c
	
Test_Main.o: tests/Test_Main.h tests/Test_Main.c
	gcc -c $(CFLAGS) tests/Test_Main.c
	
Test_Files.o: tests/Test_Files.h tests/Test_Files.c
	gcc -c $(CFLAGS) tests/Test_Files.c $(XML_CFLAGS)
	
Test_Gui_framework.o: tests/Test_Gui_framework.h tests/Test_Gui_framework.c
	gcc -c $(CFLAGS) tests/Test_Gui_framework.c $(SDL_CFLAGS)