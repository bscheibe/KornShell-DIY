# KornShell-DIY

A personal implementation of the KornShell.

## Feature list
```
cd : change directory. Pass no argument to change to the home directory. Pass '-' to change to previously visited location. Pass a relative path to set working directory.
alias: creates an alias and stores it. Prints alias list if none given.
exit: exits the shell.
which: searches path for a command, and prints the location of the first match.
where: searches path for a command, and prints the location of every match.
pwd: print the current working directory.
list: equivalent to 'ls'. Prints contents of the passed directory, prints files in current working directory if none given.
pid: prints the shell's process ID.
kill: sends a signal to the process with a given PID. Defaults to SIGTERM.
prompt: adds a prefix to the prompt with a given arg, asks for one if none given.
printenv: prints environment. User may narrow this by passing a location to search.
setenv: sets environment variables, or reads them if none given.
exec: searches for an executable, and runs it if it exists.
empty case: escapes and prints the prompt on a new line.
```

## TODO
```
Refactor bloated main method.
Remove global variables.
Refactor commands to a separate file.
Plug memory leaks, if any.
```
