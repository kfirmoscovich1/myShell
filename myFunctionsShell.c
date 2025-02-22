#include "myFunctionsShell.h"
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>   
#include <sys/types.h> 
#include <sys/wait.h> 
#include <pwd.h>  
#include <dirent.h>  
#include <fcntl.h>  
#include <sys/stat.h> 
#include <errno.h>


char *getInputFromUser()
{
    char *input = malloc(1);  // allocate single memory location to input
    if (!input) return NULL; // check ia allocation works
    int len = 0;
    char ch;

    while ((ch = getchar()) != '\n') { // start ti get chars from the user
        char *tmp = realloc(input, len + 2); // allocate memory to tmp base on input size
        if (!tmp) { 
            free(input); // if tmp memory allocation fails, free input and return NULL
            return NULL;
        }
        input = tmp; // point input to the tmp memory allocation
        input[len++] = ch; // add the new captured char into the input array of chars and increase the length counter
    }
    input[len] = '\0'; // after user ended the input, we add the \0 to indicated end of the chars list
    return input;
}

char **splitArguments(char *str) {
    // Return NULL if input is empty
    if (!str) {
        return NULL;
    }

    
    const char *delimiters = " \t\n";  // Define delimiters (spaces, tabs or newlines)
    char **args = NULL;
    int count = 0;

    // Tokenize the input string based on delimiters
    char *token = strtok(str, delimiters);
    while (token) {
        // Resize memory to store the new token
        char **temp = realloc(args, sizeof(char*) * (count + 2));
        if (!temp) {
            free(args);
            return NULL;
        }
        args = temp;
        args[count++] = token; // put the token into args list
        token = strtok(NULL, delimiters); // gets the next token.
    }

    // Null-terminate the arguments array
    if (args) {
        args[count] = NULL;
    }

    return args;
}


void getLocation() {
    // Get the username
    // struct passwd {
    //     char   *pw_name;   // Username
    //     char   *pw_passwd; // Encrypted password (not always available)
    //     uid_t   pw_uid;    // User ID (UID)
    //     gid_t   pw_gid;    // Group ID (GID)
    //     char   *pw_gecos;  // Full name or additional user info
    //     char   *pw_dir;    // Home directory
    //     char   *pw_shell;  // Default shell (e.g., /bin/bash, /bin/sh)
    // };
    
    struct passwd *pw = getpwuid(getuid());
    char *username = pw ? pw->pw_name : "unknown_user";

    // Get the hostname
    char hostname[128];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strcpy(hostname, "unknown_host");
    }

    // Get the current working directory
    char cwd[256];
    if (!getcwd(cwd, sizeof(cwd))) {
        strcpy(cwd, "unknown_path");
    }

    // Print the formatted location (with colors)
    printf("\033[1;32m%s\033[0m", username); // Green username
    printf("@");
    printf("\033[1;32m%s\033[0m", hostname); // Green hostname
    printf(":");
    printf("\033[34m%s\033[0m", cwd); // Blue current directory
    printf("$ ");
    
    // Ensure output is displayed immediately
    fflush(stdout);
}

int isExitCommand(char **args) {
    // Check only one argument was given
     int num_of_args = get_arg_num (args) ;
     if ( (num_of_args ==1) && (strcmp(args[0], "exit")) ==0) {
         return 0;
     } 
 
     return 1; 
     
 }

 void logout(char *str)
 {
     free(str);
     puts("Exiting shell... Goodbye!");
     exit(EXIT_SUCCESS);
 }

 void cd(char **args) {
    int num = 0;
    
    // Check if no directory is provided
    if (!args[1]) {
        fprintf(stderr, "cd: missing directory\n");
        return; 
    }

    // Allocate memory for the path (max length 1023)
    char *path = malloc(1023 * sizeof(char)); 
    path[0] = '\0';

    // Generate the full path from all arguments
    while (args[num] != NULL) {
        // Skip the "cd" command itself
        if (num > 0) {

            // Add a space between arguments
            if (strlen(path) > 1) strcat(path, " ");
            strcat(path, args[num]); 
        } 
        num++; 
    }

    // Remove surrounding quotes if they exist
    int len = strlen(path);
    if (len > 1 && path[0] == '"' && path[len - 1] == '"') {
        memmove(path, path + 1, len - 1);  // Shift left to remove first quote
        path[len - 2] = '\0';  // Remove last quote
    }
     
    // Change the directory to the specified path
    int val = chdir(path);
    if (val != 0) {
        // Print error message if `chdir` fails
        printf("Error %s:", strerror(errno)); 
    } 

    // Free allocated memory
    free(path); 
}

void cp(char **args) {
    // Check if source or destination is missing
    if (!args[1] || !args[2]) {
        fprintf(stderr, "cp: missing source or destination\n");
        return;
    }

    // Open source file in binary read mode
    FILE *src = fopen(args[1], "rb");
    if (!src) {
        fprintf(stderr, "cp: cannot open file %s\n", args[1]);
        return;
    }

    // Open destination file in binary write mode (overwrite if exists, create if not)
    FILE *dst = fopen(args[2], "wb");
    if (!dst) {
        fclose(src);
        fprintf(stderr, "cp: cannot create/open file %s\n", args[2]);
        return;
    }

    // Buffer for reading and copying data
    char buf[4096];
    size_t n;
    
    // Read from source and write to destination until done
    while ((n = fread(buf, 1, sizeof(buf), src)) > 0) {
        fwrite(buf, 1, n, dst);
    }

    // Close files to free resources and save data
    fclose(src);
    fclose(dst);
}

void get_dir() {
    // Open the current directory
    DIR *dir = opendir("./");
    if (!dir) {
        // Print error if directory cannot be opened
        perror("opendir");
        return;
    }

    // Read and print each entry in the directory
    struct dirent *ent;
    while ((ent = readdir(dir))) {
        printf("%s ", ent->d_name);
    }

    puts("");

    // Close the directory
    closedir(dir);
}

void delete(char **str) {
    // Check if a file path is provided
    if (!str[1]) {
        printf("delete: missing file name\n");
        return;
    }

    // Handle paths enclosed in double quotes
    char *path = str[1];
    size_t len = strlen(path);
    if (len > 1 && path[0] == '"' && path[len - 1] == '"') {
        path[len - 1] = '\0';  // Remove trailing quote
        path++; // Move pointer to exclude the first quote
    }

    // unlink(path) removes the file. Returns 0 on success, -1 on failure. If in use, deletion occurs after all processes close it.
    if (unlink(path) != 0) {
        printf("delete: cannot remove '%s'\n", path);
    } else {
        printf("delete: file '%s' successfully deleted\n", path);
    }
}

void systemCall(char **arguments) {
    // Create a new fork to verify that the command is not correct.
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        if (execvp(arguments[0], arguments) == -1) {
            perror("Error");
            exit(EXIT_FAILURE); 
        }
    }

}

void mypipe(char **argv1, char **argv2) {
    // Create two pipes, input and output.
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0) {
        close(fd[0]); // close the input pipe, it not required
        dup2(fd[1], STDOUT_FILENO); // redirect the standard output from the display to FD1, the next command input 
        close(fd[1]); // close the FD1 as it already used 
        execvp(argv1[0], argv1); // exeute the first function 
        perror("execvp argv1");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        close(fd[1]); 
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execvp(argv2[0], argv2);
        perror("execvp argv2");
        exit(EXIT_FAILURE);
    }

    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

void move(char **args) {
    // Check if source and destination are provided
    if (!args[1] || !args[2]) {
        printf("mv: missing source or destination\n");
        return;
    }

    // Handle paths enclosed in double quotes
    char *source = args[1];
    char *destination = args[2];

    size_t len_src = strlen(source);
    size_t len_dst = strlen(destination);

    if (len_src > 1 && source[0] == '"' && source[len_src - 1] == '"') {
        source[len_src - 1] = '\0'; // Remove trailing quote
        source++; // Move pointer to exclude the first quote
    }

    if (len_dst > 1 && destination[0] == '"' && destination[len_dst - 1] == '"') {
        destination[len_dst - 1] = '\0'; // Remove trailing quote
        destination++; // Move pointer to exclude the first quote
    }

    // Try renaming the file
    if (rename(source, destination) != 0) {
        printf("mv: cannot move '%s' to '%s', trying copy & delete...\n", source, destination);
        
        // Prepare arguments for cp
        char *cp_args[] = {"cp", source, destination, NULL};
        cp(cp_args);

        // Attempt to delete the source file
        if (unlink(source) != 0) {
            printf("mv: failed to remove '%s'\n", source);
        }
    }
}




void echoppend(char **args) {}
void echowrite(char **args) {}
void _read(char **args) {}
void wordCount(char **args) {}
void echo(char **arguments)
{
    while (*(++arguments))
        printf("%s ", *arguments);

    puts("");
}

