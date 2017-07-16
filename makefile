OBJS = htmlparser.o networking.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

whatever : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o exe

htmlparser.o : htmlparser.h
	$(CC) $(CFLAGS) htmlparser.cc

server.o : networking.h
	$(CC) $(CFLAGS) networking.cc

clean:
	\rm *.o *~ exe