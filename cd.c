#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int cd(char *rawpath) {
	struct pass *p = getpwuid(getuid());
	const char *home = p->pw_dir;
	char path[128];
	char cwd[128];
	strcpy(path, rawpath);

	if (path[0] == '\0') {
		chdir(home);
	} else if (path[0] != '\0') {
		getcwd(cwd, sizeof(cwd));
		sprintf(cwd, "%s/%s", cwd, path);
		chdir(cwd);
	} else { 
		chdir(path);
	}
