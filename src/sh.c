#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"
#include "alias.h"
#include "history.h"
#include "commands.h"
#include "get_path.h"

int sh( int argc, char **argv, char **envp ) {

  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, commandpath[256], *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;
  struct alias *firstalias = NULL;
  struct alias *lastalias;
  struct history *firsthistory = NULL;

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
						  out with*/
  setenv("TEST", "testing", 1);

  if ((pwd = getcwd(NULL, PATH_MAX+1)) == NULL ) {
    perror("getcwd");
    exit(2);
  }

  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();

  // Allocate our strings.
  args[0] = (char*)calloc(32, sizeof(char));

  while (go) {
    /* print your prompt */
	printf("%s [%s]> ", prompt, pwd);

    	// Read in our input.
	if (fgets(commandline, PROMPTMAX, stdin) == NULL) {
		printf("\n");
		continue;
	}

	// Add to, or create, our history.
	struct history *temp = malloc(sizeof(history));
	temp->command = (char*)malloc(PROMPTMAX * sizeof(char));
	strcpy(temp->command, commandline);
	if (firsthistory == NULL) {
		firsthistory = temp;
	} else {
		temp->next = firsthistory;
		firsthistory = temp;
	}

	// Clear our argument array.
	memset(args[0], 32, '0');
	i = 1;
	while (i < MAXARGS) {
		free(args[i]);
		args[i] = NULL;
		i++;
	}

	// Parse our input.
	char input[MAX_CANON];
	strcpy(input, commandline);

	// Check for alias usage.
	alias *ch = firstalias;
	while (ch != NULL) {
		if (!strncmp(commandline, ch->replacement, strlen(ch->replacement)-1)) {
			strcpy(commandpath, &commandline[strlen(ch->replacement)]);
			strcpy(&commandline[strlen(ch->string)], command);
			snprintf(commandline, 256, "%s %s", ch->string, commandpath);
			sprintf(args[0], "%s", ch->string);
		}
		ch = ch->next;
	}// Check the first portion of the string for an alias and replace.

	command = strtok(commandline, " ");
	strcpy(args[0], command);
	arg = strtok(NULL, " ");
	i = 1;
	while (arg != NULL) {
		args[i] = (char*)malloc(32*sizeof(char));
		strncpy(args[i], arg, strlen(arg));
		arg = strtok(NULL, " ");
		i++;
	}
	if (i > 0) {
		args[i-1][strlen(args[i-1])-1] = '\0';
	}

    /* check for each built in command and implement */
	if (!strcmp(command, "exit\n") | !strcmp(command, "exit")) {
		return 0;
		// Make our free calls.
		alias *al = firstalias;
		alias *temp;
		history *hist = firsthistory;
		while (al != NULL) {
			temp = al->next;
			free(al->string);
			free(al->replacement);
			free(al);
			al = temp;
		}
		while (hist != NULL) {
			hist = firsthistory->next;
			free(firsthistory->command);
//			free(firsthistory);
			firsthistory = hist;
		}
		return 0;
	}// Return for our exit command.

	if (!strcmp(command, "which") | !strcmp(command, "which\n")) {
		if (args[1] == NULL) {
			continue;
		}
	//	if (args[2] != NULL) {
	//		continue;
	//	}
		pathlist = get_path();
		while (pathlist) {
			sprintf(commandpath, "%s/%s", pathlist->element, args[1]);
			if (access(commandpath, F_OK) == 0) {
				printf("%s\n", commandpath);
				break;
			}
			pathlist = pathlist->next;
		}
	}// Search for a command and print the first match we find.

	else if (!strcmp(command, "where") | !strcmp(command, "where\n")) {
		if (args[1] == NULL) {
			continue;
		}
		if (args[2] != NULL) {
			continue;
		}
		pathlist = get_path();
		while (pathlist) {
			sprintf(commandpath, "%s/%s", pathlist->element, args[1]);
			commandpath[strlen(commandpath)] = 0;
			if (access(commandpath, F_OK) == 0) {
				printf("%s\n", commandpath);
			}
			pathlist = pathlist->next;
		}
	}// Search for a command and print all matches.

	else if (!strcmp(command, "cd") | !strcmp(command, "cd\n")) {
		if (args[1] == NULL) {
			chdir(homedir);
			strcpy(owd, pwd);
		} else if (args[1][0] == '-') {
			chdir(owd);
			strcpy(owd, pwd);
		} else {
			strcpy(owd, pwd);
			sprintf(pwd, "%s/%s", pwd, args[1]);
			chdir(pwd);
		}
		pwd = getcwd(NULL, PATH_MAX + 1);
	}// Change directory, to home if blank, to last if '-', and to named if given.

	else if (!strcmp(command, "pwd\n") | !strcmp(command, "pwd")) {
		printf("%s\n", pwd);
	}// Print the working directory.

	else if (!strcmp(command, "list\n") | !strcmp(command, "list")) {
		DIR *dir;
		struct dirent *file;

		if (args[1] == NULL) {
			dir = opendir(pwd);
		} else {
			dir = opendir(args[1]);
		}
		while ((file = readdir(dir)) != NULL) {
			printf("%s\n", file->d_name);
		}
		closedir(dir);
	}// Lists files in the given directory, lists files in current if none given.

	else if (!strcmp(command, "pid\n")) {
		printf("%d\n", (int) getpid());
	}// Prints the shell's process ID.

	else if (!strcmp(command, "kill")) {
		int chk = 2;
		int val0 = atoi(args[1]);
		int val1;
		if (args[2] != NULL) {
			chk = 1;
			int val1 = atoi(args[2]);
		}
		if ((val0 < 0) && chk) {
			kill(val1, val0);
		} else {
			printf("Defaulting.");
			kill(val0, SIGTERM);
		}
	}// Sends a signal to the process with a given PID. Defaults to SIGTERM.

	else if (!strcmp(command, "prompt\n") | !strcmp(command, "prompt")) {
		if (args[1] == NULL) {
			printf("Input prompt prefix: ");
			fgets(prompt, PROMPTMAX, stdin);
			prompt[strcspn(prompt, "\n")] = 0;
		} else {
			sprintf(prompt, "%s", args[1]);
		}
	}// Adds a new prefix to the prompt, asks for one if none given.

	else if (!strcmp(command, "printenv\n") | !strcmp(command, "printenv")) {
		pathlist = get_path();
		if (args[1] == NULL) {
			while(pathlist) {
				printf("%s\n", pathlist->element);
				pathlist = pathlist->next;
			}
		} else if (args[2] != NULL) {
			printf("printenv: too many arguments.\n");
		} else {
			if (getenv(args[1]) != NULL) {
				strcpy(commandpath, getenv(args[1]));
				printf("%s\n", commandpath);
			}
		}
	}// Prints environment, narrow to one if given.

	else if (!strcmp(command, "alias") | !strcmp(command, "alias\n")) {
		if (args[1] == NULL) {
			alias *al = firstalias;
			while (al != NULL) {
				printf("%s, aliased as %s", al->string, al->replacement);
				al = al->next;
			}
		} else {
			if (firstalias == NULL) {
				firstalias = malloc(sizeof(alias));
				lastalias = firstalias;
				firstalias->string = (char*)malloc(64*sizeof(char));
				firstalias->replacement = (char*)malloc(64*sizeof(char));
			} else {
				lastalias->next = malloc(sizeof(alias));
				lastalias = lastalias->next;
				lastalias->string = (char*)malloc(64*sizeof(char));
				lastalias->replacement = (char*)malloc(64*sizeof(char));
				}
			memcpy(lastalias->replacement, &input[2+strlen(command)+strlen(args[1])], 
				 (1+strlen(input)-(strlen(command)+strlen(args[1]))));
			strncpy(lastalias->string, args[1],1+strlen(args[1]));
		}// Substring our input for aliasing and add a new alias with that value.
	} // Creates an alias and stores it. Prints alias list if none given.

	else if (!strcmp(command, "history\n") | !strcmp(command, "history")) {
		history *hist = firsthistory->next;
		int amount;
		i = 0;
		if (args[1] == NULL) {
			amount = 10;
		} else {
			amount = atoi(args[1]);
		}
		while ((i < amount) && (hist != NULL)) {
			printf("%s", hist->command);
			hist = hist->next;
			i++;
		}
	}// Prints out recent commands. Defaults to 10, can be given a different value.

	else if (!strcmp(command, "setenv\n") | !strcmp(command, "setenv")) {
		pathlist = get_path();
		if (args[1] == NULL) {
			while(pathlist) {
				printf("%s\n", pathlist->element);
				pathlist = pathlist->next;
			}
		} else if (args[3] != NULL) {
			printf("setenv: too many arguments.\n");
		} else if (args[2] != NULL) {
			if (!strcmp("HOME", args[1])){
				strcpy(homedir, args[2]);
			} else {
				setenv(args[1], args[2], 1);
			}
		} else {
			printf("%s", args[1]);
			setenv(args[1], "", 1);
		}
	}// Sets environment variables, or reads them if none given. Rejects more than two args.

     /*  else  program to exec */
	else { //(which(command) | !strncmp(command, "/", 1) | !strncmp(command, "./", 2) | !strncmp(command, "../", 3)) { 
		if (args[1] == NULL) {
			command[strlen(command)-1] = '\0';
		}
		pathlist = get_path();
		which(command, pathlist);
		if (!access(command, X_OK)) {
			pid_t childid = fork();
			int status;
			if (childid == 0) {
				chdir(pwd);
				execv(command, args);
				return(0);
			}
			int wait = waitpid(childid, &status, 0);
		} else {
			fprintf(stderr, "%s: Command not found.\n", args[0]);
		}// Serach for an executable, and run it if it exists.
  	}
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist ) {
	char temp[256];
	while (pathlist) {
		sprintf(temp, "%s/%s", pathlist->element, command);
		if (access(temp, X_OK) == 0) {
			sprintf(command, "%s", temp);
			return NULL;
//			return(command);
		}
		pathlist = pathlist->next;
	}
	return NULL;
} /* which() */

char *where(char *command, struct pathelement *pathlist ) {
  /* similarly loop through finding all locations of command */
} /* where() */

void list ( char *dir ) {
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */
