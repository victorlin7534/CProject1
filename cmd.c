#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
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

int first_nondel_index(char ** str){
  for(int i=0;str;i++)
    if(strlen(str[i])>0) return i;
}

void execute (char * str) {
  char ** arguments = parse(str," ");
  char loc[100];
  if (!strcmp(arguments[0], "exit")) exit(0);
  if (!strcmp(arguments[0], "cd")) {
    if(!strlen(arguments[1])||!arguments[1]) strcpy(str,"/");
    else strcpy(loc, arguments[1]);
    if(chdir(loc)) printf("path does not exist\n");
    return; 
  }
  if (!fork()){ 
      int p = first_nondel_index(arguments);
      execvp(arguments[p], arguments+p);
      exit(1);
  } else {
    int status;
    wait(&status);
  }
}

void feed(char * in){
  char ** args = parse(in,";");
  for(int i=0;args;i++){
    if(args[i]==NULL) return;
    execute(args[i]);
  }
}