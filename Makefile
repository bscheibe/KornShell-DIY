# choose your compiler
CC=gcc
#CC=gcc -Wall

myshell: shell.o commands.o src/main.c src/commands.c
	$(CC) -g src/main.c shell.o commands.o -o myshell
#	$(CC) -g src/main.c sh.o commands.o bash_getcwd.o -o myshell

shell.o: ./src/shell.c src/shell.h
	$(CC) -g -c src/shell.c

commands.o: ./src/commands.c ./src/commands.h
	$(CC) -g -c ./src/commands.c

clean:
	rm -rf *.o myshell
