CC = gcc
CFLAGS  = -std=c99 -g -std=c99 
#gtk-config --cflags --libs gtk+-2.0
#`pkg-config --cflags --libs gtk+-2.0`
rzucacz: rzucacz.c renderer.o bsp.o postscript.o geometry.o lista.o
	$(CC) $(CFLAGS)  rzucacz.c $(shell pkg-config --cflags --libs gtk+-2.0) gui.o  renderer.o bsp.o postscript.o geometry.o lista.o -o rzucacz -lm

gui.o: gui.c gui.h
	$(CC) $(CFLAGS) -c gui.c $(shell pkg-config --cflags --libs gtk+-2.0) renderer.o -o gui.o

renderer.o: renderer.c renderer.h
	$(CC) $(CFLAGS) -c renderer.c bsp.o -o renderer.o

bsp.o: bsp.c bsp.h
	$(CC) $(CFLAGS) -c bsp.c -o bsp.o

postscript.o: postscript.c geometry.o
	$(CC) $(CFLAGS) -c postscript.c geometry.o -o postscript.o

lista.o: lista.c lista.h
	$(CC) $(CFLAG) -c lista.c -o lista.o

geometry.o: geometry.c 
	$(CC) $(CFLAGS) -c geometry.c -o geometry.o
