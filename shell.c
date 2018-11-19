#include <stdlib.h>
#include <stdio.h>

int main(){
  char *in = malloc(200);
  while(1){
    fgets(in,200,stdin);
    in [strlen(in )-1]=0;
    parse_args();
  }
}


char ** parse_args( char * line ){
  char **container = malloc(64);
  char *mark = line;
  int x=0;
  while(container[x] =  strsep(&mark," "))
    x++;
  return container;
} 
