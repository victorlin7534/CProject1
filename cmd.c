#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h> 
#include "cmd.h"

//prints the basic bash prompt with current directory path
//arguments: n/a
//returns: n/a
void printSajictorPrompt(){printf("%s Sajictor Shell~$ ",call_getcwd());}

//checks if redirecting out is in user input
//arguments: char *line
//returns:  0 or any # != 0 
int has_out(char * line){ return strchr(line, '>')!=NULL;}

//checks if redirecting in is in user input
//arguments: char *line
//returns:  0 or any # != 0
int has_in (char * line){ return strchr(line, '<')!=NULL;}

//checks if piping is in user input
//arguments: char *line
//returns:  0 or any # != 0
int has_pipe (char * line){ return strchr(line, '|')!=NULL;}

//prevents user from ^C (CTRL-C); prints error message and requests proper exit
//arguments: int sig_num
//returns: n/a
void sigintHandler(int sig_num) { 
    signal(SIGINT, sigintHandler); 
    printf("\nTo exit the Sajictor Shell, type 'exit'\n"); 
    printSajictorPrompt();
    fflush(stdout); 
} 

//retrieves path of current working directory 
//arguments: n/a
//returns: char * path of current working directory
char * call_getcwd (){
    char * cwd = getcwd (0,0);
    if(!cwd)return "";
    else return cwd;
}

//separates user input into parts in order to be executed
//arguments: char * in, char * del
//returns: pointer of char * of strings separated by del in in
char ** parse( char * in, char * del){
  char *temp = calloc(strlen(in),sizeof(in));
  strcpy(temp,in);
  char **arr = calloc(6,sizeof(char *));
  for(int i=0;temp;){
    char* a = strsep(&temp,del);
    if(strcmp(a," ")&&strcmp(a,"")){arr[i]=a;i++;}//takes care of extraneous spaces
  }
  return arr;
}

//executes given a single bash command
//arguments: char *str
//returns: n/a
void execute (char * str) {
  char ** arguments = parse(str," ");
  char loc[100];
  if (!strcmp(arguments[0], "exit")) exit(0);//exits from shell
  if (!strcmp(arguments[0], "cd")) {//changes directory
    if(!arguments[1]){
      struct passwd *pwd = getpwuid(getuid());
      while(strcmp(pwd->pw_dir,getcwd(0,0))) chdir("..");//goes to root directory if no path given
      return;
    }
    else strcpy(loc, arguments[1]);
    if(chdir(loc)) printf("path does not exist\n");//error message if path does not exits
    return; 
  }
  if (!fork()){ 
    execvp(arguments[0], arguments);
    exit(1);
  } else {
    int status;
    wait(&status);
  }
}

//executes first command and redirects as input for second command
//arguments: char *line
//returns: n/a
void exepipe(char * line){
  char ** tokens = parse(line,"|");
  int fds[2],f;
  int save_in = dup(fileno(stdin));
  int save_out = dup(fileno(stdout));
  pipe(fds);
  if(!(f=fork())){
    close(fds[0]);
	dup2(fds[1], fileno(stdout));
	char ** arguments = parse(tokens[0]," ");
	execvp(arguments[0], arguments);
  }
  else{
  	dup2(save_out, fileno(stdout));
	close(save_out);
  	if(!(f=fork())){
	    close(fds[1]);
	    dup2(fds[0],fileno(stdin));
	    char ** arguments = parse(tokens[1]," ");
		execvp(arguments[0], arguments);
	}
	dup2(save_in, fileno(stdin));
	close(save_in);
  }
}

//executes the redirecting out to specified file
//arguments: char *line
//returns: n/a
void exeout (char * line) {
  char ** tokens = parse(line,">");
  int out = open(parse(tokens[1]," ")[0], O_RDWR|O_CREAT|O_APPEND, 0600);
  int save_out = dup(fileno(stdout));
  dup2(out, fileno(stdout));
  execute(tokens[0]);
  dup2(save_out, fileno(stdout));
  close(save_out);
}

//executes the redirecting in from specified file
//arguments: char *line
//returns: n/a
void exein (char * line) {
  char ** tokens = parse(line,"<");
  int in = open(parse(tokens[1]," ")[0], O_RDWR|O_CREAT|O_APPEND, 0600);
  int save_in = dup(fileno(stdin));
  dup2(in,fileno(stdin));
  execute(tokens[0]);
  dup2(save_in, fileno(stdin));
  close(save_in);
}

//processes raw user input
//arguments: char *in
//returns: n/a
void feed(char * in){
  char ** args = parse(in,";");//handles separate commands
  for(int i=0;args;i++){
    if(args[i]==NULL) return;
    if (has_out(args[i])) exeout(args[i]);//refirect out >
    else if (has_in(args[i])) exein(args[i]);//redirect in <
    else if (has_pipe(args[i])) exepipe(args[i]);//piping |
    else execute(args[i]);//normal
  }
}