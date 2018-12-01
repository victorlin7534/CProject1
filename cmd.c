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

void printSajictorPrompt () {printf("%s Sajictor Shell~$ ", call_getcwd());}

void sigintHandler(int sig_num) { 
    signal(SIGINT, sigintHandler); 
    printf("\nTo exit the Sajictor Shell, type 'exit'\n"); 
    printSajictorPrompt();
    fflush(stdout); 
} 

char * call_getcwd (){
    char * cwd = getcwd (0,0);
    if(!cwd)return "";
    else return cwd;
}

char ** parse( char * in, char * del){
  char *temp = calloc(strlen(in),sizeof(in));
  strcpy(temp,in);
  char **arr = calloc(6,sizeof(char *));
  for(int i=0;temp;){
    char* a = strsep(&temp,del);
    if(strcmp(a," ")&&strcmp(a,"")){
      arr[i]=a;
      i++;
    }
  }
  return arr;
}

void execute (char * str) {
  char ** arguments = parse(str," ");
  char loc[100];
  if (!strcmp(arguments[0], "exit")) exit(0);
  if (!strcmp(arguments[0], "cd")) {
    if(!arguments[1]){
      struct passwd *pwd = getpwuid(getuid());
      while(strcmp(pwd->pw_dir,getcwd(0,0))) chdir("..");
      return;
    }
    else strcpy(loc, arguments[1]);
    if(chdir(loc)) printf("path does not exist\n");
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

int has_out(char * line){ return strchr(line, '>')!=NULL;}

int has_in (char * line){ return strchr(line, '<')!=NULL;}

int has_pipe (char * line){ return strchr(line, '|')!=NULL;}

void exepipe(char * line){
/*  char ** tokens = parse(line,"|");
  int fds[2];
  pipe(fds);
  char input[100];
  int f = fork();
  if(f){
    close(fds[1]);
    execute(tokens[0]);
    read(fds[0],input,sizeof(input));
  }
  else{
    close(fds[0]);
    write(fds[1],);
  }*/
}

void exeout (char * line) {
  char ** tokens = parse(line,">");
  int out = open(parse(tokens[1]," ")[0], O_RDWR|O_CREAT|O_APPEND, 0600);
  int save_out = dup(fileno(stdout));
  dup2(out, fileno(stdout));
  execute(tokens[0]);
  fflush(stdout);
  fflush(stderr);
  dup2(save_out, fileno(stdout));
  close(save_out);
}

void exein (char * line) {
  char ** tokens = parse(line,"<");
  int in = open(parse(tokens[1]," ")[0], O_RDWR|O_CREAT|O_APPEND, 0600);
  int save_in = dup(fileno(stdin));
  dup2(in,fileno(stdin));
  execute(tokens[0]);
  fflush(stdin);
  fflush(stderr);
  dup2(save_in, fileno(stdin));
  close(save_in);
}

void feed(char * in){
  char ** args = parse(in,";");
  for(int i=0;args;i++){
    if(args[i]==NULL) return;
    if (has_out(args[i])) exeout(args[i]);
    else if (has_in(args[i])) exein(args[i]);
    else if (has_pipe(args[i])) exepipe(args[i]);
    else execute(args[i]);
  }
}
