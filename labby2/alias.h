typedef struct alias {
	char* string;
	char* replacement;
	struct alias *next;
} alias;
