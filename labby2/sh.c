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

int sh( int argc, char **argv, char **envp ) {

  char *prompt = calloc(PROMPTMAX, sizeof(char));
  char *commandline = calloc(MAX_CANON, sizeof(char));
  char *command, *arg, commandpath[64], *p, *pwd, *owd;
  char **args = calloc(MAXARGS, sizeof(char*));
  int uid, i, status, argsct, go = 1;
  struct passwd *password_entry;
  char *homedir;
  struct pathelement *pathlist;

  uid = getuid();
  password_entry = getpwuid(uid);               /* get passwd info */
  homedir = password_entry->pw_dir;		/* Home directory to start
						  out with*/

  if ((pwd = getcwd(NULL, PATH_MAX+1)) == NULL ) {
    perror("getcwd");
    exit(2);
  }

  owd = calloc(strlen(pwd) + 1, sizeof(char));
  memcpy(owd, pwd, strlen(pwd));
  prompt[0] = ' '; prompt[1] = '\0';

  /* Put PATH into a linked list */
  pathlist = get_path();
  i = 0;

  // Allocate our strings.
  while (i < MAXARGS) {
	args[i] = (char*)calloc(32, sizeof(char));
	i++;
  }

  while (go) {
    /* print your prompt */
	printf(" [%s]> ", pwd);

    	// Read in our input.
	if (fgets(commandline, PROMPTMAX, stdin) == NULL) {
		printf("\n");
		continue;
	}

	// Clear our argument array.
	i = 0;
	while (i < MAXARGS) {
		memset(args[i], 0, 32);
		i++;
	}

	// Parse our input.
	command = strtok(commandline, " ");
	arg = strtok(NULL, " ");
	i = 0;
	while (arg != NULL) {
		strncpy(args[i], arg, strlen(arg) - 1);
		arg = strtok(NULL, " ");
		i++;
	}

    /* check for each built in command and implement */
	if (!strcmp(command, "exit\n")) {
		return 0;
	}// Return for our exit command.

	if (!strcmp(command, "which")) {
		pathlist = get_path();
		while (pathlist) {
			sprintf(commandpath, "%s/%s", pathlist->element, args[0]);
			if (access(commandpath, F_OK) == 0) {
				printf("%s\n", commandpath);
				break;
			}
			pathlist = pathlist->next;
		}
	}// Search for a command and print the first match we find.

	else if (!strcmp(command, "where")) {
		pathlist = get_path();
		while (pathlist) {
			sprintf(commandpath, "%s/%s", pathlist->element, args[0]);
			if (access(commandpath, F_OK) == 0) {
				printf("%s\n", commandpath);
			}
			pathlist = pathlist->next;
		}
	}// Search for a command and print all matches.

	else if (!strcmp(command, "cd") | !strcmp(command, "cd\n")) {
		if (args[0][0] == '\0') {
			chdir(homedir);
			strcpy(owd, pwd);
		} else if (args[0][0] == '-') {
			chdir(owd);
			strcpy(owd, pwd);
		} else {
			strcpy(owd, pwd);
			sprintf(pwd, "%s/%s", pwd, args[0]);
			chdir(pwd);
		}
		pwd = getcwd(NULL, PATH_MAX + 1);
	}// Change directory, to home if blank, to last if '-', and to named if given.

	else if (!strcmp(command, "pwd\n")) {
		printf("%s\n", pwd);
	}// Print the working directory.

	else if (!strcmp(command, "list\n")) {
		//list(args);
	}
	else if (!strcmp(command, "pid\n")) {
		//pid(args);
	}
	else if (!strcmp(command, "kill\n")) {
		//kill(args);
	}
	else if (!strcmp(command, "prompt\n")) {
		//
	}
	else if (!strcmp(command, "printev\n")) {
		//
	}
	else if (!strcmp(command, "alias\n")) {
		//
	}
	else if (!strcmp(command, "history\n")) {
		//
	}
	else if (!strcmp(command, "setenv\n")) {
		//
	}

     /*  else  program to exec */
//	}
       /* find it */
       /* do fork(), execve() and waitpid() */
//
//    else
//    fprintf(stderr, "%s: Command not found.\n", args[0]);
// }
//	printf("\n");
  }
  return 0;
} /* sh() */

char *which(char *command, struct pathelement *pathlist ) {
   /* loop through pathlist until finding command and return it.  Return
   NULL when not found. */

} /* which() */

char *where(char *command, struct pathelement *pathlist ) {
  /* similarly loop through finding all locations of command */
} /* where() */

void list ( char *dir ) {
  /* see man page for opendir() and readdir() and print out filenames for
  the directory passed */
} /* list() */
