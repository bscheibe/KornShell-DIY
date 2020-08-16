typedef struct pathelement
{
  char* element;
  struct pathelement* next;
} pathelement;

typedef struct history
{
	char* command;
	struct history* next;
} history;

typedef struct alias
{
	char* string;
	char* replacement;
	struct alias* next;
} alias;