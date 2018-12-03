# SAJICTOR SHELL
## Invented by Sajed Nahian & Victor Lin

## FEATURES
- redirection(<, >)
- piping(|)
- multiple same-line commands(;)
- root directory shortcut(cd )
- signal handling

## ATTEMPTED
- [tab] autocomplete

## BUGS
- once the prompt moves on to the next line, the user cannot return to the previous line to edit
- leaving an application will result in the prompt printing twice
- running tr a-z A-Z < wholist > foo via makefile creates a foo file with normal text; however, when done manually, the file is in binary. (this is the case for ls > wholist)

## FUNCTION HEADERS
#### 1) void printSajictorPrompt()
	- prints the basic bash prompt with current directory path
	- arguments: n/a
	- returns: n/a

#### 2) int has_pipe (char * line)
	- checks if piping is in user input
	- arguments: char *line
	- returns:  0 or any # != 0

#### 3) void sigintHandler(int sig_num)
	- prevents user from ^C (CTRL-C); prints error message and requests proper exit
	- arguments: int sig_num
	- returns: n/a

#### 4) char * call_getcwd ()
	- retrieves path of current working directory 
	- arguments: n/a
	- returns: char * path of current working directory

#### 5) char ** parse( char * in, char * del)
	- separates user input into parts in order to be executed
	- arguments: char * in, char * del
	- returns: pointer of char * of strings separated by del in in

#### 6) void execute (char * str)
	- executes given a single bash command
	- arguments: char *str
	- returns: n/a

#### 7) void exepipe(char * line)
	- executes first command and redirects as input for second command
	- arguments: char *line
	- returns: n/a

#### 8) int has_redir(char * line)
	- checks if redirection is in user input
	- arguments: char *line
	- returns:  0 or any # != 0

#### 9) int * redir_order(char * line)
	- arguments: char *line
	- returns: array of ints indicating order of < or >

#### 10) void symexe(char * line)
	- executes redirection in both directions, however many times
	- arguments: char *line
	- returns: n/a

#### 11) void feed(char * in)
	- processes raw user input
	- arguments: char *
	- returns: n/a
