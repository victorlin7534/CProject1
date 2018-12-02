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
  int fds[2],f1,f2;
  int save_in = dup(0);
  int save_out = dup(1);
  pipe(fds);
  if(!(f1=fork())){
	dup2(fds[1], 1);
	close(fds[0]);
	char ** arguments = parse(tokens[0]," ");
	execvp(arguments[0], arguments);
  }
  if(!(f2=fork())){
	dup2(fds[0],0);
	close(fds[1]);
	char ** arguments = parse(tokens[1]," ");
	execvp(arguments[0], arguments);
  }
  close(fds[0]); close(fds[1]);
  waitpid(f1,NULL,0); waitpid(f2,NULL,0);
  dup2(save_out, 1); close(save_out);
  dup2(save_in, 0); close(save_in);
}

//checks if redirection is in user input
//arguments: char *line
//returns:  0 or any # != 0
int has_redir(char * line){return strchr(line, '>')!=NULL || strchr(line, '<')!=NULL;}

//arguments: char *line
//returns: array of ints indicating order of < or >
int * redir_order(char * line){
	int * arr = calloc(strlen(line),1);
	char temp[strlen(line)];
	strcpy(temp,line);
	int j=0,i=0;
	for(;i<strlen(line);i++){
		if(temp[i]=='>'){arr[j] = 1;j++;}// < = 2
		if(temp[i]=='<'){arr[j] = 2;j++;}// > = 1 
	}
	return arr;
}

//executes redirection in both directions, however many times
//arguments: char *line
//returns: n/a
void symexe(char * line){
	int * or = redir_order(line);
	char ** args = parse(line,"<>");
	int save_in = dup(0);
	int save_out = dup(1);
	for(int i=0;or[i]==1||or[i]==2;i++){
		int file = open(parse(args[i+1]," ")[0], O_RDWR|O_CREAT|O_APPEND, 0600);
		if(or[i]==1){// redirect out >
			dup2(file,1);
			execute(args[i]);
		}
		else if(or[i]==2){// redirect in <
			dup2(file,0);
			execute(args[i]);
		}
		dup2(save_out, 1);
		dup2(save_in, 0);
	}
	close(save_in);
	close(save_out);
}

//processes raw user input
//arguments: char *in
//returns: n/a
void feed(char * in){
  char ** args = parse(in,";");//handles separate commands
  for(int i=0;args;i++){
    if(args[i]==NULL) return;
    else if(has_redir(args[i])) symexe(args[i]);// redirection <>
    else if (has_pipe(args[i])) exepipe(args[i]);//piping |
    else execute(args[i]);//normal
  }
}