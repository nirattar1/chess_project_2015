all: chessprog 

clean:
	-rm Chess.o Game.o ListNode.o Memory.o Minimax.o Console_ui.o Test_Main.o chessprog

chessprog: Chess.o Game.o ListNode.o Memory.o Minimax.o Console_ui.o Tests
	gcc -o chessprog Chess.o Game.o ListNode.o Memory.o Minimax.o Console_ui.o Test_Main.o Test_GameBasic.o -lm -std=c99 -pedantic-errors -g

Chess.o: Chess.h Chess.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Chess.c

Game.o: Game.h Game.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Game.c
	
ListNode.o: ListNode.h ListNode.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm ListNode.c

Memory.o: Memory.h Memory.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Memory.c
	
Minimax.o: Minimax.h Minimax.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Minimax.c
	
Console_ui.o: Console_ui.h Console_ui.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Console_ui.c

Tests: Test_Main.o Test_GameBasic.o

Test_GameBasic.o: tests/Test_GameBasic.h tests/Test_GameBasic.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm tests/Test_GameBasic.c
	
Test_Main.o: tests/Test_Main.h tests/Test_Main.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm tests/Test_Main.c
	
