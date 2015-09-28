#all: sdl_test
#
#clean:
#	-rm sdl_test.o sdl_test
#
#sdl_test: sdl_test.o
#	gcc  -o sdl_test sdl_test.o -lm -std=c99 -pedantic-errors -g `sdl-config --libs`
#
#sdl_test.o: sdl_test.c
#	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm sdl_test.c `sdl-config --cflags`

all: sdl_test2

clean:
	-rm sdl_test2.o sdl_test2

sdl_test2: sdl_test2.o
	gcc  -o sdl_test2 sdl_test2.o -lm -std=c99 -pedantic-errors -g `sdl-config --libs`

sdl_test2.o: sdl_test2.c
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm sdl_test2.c `sdl-config --cflags`
