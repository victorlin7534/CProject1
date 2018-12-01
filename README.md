# SAJICTOR SHELL

## FEATURES
- redirection(<, >)
- piping(|)
- multiple same-line commands(;)
- root directory shortcut(cd )

## ATTEMPTED

## BUGS
- if one presses [tab] out of habit or any non-alphanumeric/symbol key and deletes what is added on, the shell will be unable to process the input. (e.g. emacs sh[tab] -> emacs sh -> nothing happens)
- leaving an application will result in the prompt printing twice

## FUNCTION HEADERS
prints the basic bash prompt with current directory path
arguments: n/a
//returns: n/a
void printSajictorPrompt()

//checks if redirecting out is in user input
//arguments: char *line
//returns:  0 or any # != 0 
int has_out(char * line)

//checks if redirecting in is in user input
//arguments: char *line
//returns:  0 or any # != 0
int has_in (char * line)

//checks if piping is in user input
//arguments: char *line
//returns:  0 or any # != 0
int has_pipe (char * line)

//prevents user from ^C (CTRL-C); prints error message and requests proper exit
//arguments: int sig_num
//returns: n/a
void sigintHandler(int sig_num)

//retrieves path of current working directory 
//arguments: n/a
//returns: char * path of current working directory
char * call_getcwd ()

//separates user input into parts in order to be executed
//arguments: char * in, char * del
//returns: pointer of char * of strings separated by del in in
char ** parse( char * in, char * del)

//executes given a single bash command
//arguments: char *str
//returns: n/a
void execute (char * str)

//executes first command and redirects as input for second command
//arguments: char *line
//returns: n/a
void exepipe(char * line)

//executes the redirecting out to specified file
//arguments: char *line
//returns: n/a
void exeout (char * line)

//executes the redirecting in from specified file
//arguments: char *line
//returns: n/a
void exein (char * line) 

//processes raw user input
//arguments: char *in
//returns: n/a
void feed(char * in)
