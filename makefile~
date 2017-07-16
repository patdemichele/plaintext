OBJS = htmlparser.o server.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

whatever : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o exe

htmlparser.o : htmlparser.h
	$(CC) $(CFLAGS) htmlparser.cc

server.o : server.h
	$(CC) $(CFLAGS) server.cc

clean:
	\rm *.o *~ exe