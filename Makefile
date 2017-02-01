CC = gcc
CFLAGS = -Wall -ansi -pedantic -g 
MAIN = guess
OBJS = guess.o readLine.o database.o 

all : $(MAIN)

$(MAIN) : $(OBJS) readLine.h database.h guess.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) 

