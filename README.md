# Custom Shell Implementation

## Overview
This project is a custom shell implementation in C that provides basic shell functionalities such as executing commands, handling input and output redirection, managing processes, and supporting pipes. The shell is designed to mimic a UNIX-like command-line interface.

## Features
- **Command Execution**: Runs system commands and built-in commands.
- **Custom Commands**:
  - `cd` - Change directory.
  - `echo` - Print text or redirect output to a file.
  - `cp` - Copy files.
  - `mv` - Move or rename files.
  - `delete` - Delete files.
  - `dir` - List files in the current directory.
  - `read` - Read file contents.
  - `wc` - Count words or lines in a file.
- **Pipelines (`|`)**: Supports piping between commands.
- **Custom Prompt**: Displays the username, hostname, and current directory.
- **Graceful Exit**: The shell exits when `exit` is entered.

## File Structure
- `myShell.c` - The main program that runs the shell loop and processes user commands.
- `myFunctionsShell.c` - Implements the helper functions for command execution.
- `myShell.h` - Header file for shell-related functions.
- `myFunctionsShell.h` - Header file for helper functions.
- `Makefile` - Compilation script for building the shell.

## Compilation
To compile the shell, run:
```sh
make
```
This will generate an executable file named `myShell`.

## Usage
To start the shell, run:
```sh
./myShell
```
Once started, the shell will display a prompt where you can enter commands.

## Example Commands
```sh
cd /home/user/Documents   # Change directory
ls -l                     # List files
cp file1.txt file2.txt    # Copy a file
mv file2.txt archive/     # Move a file
delete file1.txt          # Delete a file
wc -l file2.txt           # Count lines in a file
echo "Hello, World!" > output.txt  # Write to a file
```

## Exit
To exit the shell, type:
```sh
exit
```

## Author
Kfir Moscovich

## License
This project is open-source and can be modified and distributed under the MIT License.
