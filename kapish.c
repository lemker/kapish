#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <signal.h>
#include "split_line.h"

/**
 * Set debug level
 * 0    No debug output
 * 1    Debug output
 */
int debug = 0;

/**
 * Debug writer method for handy output
 * @param input      Input string
 */
void debugWrite(char* input) {

    if (debug == 1) {
        printf("%s", input);
    }
}

/*
 * Execute a non built-in command
 */
int execute_unfamiliar(char** args) {

    // Set the command as the first input token
    char* cmd = args[0];

    debugWrite("\nForking child process...");
    pid_t child_pid = fork();
    debugWrite("Done!");

    // Execute built-in command
    if(child_pid == 0) {
        debugWrite("\nExecuting built-in command...");
        if(execvp(cmd, &args[0]) == -1) {
            printf("\nERROR");

            // Kill current child process
            exit(EXIT_FAILURE);
        }
        else {
            debugWrite("Done!");
        }
    }

        // Kill child if status is 1
    else {
        debugWrite("\nKilling child process...");
        return waitpid(child_pid, NULL, 0);
    }
    return 1;
}

/**
 * Execute shell arguments
 * @param args       Shell arguments
 * @return
 */
int execute_args(char** args) {

    // Catch if there is no input
    if(args[0] == NULL) {
        debugWrite("\nNothing was entered!");
        return 1;
    }

    // Catch exit command
    if(strcmp("exit", args[0]) == 0 ) {
        debugWrite("\nReceived exit command!");
        return 0;
    }

    // Catch cd command
    else if(strcmp("cd", args[0] ) == 0 ) {
        debugWrite("\nReceived cd command...");

        // cd command with no args
        if(args[1] == NULL) {
            debugWrite("\ncd command has no arguments, defaulting to home directory...");
            chdir(getenv("HOME"));
            return 1;
        }

        // cd command with args
        else {
            // Catch invalid destinations
            if(chdir(args[1]) == -1) {
                debugWrite("\nReceived invalid input for cd!");
                return 1;
            }

            // Run command if args is a valid destination
            else {
                debugWrite("\ncd command has arguments, executing...");
                chdir(args[1]);
                debugWrite("Done!");
                return 1;
            }
        }
    }

    // Catch setenv command
    else if(strcmp("setenv", args[0]) == 0) {

        // Catch if setenv has no var argument
        if(args[1] == NULL) {
            debugWrite("\nsetenv command is missing var argument!");
            return 1;
        }

        // Catch if setenv has more than 2 arguments
        if(args[3] != NULL) {
            debugWrite("\nsetenv command has too many inputs!");
            return 1;
        }

        // Execute setenv command with overwrite
        else {
            debugWrite("\nSetting new environment variable...");
            setenv(args[1], args[2], 1);
            debugWrite("Done!");
            return 1;
        }
    }

    // Catch unsetenv command
    else if(strcmp("unsetenv", args[0]) == 0) {

        // Catch if unsetenv has no var argument
        if(args[1] == NULL) {
            debugWrite("\nunsetenv is missing var argument!");
            return 1;
        }

        // Catch if unsetenv has too many arguments
        if(args[2] != NULL) {
            debugWrite("/nunsetenv has too many arguments!");
            return 1;
        }

        // Execute unsetenv command
        else {
            debugWrite("\nUnsetting environment variable...");
            unsetenv(args[1]);
            debugWrite("Done!");
            return 1;
        }
    }

    // Catch non-built-in commands
    else {
        debugWrite("\nCommand is not built-in!");
        return execute_unfamiliar(args);
    }
}

/**
 * Main program loop
 */
void program_loop(void) {

    // Initialize run status variable
    int run;

    // Find HOME environment variable
    char* home_tmp = getenv("HOME");

    // Allocate space in memory for HOME environment variable
    char* home_dir = (char *) malloc(128);

    // Copy data into home_dir
    strcpy(home_dir, home_tmp);

    // Concatenate HOME directory and file to get file path
    char* file_name = strcat(home_dir, "/.kapishrc");

    // Check if the file exists
    if(access(file_name, F_OK) != -1) {

        // Check if the file is readable
        if(access(file_name, R_OK) != -1) {

            // Get file
            FILE* file = fopen(file_name, "r");

            // Initialize line size to 512 bytes
            char line[512];

            // Work through each line in the file
            while (fgets(line, sizeof(line), file)) {

                // Print read line
                printf("\n? %s", line);

                // Execute line
                debugWrite("\nTokenizing file input line...");
                char **args = split_line(line);
                debugWrite("Done!");

                // Execute arguments and set program run status
                debugWrite("\nExecuting arguments...");
                execute_args(args);
                debugWrite("\nArguments executed!");

                // Free memory allocation for args variable
                free(args);
            }
            // Close file reader
            fclose(file);
        }else {
            printf("\n.kapishrc file is not readable at location: %s", file_name);
        }
    }else {
        printf("\n.kapishrc file does not exist at location: %s", file_name);
    }

    // Free memory allocation for home_tmp variable
    free(home_dir);

    // Run main shell loop
    do {

        // Initialize args variable
        char** args;

        // Initialize reply variable to 512 bytes
        char reply[512];

        // Initiate new line
        printf("\n? ");

        // Get shell input from user
        fgets(reply, 512, stdin);
        if(feof(stdin)) {
            // Catch CTRL-D EOF
            break;
        }
        else {
            debugWrite("\nShell got reply!");

            // Tokenize reply
            debugWrite("\nTokenizing reply...");
            args = split_line(reply);
            debugWrite("Done!");

            // Execute arguments and set program run status
            debugWrite("\nExecuting arguments...");
            run = execute_args(args);
            debugWrite("\nArguments executed!");

            // Free memory allocation for args variable
            free(args);
        }
    }while(run);
}

/*
 * sigintHandler taken from:
 * https://www.geeksforgeeks.org/write-a-c-program-that-doesnt-terminate-when-ctrlc-is-pressed/
 */
void sigintHandler(int sig_num) {
    signal(SIGINT, sigintHandler);
    debugWrite("CTRL-C input blocked!");
    fflush(stdout);
}

/**
 * Main method
 * @return           Program status
 */
int main() {
    debugWrite("\nStarting program...");

    // Block CTRL-C in parent process
    signal(SIGINT, sigintHandler);

    debugWrite("\nStarting main program loop...");
    program_loop();
    debugWrite("\nStepped out of main program loop!");
    debugWrite("\nExiting!");
    return 0;
}