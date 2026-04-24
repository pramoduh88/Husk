#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

int main() {
    char cwd[PATH_MAX + 1];
    char input[1024];
    char* command_list[1024];
    int chdir_ret;
    pid_t pid;
    int status;
    char* cwd_ret;
    int ch;
    cwd_ret = getcwd(cwd, sizeof(cwd));
    if (!cwd_ret){
        perror("failed to get current working directory");
    }
    while (1) {
        printf("%s> ", cwd);
        fflush(stdout);
        
        int character_count = 0;
        int command_count = 0;
        int in_word = 0;
        int in_quotes = 0;
        while ((ch = getchar()) != '\n') {
            if (ch == EOF){
                return 0;
            }
            if (ch =='"'){
                in_quotes = 1 - in_quotes;
                continue;
            }
            if (ch == ' ' && !in_quotes){
                input[character_count] = '\0';
                in_word = 0;
            }else{
                input[character_count] = ch;
                if (!in_word){
                    command_list[command_count] = input + character_count;
                    command_count++;
                    in_word = 1;
                } 
            }
            character_count++; 
        }
        if (command_count == 0){
            continue;
        }
        input[character_count] = '\0';
        if (strcmp(command_list[0], "exit") == 0) return 0;
        if (strcmp(command_list[0], "cd") == 0){
            if (command_count > 2){
                fprintf(stderr, "cd: too many arguments\n");
                continue;
            }
            char* destination;
            if (command_count == 1){
                destination = getenv("HOME");
                if (!destination){
                    fprintf(stderr, "Warning: home environmental variable not set!\n");
                    continue;
                }
            }else{
                destination = command_list[1];
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
        command_list[command_count] = NULL;
        pid = fork();
        if (pid < 0){
            perror("creating a new process failed");
            exit(1);
        }
        if (pid == 0){
            execvp(command_list[0], command_list);
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