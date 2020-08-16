#define PROMPTMAX 32
#define MAXARGS 10
#define MAX_ARG_SIZE 32
#define MAX_CANON 128
#define PATH_MAX 128


int shell(int argc, char **argv, char **envp);
void list(char *dir );
void printenv(char **envp);