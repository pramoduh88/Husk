#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

int main() {
    char cwd[PATH_MAX + 1];
    char input[1024];
    char* pch;
    char* list[1024];
    int count;
    int chdir_ret;
    pid_t pid;
    int status;
    char* cwd_ret;
    
    cwd_ret = getcwd(cwd, sizeof(cwd));
    if (!cwd_ret){
        perror("failed to get current working directory");
    }
    while (1) {
        printf("%s> ", cwd);
        fflush(stdout);
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = '\0';
        
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
        if (strcmp(list[0], "exit") == 0) return 0;
        if (strcmp(list[0], "cd") == 0){
            if (count > 1){
                fprintf(stderr, "cd: too many arguments\n");
                continue;
            }
            char* destination;
            if (!count){
                destination = getenv("HOME");
                if (!destination){
                    fprintf(stderr, "Warning: home environmental variable not set!\n");
                    continue;
                }
            }else{
                destination = list[1];
            }
            chdir_ret = chdir(destination);
            if (chdir_ret < 0){
                perror("change directory failed");
                continue;
            }
            cwd_ret = getcwd(cwd, sizeof(cwd));
            if (!cwd_ret){
                perror("failed to get current working directory");
                exit(1);
            }                
            continue;
        }
        list[count+1] = NULL;
        pid = fork();
        if (pid < 0){
            perror("creating a new process failed");
            exit(1);
        }
        if (pid == 0){
            execvp(list[0], list);
            perror("executing tool failed");
            _exit(1);
        }else{
            pid = waitpid(pid, &status, 0);
            if (pid < 0){
                perror("error in waiting for child to complete");
                exit(1);
            }
        }          
    }
    return 0;
}