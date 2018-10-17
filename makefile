CC=g++

MAKE_EXE=-o exe

CFLAGS=-O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -std=c++11

and_go:	./includes/voraldo/voraldo.cc ./includes/voraldo/voraldo.h ./includes/block/block.cc ./includes/block/block.h main.cc
	$(CC) $(MAKE_EXE) main.cc ./includes/voraldo/voraldo.cc ./includes/block/block.cc $(CFLAGS)

go:
	./exe