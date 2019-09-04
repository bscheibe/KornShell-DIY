#include <signal.h>
#include <stdio.h>
#include "shell.h"

void sig_handler(int signal); 

int main(int argc, char **argv, char **envp)
{

    // Signals to watch for.
    signal(SIGINT, sig_handler);
    signal(SIGCLD, sig_handler);
    signal(SIGTSTP, sig_handler);

    // Start our shell.
    return shell(argc, argv, envp);
}

void sig_handler(int signal)
{
	if (signal == SIGINT) {}
	else if (signal == SIGTSTP) {}
	else if (signal == SIGCLD) {}
}
