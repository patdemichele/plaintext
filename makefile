OBJS = htmlparser.o networking.o readwrite.o main.o
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

readwrite.o : readwrite.h readwrite.cc
	$(CC) $(CFLAGS) readwrite.cc

main.o : main.cc
	$(CC) $(CFLAGS) main.cc

clean:
	\rm *.o *~ exe
