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
    char *input = malloc(1);
    if (!input) return NULL;
    int len = 0;
    char ch;

    while ((ch = getchar()) != '\n') 
    {
        char *tmp = realloc(input, len + 2);
        if (!tmp) {
            free(input);
            return NULL;
        }
        input = tmp;
        input[len++] = ch;
    }
    input[len] = '\0';
    return input;
}

char **splitArguments(char *input) 
{
    // Return NULL if input is empty
    if (!input) 
    {
        return NULL;
    }

    // Define delimiters (spaces, tabs, newlines)
    const char *delimiters = " \t\n";
    char **args = NULL;
    int count = 0;

    // Tokenize the input string based on delimiters
    char *token = strtok(input, delimiters);
    while (token) {
        // Resize memory to store the new token
        char **temp = realloc(args, sizeof(char*) * (count + 2));
        if (!temp) 
        {
            free(args);
            return NULL;
        }
        args = temp;
        args[count] = token;
        count++;
        token = strtok(NULL, delimiters);
    }

    // Null-terminate the arguments array
    if (args) 
    {
        args[count] = NULL;
    }

    return args;
}

void getLocation() 
{
    // Get the username
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

int isExitCommand(const char *input) 
{
    // Skip leading spaces
    while (isspace((unsigned char)*input)) {
        input++;
    }

    // Define the "exit" command string
    const char *exitStr = "exit";
    size_t len = strlen(exitStr);

    // Check if the input starts with "exit"
    if (strncmp(input, exitStr, len) != 0) {
        return 0;
    }

    // Check if the next character is a space or end of string (valid "exit" command)
    char c = input[len];
    if (c == '\0' || isspace((unsigned char)c)) {
        return 1;
    }

    return 0;
}

void logout(char *input)
{
    free(input);
    puts("Exiting shell... Goodbye!");
    exit(EXIT_SUCCESS);
}

void cd(char **args) 
{
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

void cp(char **arguments) 
{
    // Check if source or destination is missing
    if (!arguments[1] || !arguments[2]) {
        fprintf(stderr, "cp: missing source or destination\n");
        return;
    }

    // Open source file in binary read mode
    FILE *src = fopen(arguments[1], "rb");
    if (!src) {
        fprintf(stderr, "cp: cannot open file %s\n", arguments[1]);
        return;
    }

    // Open destination file in binary write mode (overwrite if exists, create if not)
    FILE *dst = fopen(arguments[2], "wb");
    if (!dst) {
        fclose(src);
        fprintf(stderr, "cp: cannot create/open file %s\n", arguments[2]);
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





void systemCall(char **arguments)
{
    puts("systemCall");
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork err");
        return;
    }

    if (pid == 0)
    {

        if (execvp(arguments[0], arguments) == -1)
        {
            exit(EXIT_FAILURE);
        }
    }
}
void mypipe(char **argv1, char **argv2)
{

    int fd[2];

    if (fork() == 0)
    {
        pipe(fd);
        if (fork() == 0)
        {

            close(STDOUT_FILENO);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            close(fd[0]);
            execvp(argv1[0], argv1);
        }

        close(STDIN_FILENO);
        dup(fd[0]);
        close(fd[1]);
        close(fd[0]);
        execvp(argv2[0], argv2);
    }
}
void move(char **args) {}
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
void get_dir()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("./")) == NULL)
    {
        perror("");
        return;
    }
    while ((ent = readdir(dir)) != NULL)
        printf("%s ", ent->d_name);
    puts("");
}
void delete(char **arguments)
{
    if (unlink(arguments[1]) != 0)
        printf("-myShell: delete: %s: No such file or directory\n", arguments[1]);
}
