CC=g++

MAKE_EXE=-o exe

CFLAGS=-O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -std=c++11

and_go:	./includes/voraldo/voraldo.cc ./includes/voraldo/voraldo.h main.cc
	$(CC) $(MAKE_EXE) main.cc ./includes/voraldo/voraldo.cc $(CFLAGS)

go:
	./exe