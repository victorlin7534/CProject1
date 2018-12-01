#include <string.h>
#include <stdio.h>
#include <signal.h> 
#include "cmd.h"

int main(){
  	signal(SIGINT, sigintHandler); 
    while (1) {
        printSajictorPrompt();
        char str[100];
        fgets(str, 100, stdin);
        strtok(str, "\n");
        feed(str);
    }
    return 0;
}