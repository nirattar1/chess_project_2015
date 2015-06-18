all: Draughts 

clean:
	-rm Draughts.o Game.o ListNode.o Memory.o Minimax.o Draughts

Draughts: Draughts.o Game.o ListNode.o Memory.o Minimax.o
	gcc -o Draughts Draughts.o Game.o ListNode.o Memory.o Minimax.o -lm -std=c99 -pedantic-errors -g

Draughts.o: Draughts.h Draughts.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Draughts.c

Game.o: Game.h Game.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Game.c
	
ListNode.o: ListNode.h ListNode.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm ListNode.c

Memory.o: Memory.h Memory.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Memory.c
	
Minimax.o: Minimax.h Minimax.c
	gcc -std=c99 -pedantic-errors -c -Wall -g -lm Minimax.c