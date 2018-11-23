#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include "cmd.h"

char * call_getcwd (){
    char * cwd;
    cwd = getcwd (0, 0);
    if (! cwd) {
        return "";
    }
    else {
      return cwd;
      free (cwd);
    }
}

char ** parse( char * in, char * del){
  char *cpy = calloc(strlen(in),sizeof(in));
  strcpy(cpy,in);
  char **arr = calloc(6,sizeof(char *));
  for(int i=0;cpy;++i)arr[i]=strsep(&cpy,del);
  return arr;
}

void execute (char * str) {
  char ** arguments = NULL;
  arguments = parse(str," ");
  if (!strcmp(arguments[0], "exit")) {
    exit(0);
  }
  if (!strcmp(arguments[0], "cd")) {
    char str[200];
    strcpy(str, call_getcwd());
    strcpy(str, arguments[1]);
    if(chdir(str))
      printf("path does not exist\n");
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

void feed(char * in){
  char ** args = parse(in,";");
  for(int i=0;args;i++){
    if(args[i]==NULL)
      return;
    execute(args[i]);
  }
}