struct pathelement
{
  char *element;
  struct pathelement *next;
};

typedef struct history
{
	char* command;
	struct history *next;
} history;

typedef struct alias
{
	char* string;
	char* replacement;
	struct alias *next;
} alias;

struct pathelement *get_path();
int cd(char *rawpath);
