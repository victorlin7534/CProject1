#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include "cmd.h"

int main(){
    while (1) {
        printf("%s Sajictor Shell~$ ", call_getcwd());
        char str[100];
        fgets(str, 100, stdin);
        strtok(str, "\n");
        feed(str);
    }
    return 0;
}
