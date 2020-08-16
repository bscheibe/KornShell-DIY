# TODO Update makefile to be dynamic for object files, and build .o files in the build dir.
CC=gcc -Wall

shell: shell.o commands.o src/main.c src/commands.c
	$(CC) -g src/main.c shell.o commands.o -o build/shell
	mv *.o build/

shell.o: ./src/shell.c src/shell.h
	$(CC) -g -c src/shell.c

commands.o: ./src/commands.c ./src/commands.h
	$(CC) -g -c ./src/commands.c

clean:
	rm -rf build/*.o build/shell