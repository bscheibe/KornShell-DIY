#include "sh.h"
#include <signal.h>
#include <stdio.h>

void sig_handler(int signal); 

int main(int argc, char **argv, char **envp) {
  /* put signal set up stuff here */
  signal(SIGINT, sig_handler);
  signal(SIGCLD, sig_handler);
  signal(SIGTSTP, sig_handler);

  return sh(argc, argv, envp);
}

void sig_handler(int signal) {
  /* define your signal handler */
  if (signal == SIGINT) {
//	printf("\n");
  }
  else if (signal == SIGTSTP) {
//	printf("\n");
  }
  else if (signal == SIGCLD) {
	printf("\n Caught signal %d.", signal);
  }
}
