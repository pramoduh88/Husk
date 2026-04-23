#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

int main() {
    char cwd[PATH_MAX + 1];
    char input[1024];
    char * pch;
    char* list[1024];
    int count;
    pid_t pid;
    int status;

    getcwd(cwd, sizeof(cwd));
    while (1) {
        printf("%s> ", cwd);
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "exit") == 0) return 0;
        count = -1;  
        pch = strtok (input," ");
        while(pch != NULL){
            count++;
            list[count] = pch;
            pch = strtok(NULL, " ");
        }
        if (count == -1){
            continue;
        }
        if (strcmp(list[0], "cd") == 0){
            if (count > 0){
                chdir(list[1]);
                getcwd(cwd, sizeof(cwd));
            }
            continue;
        }
        list[count+1] = NULL;
        pid = fork();
        if (pid == 0){
            execvp(list[0], list);
        }else{
            waitpid(pid, &status, 0);
        }          
    }
    return 0;
}