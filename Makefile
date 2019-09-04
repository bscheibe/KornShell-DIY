# choose your compiler
CC=gcc
#CC=gcc -Wall

myshell: sh.o commands.o src/main.c 
	$(CC) -g src/main.c sh.o commands.o -o myshell
#	$(CC) -g src/main.c sh.o commands.o bash_getcwd.o -o myshell

sh.o: ./src/sh.c src/sh.h
	$(CC) -g -c src/sh.c

commands.o: ./src/commands.c ./src/commands.h
	$(CC) -g -c ./src/commands.c

clean:
	rm -rf *.o myshell
