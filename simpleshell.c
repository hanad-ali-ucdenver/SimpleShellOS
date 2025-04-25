#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64

/**
 * Parses a command string into an array of arguments
 * @param command The command string to parse
 * @param args Array where parsed arguments will be stored
 */
void parse_command(char *command, char **args) {
    char *token;
    int i = 0;
    
    // Split command into tokens using space, tab, or newline as delimiters
    token = strtok(command, " \t\n");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token; // Store the token in the args array
        token = strtok(NULL, " \t\n"); // Get the next token
    }
    args[i] = NULL;  // Last argument must be NULL for execvp to work
}

/**
 * Handles built-in shell commands
 * @param args Array of command arguments
 * @return 0 if shell should exit, 1 if built-in command was executed, -1 if not a built-in command
 */
int execute_builtin(char **args) {
    if (args[0] == NULL) {
        return 1;  // If command is empty, don't do anything, continue
    }
    
    // Built-in Exit command to terminate shell
    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }
    
    // Built in CD command to change current directory
    if (strcmp(args[0], "cd") == 0) {
        // if no directory specified, print error
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

/**
 * Execute an external command using fork, exec, and wait
 * @param args Array of command arguments
 */
void execute_command(char **args) {
    pid_t pid; // Process ID
    int status;
    
    // Create a new process using fork
    pid = fork();
    if (pid == 0) {
        // Child process
        // Replace child process with the commands specified by args
        if (execvp(args[0], args) == -1) {
            perror("Command execution failed"); //if exec fails, print error
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        // Fork failed
        perror("Fork failed");
    } else {
        // Parent process
        // Wait for the child process to terminate
        do {
            waitpid(pid, &status, WUNTRACED); // command that tells parent to wait until child is stopped
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

/**
 * Main function - implements the shell's command loop
 */
int main() {
    char input[MAX_INPUT_SIZE]; // Buffer for command input
    char *args[MAX_ARGS]; // Array to store command arguments
    int status = 1; // Shell status (1=continue, 0=exit)
    
    // main loop
    while (status) {
        printf("> ");  // Display prompt
        
        // Read input
        if (!fgets(input, MAX_INPUT_SIZE, stdin)) {
            break;
        }
        
        // Parse command into argument tokens
        parse_command(input, args);
        
        // Execute command
        int result = execute_builtin(args);
        if (result == 0) {
            break;  // Exit shell
        } else if (result < 0) { // if result is less than zero, command is not a built-in command, execute as external
            execute_command(args);  
        }
    }
    
    return EXIT_SUCCESS;
}