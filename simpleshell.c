#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

// Function to parse command line into arguments
void parse_command(char *command, char **args) {
    char *token;
    int i = 0;
    
    token = strtok(command, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;  // Last argument must be NULL for execvp
}

// Function to execute built-in commands
int execute_builtin(char **args) {
    if (args[0] == NULL) {
        return 1;  // Empty command
    }
    
    // Exit command
    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }
    
    // CD command
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Expected argument to \"cd\"\n");
        } else {
            if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
        }
        return 1;
    }
    
    return -1;  // Not a builtin command
}

// Function to execute external commands
void execute_command(char **args) {
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Command execution failed");
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        // Fork failed
        perror("Fork failed");
    } else {
        // Parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    int status = 1;
    
    while (status) {
        printf("> ");  // Display prompt
        
        // Read input
        if (!fgets(input, MAX_INPUT_SIZE, stdin)) {
            break;
        }
        
        // Parse command
        parse_command(input, args);
        
        // Execute command
        int result = execute_builtin(args);
        if (result == 0) {
            break;  // Exit shell
        } else if (result < 0) {
            execute_command(args);  // Not a builtin command
        }
    }
    
    return EXIT_SUCCESS;
}