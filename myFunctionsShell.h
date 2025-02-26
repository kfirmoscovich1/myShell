#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <pwd.h>
#include <sys/types.h>
#include <errno.h>

#define SIZE_BUFF 256

/**
 * @brief Reads a line of input from the user until '\n'.
 *
 * Allocates memory dynamically to store the input. The caller is responsible
 * for freeing the allocated memory when no longer needed.
 *
 * @return A pointer to the dynamically allocated string containing the user input.
 *         Returns NULL if memory allocation fails.
 */
char *getInputFromUser();

/**
 * @brief Splits a string into individual tokens based on whitespace.
 *
 * This function takes a string as input and tokenizes it based on whitespace
 * characters (space, tab, newline). It dynamically allocates memory for an
 * array of strings to store the tokens. The caller is responsible for freeing
 * the memory allocated for the array and its elements when it's no longer needed.
 *
 * @param str A pointer to the string to be split.
 *
 * @return A dynamically allocated array of strings (char**) containing the tokens.
 *         The last element of the array is NULL. Returns NULL if str is NULL
 *         or if memory allocation fails.
 *
 * @note The input string will be modified (tokenized) by this function.
 *       If preservation of the original string is required, make a copy before passing it.
 *
 * @warning The caller is responsible for freeing the memory allocated for
 *          the returned array and its elements using free().
 */
char **splitArguments(char *str);

/**
 * @brief Displays the current user@hostname:directory$ prompt in the terminal.
 *
 * Retrieves the username, hostname, and current working directory, then prints
 * them in a colored/bold format to resemble a standard shell prompt.
 *
 * @return void
 */
void getLocation();

/**
 * @brief Checks if the given input corresponds to an "exit" command.
 *
 * This function checks if the first argument in the array is "exit".
 *
 * @param args The array of argument tokens.
 * @return 1 if the string is an exit command, 0 otherwise.
 */
int isExitCommand(char **args);

/**
 * @brief Logs the user out of the custom shell, printing a message and exiting the process.
 *
 * Frees the input buffer before calling exit(EXIT_SUCCESS).
 *
 * @param str The user input buffer to be freed.
 * @return void
 */
void logout(char *str);

/**
 * @brief Prints the arguments to the screen, separated by spaces.
 *
 * Implements basic echo functionality: prints all tokens after "echo".
 *
 * @param args The array of argument tokens.
 */
void echo(char **args);

/**
 * @brief Changes the current working directory of the shell.
 *
 * If args[1] starts with a quote, the function may gather tokens until the
 * closing quote (implementation-dependent). Otherwise, calls chdir with args[1].
 *
 * @param args The array of argument tokens.
 */
void cd(char **args);

/**
 * @brief Copies a file from source to destination.
 *
 * Requires two arguments: source file path and destination file path.
 * Opens the source file in "rb" mode and the destination in "wb" mode, then copies.
 *
 * @param args The array of argument tokens.
 */
void cp(char **args);

/**
 * @brief Lists the contents of the current directory.
 *
 * Uses opendir() and readdir() to read filenames, printing them to stdout.
 *
 * @return void
 */
void get_dir();

/**
 * @brief Deletes (unlinks) a file from the filesystem.
 *
 * Expects at least one argument specifying the file path to delete.
 *
 * @param args The array of argument tokens.
 */
void delete(char **str);

/**
 * @brief Executes a command (external program) by creating a child process and calling execvp.
 *
 * The parent process will wait for the child to finish.
 *
 * @param args The array of argument tokens.
 */
void systemCall(char **args);

/**
 * @brief Creates a pipe between two commands and executes them.
 *
 * The output of argv1 is piped into the input of argv2, using fork() and dup2().
 *
 * @param argv1 Null-terminated array of argument tokens for the first command.
 * @param argv2 Null-terminated array of argument tokens for the second command.
 */
void mypipe(char **argv1, char **argv2);

/**
 * @brief Moves (renames) a file from one path to another.
 *
 * Implementation typically calls rename(). If rename fails due to cross-filesystem,
 * one might copy then delete.
 *
 * @param args The array of argument tokens.
 */
void move(char **args);

/**
 * @brief Appends text to a file, implementing "echo ... >> file".
 *
 * Finds ">>" in the arguments, gathers text before it, and appends the text
 * to the file specified after ">>".
 *
 * @param args The array of argument tokens.
 */
void echoppend(char **args);

/**
 * @brief Writes text to a file, implementing "echo ... > file".
 *
 * Finds ">" in the arguments, gathers text before it, and writes the text
 * to the file (overwriting existing contents).
 *
 * @param args The array of argument tokens.
 */
void echowrite(char **args);

/**
 * @brief Reads the entire content of a file and prints it to stdout.
 *
 * If the file does not exist or cannot be opened, nothing is printed.
 *
 * @param args The array of argument tokens.
 */
void _read(char **args);

/**
 * @brief Counts lines or words in a file, implementing wc -l or wc -w.
 *
 * - If args[1] == "-l", prints the number of lines in the file.
 * - If args[1] == "-w", prints the number of words in the file.
 * If the file does not exist or cannot be opened, no output is produced.
 *
 * @param args The array of argument tokens.
 */
void wordCount(char **args);

/**
 * @brief Counts the number of arguments in an array of strings.
 *
 * This function iterates over the provided argument array and counts
 * the number of elements until it encounters a NULL terminator.
 *
 * @param args The array of argument tokens, typically from a command-line input.
 *             The last element of the array should be NULL.
 *
 * @return The number of arguments in the array (excluding the NULL terminator).
 *
 * @note The function does not modify the input array.
 *
 * @warning If the input array is NULL, behavior is undefined.
 */
int get_arg_num(char **args);
