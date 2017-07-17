OBJS = htmlparser.o networking.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

exe : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o exe

htmlparser.o : htmlparser.h htmlparser.cc
	$(CC) $(CFLAGS) htmlparser.cc

networking.o : networking.h networking.cc
	$(CC) $(CFLAGS) networking.cc

main.o : main.cc
	$(CC) $(CFLAGS) main.cc

new : new.o networking.o
	$(CC) $(LFLAGS) new.o networking.o -o new

new.o : new.cc
	$(CC) $(CFLAGS) new.cc

clean:
	\rm *.o *~ exe
