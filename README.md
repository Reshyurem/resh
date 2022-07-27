# Assignment 2 - Part 1: Shell

## Introduction

The following code is a simple shell program that tries to emulate an actual shell and is written in C.

On running the makefile, the executable, `resh` will be created in the current directory and running it will produce start the execution of the shell.

For Asthetic reasons, my code has been colored to differentiate between input, working output, errors and the prompt

**The Code has been divided into the following files for Part 1:**

-   [`header.h`](header.h)
-   [`main.c`](main.c)
-   [`shell.c`](shell.c)
-   [`command_processing.c`](command_processing.c)
-   [`parse.c`](parse.c)
-   [`linked_lists.c`](linked_lists.c)
-   [`ls.c`](ls.c)
-   [`pinfo.c`](pinfo.c)
-   [`rawmode.c`](rawmode.c)
-   [`technical_functions.c`](technical_functions.c)
## Explanation of Code

### [header.h](header.c)

- Header file for the shell program
- Contains all required header files and function prototypes

### [main.c](main.c)
- contains the main function

### [shell.c](shell.c)
- contains the shell functions that are required for processing and operation of the shell
- `shell_setup()` is run to set the global variables and other constants that are required in the program
- `shell_prompt()` is run to print the shell prompt along with the relative path
- `shell()` is the actual function in which everything will run via a while loop
- `child_terminate()` is called by signal when the child dies by `signal()` and then reaps the child

### [command_processing.c](command_processing.c)
- contains the functions to choose which command to run, as well as the code for each user defined function
- `command_process()` chooses which function to run based on the input string
- `cd_command()` runs the cd command using `chdir()`
- `echo_command()` runs the echo command using `printf()`
- `pwd_command()` runs the pwd command using `cwd()`
- `repeat_command()` runs the echo command by parsing and combining and calling `command_process()` through a for loop
- `bg_command()` runs the command given using `execvp()` in the background after using a `fork()`
- `fg_command()` runs the command  given using `execvp()` in the foreground after using a `fork()` and `wait()`

### [parse.c](parse.c)
- contains the functions for the parsing the input so they can be easily processed
- `parse()` takes in a string, a delimiter string and an integer pointer. parse will then return a pointer to an array of strings where the string has been parsed and store the number of strings in the integer pointer. Input string is not modified
- `freemem()` can be called to free the memory that was allocated in `parse()` to be memory efficient

### [linked_lists.c](linked_lists.c)
- contains the linked list related functions used in history and storing background process names
- `add_bgp_node()` stores a newly created background process's name and pid in stack
- `get_process_name()` retrieves the background process's node from the stack using pid
- `remove_process()` deletes the node relating to the pid given
- `add_his_node()` stores the command entered as a node in a dequeue
- `history_command()` prints the commands in the dequeue with a limit of the number given or the length of the dequeue
- `history_storage()` writes the existing queue to a file so that it can be retrieved in another session
- `history_retrieval()` reads the file of previous commands so the old history is retained

### [ls.c](ls.c)
- `ls_command()` will go through all the elements in a directory and print it based on the flags assigned or just if a file, it will just print it based on the flags
- `list_form()` will print a file/directory given to it in the list format

### [pinfo.c](pinfo.c)
- contains the pinfo command code
- `pinfo_command()` will access the process files and print the process details based on the pid given to it

### [rawmode.c](rawmode.c)
- contains files that enable bufferless inputs to enable up arrow detection
- `enableRawMode()` enables bufferless input
- `disableRawMode()` disables bufferless input so other printf statements work properly
- `die()` prints the error related to enabling and disabling bufferless input
- All of this is thanks to Kannav's code which was sent in Moodle

### [technical_functions.c](technical_functions.c)
- contains functions that serve a specific purpose and are used in the code
- `remove_spaces()` removes the spaces in the input string so that no problem occurs while parsing
- `combination()` is the opposite of `parse()`as it turns an array of strings into a whole string again. Used in repeat due to the nature of execution
- `input_line()` returns the input that is also affected by the up arrow which requires special cases due to the input being bufferless
- `backspace()` removes a character from the input stored in the input as well as from the screen as needed to emulate a normal backspace
- `rel_path()` edits the string given to it so that it is turned into the relative path to the home directory if required