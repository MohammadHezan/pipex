/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hpipex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:48:32 by mhaizan           #+#    #+#             */
/*   Updated: 2026/02/06 13:54:46 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** PIPEX HEADER FILE
** =============================================================================
**
** This header file serves as the central declaration hub for the pipex project.
** It contains all necessary includes, macro definitions, and function prototypes
** that are shared across the source files.
**
** PURPOSE OF PIPEX:
** -----------------
** Pipex recreates the behavior of the Unix shell pipe operator. When you run:
**     < infile cmd1 | cmd2 > outfile
**
** The shell performs these operations:
** 1. Reads input from 'infile'
** 2. Passes it to 'cmd1' for processing
** 3. Pipes cmd1's output to 'cmd2'
** 4. Writes cmd2's final output to 'outfile'
**
** This project teaches fundamental Unix concepts:
** - Process creation with fork()
** - Inter-process communication with pipe()
** - File descriptor manipulation with dup2()
** - Program execution with execve()
** - Process synchronization with waitpid()
**
** HEADER ORGANIZATION:
** --------------------
** 1. Include guards (prevent double inclusion)
** 2. System library includes
** 3. Project-specific includes (libft)
** 4. Memory management function prototypes
** 5. PATH resolution function prototypes
** 6. File descriptor utility prototypes
** 7. Child process handler prototypes
**
** =============================================================================
*/

#ifndef HPIPEX_H
# define HPIPEX_H

/*
** =============================================================================
** SYSTEM LIBRARY INCLUDES
** =============================================================================
** Each library provides essential functionality for Unix system programming:
*/

/*
** <stdlib.h> - Standard Library
** -----------------------------
** Provides: malloc(), free(), exit(), EXIT_SUCCESS, EXIT_FAILURE
** Used for: Dynamic memory allocation and program termination
** Why needed: We allocate memory for command paths and argument arrays
*/
# include <stdlib.h>

/*
** <unistd.h> - Unix Standard Library
** -----------------------------------
** Provides: fork(), pipe(), dup2(), execve(), close(), read(), write(), access()
** Used for: Core Unix system calls for process and I/O operations
** Why needed: Essential for creating processes and redirecting file descriptors
**
** Key functions we use:
** - fork(): Creates a child process (exact copy of parent)
** - pipe(): Creates a unidirectional data channel
** - dup2(): Duplicates file descriptors for I/O redirection
** - execve(): Replaces current process with new program
** - close(): Releases file descriptors
** - access(): Checks file permissions (X_OK for executable)
*/
# include <unistd.h>

/*
** <fcntl.h> - File Control Operations
** ------------------------------------
** Provides: open(), O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
** Used for: Opening files with specific access modes and flags
** Why needed: Opening input/output files with appropriate permissions
**
** Flags we use:
** - O_RDONLY: Open file as read-only (for infile)
** - O_WRONLY: Open file as write-only (for outfile)
** - O_CREAT: Create file if it doesn't exist
** - O_TRUNC: Truncate file to zero length if it exists
*/
# include <fcntl.h>

/*
** <sys/wait.h> - Process Wait Operations
** ---------------------------------------
** Provides: waitpid(), WIFEXITED(), WEXITSTATUS(), WIFSIGNALED(), WTERMSIG()
** Used for: Parent process waiting for child processes to complete
** Why needed: Synchronization and retrieving child exit status
**
** Macros we use:
** - WIFEXITED(status): Returns true if child terminated normally
** - WEXITSTATUS(status): Returns exit code of normally terminated child
** - WIFSIGNALED(status): Returns true if child was terminated by signal
** - WTERMSIG(status): Returns signal number that caused termination
*/
# include <sys/wait.h>

/*
** <stdio.h> - Standard I/O Library
** ---------------------------------
** Provides: perror()
** Used for: Printing descriptive error messages to stderr
** Why needed: User-friendly error reporting (e.g., "file.txt: No such file")
**
** perror() automatically appends system error description based on errno
*/
# include <stdio.h>

/*
** =============================================================================
** PROJECT-SPECIFIC INCLUDES
** =============================================================================
*/

/*
** "../libft/libft.h" - 42 Standard Library
** -----------------------------------------
** Provides: ft_split(), ft_strjoin(), ft_strlen(), ft_strchr(), ft_strncmp()
** Used for: String manipulation and utility functions
** Why needed: Parsing commands, building paths, string operations
**
** Key functions we use from libft:
** - ft_split(): Splits string by delimiter (parsing commands)
** - ft_strjoin(): Concatenates two strings (building paths)
** - ft_strlen(): Gets string length (for write() calls)
** - ft_strchr(): Finds character in string (checking for '/')
** - ft_strncmp(): Compares n characters (finding PATH= in envp)
** - ft_strdup(): Duplicates a string (copying paths)
*/
# include "../libft/libft.h"

/*
** =============================================================================
** MEMORY MANAGEMENT FUNCTION PROTOTYPES
** =============================================================================
** Functions for safe memory deallocation to prevent memory leaks.
*/

/*
** ft_free_split - Frees a NULL-terminated array of strings
** ---------------------------------------------------------
** @param split: Array of strings created by ft_split()
**
** This function safely frees all memory allocated by ft_split().
** It iterates through each string in the array, frees it, then
** frees the array itself. Handles NULL input gracefully.
**
** Memory layout being freed:
**     split[0] -> "cmd"  [freed]
**     split[1] -> "-l"   [freed]
**     split[2] -> "-a"   [freed]
**     split[3] -> NULL
**     split -----------> [freed]
*/
void	ft_free_split(char **split);

/*
** =============================================================================
** PATH RESOLUTION FUNCTION PROTOTYPES
** =============================================================================
** Functions for finding executable commands in the system PATH.
**
** When you type 'ls' in a shell, it searches directories in $PATH
** to find '/bin/ls' or '/usr/bin/ls'. These functions replicate that.
*/

/*
** find_in_paths - Searches PATH directories for an executable command
** --------------------------------------------------------------------
** @param paths: Array of directory paths from $PATH (e.g., {"/bin", "/usr/bin"})
** @param cmd: Command name to search for (e.g., "ls")
** @return: Full path to executable (e.g., "/bin/ls") or NULL if not found
**
** Algorithm:
** 1. For each directory in paths:
**    a. Construct full path: directory + "/" + cmd
**    b. Check if file exists and is executable (access with X_OK)
**    c. If yes, return the full path (caller must free)
**    d. If no, free path and try next directory
** 2. If no match found in any directory, return NULL
**
** Example: find_in_paths({"/bin", "/usr/bin"}, "ls")
**          Tries: "/bin/ls" -> found! -> returns "/bin/ls"
*/
char	*find_in_paths(char **paths, char *cmd);

/*
** get_path - Resolves a command to its full executable path
** ----------------------------------------------------------
** @param cmd: Command to resolve (e.g., "ls" or "/bin/ls")
** @param envp: Environment variables array (contains PATH)
** @return: Full path to executable or NULL if not found
**
** This is the main entry point for path resolution.
**
** Logic:
** 1. If cmd already contains '/' (absolute or relative path):
**    - Return a copy of cmd (it's already a path)
** 2. Otherwise (bare command name):
**    - Find PATH variable in envp
**    - Split PATH by ':' to get individual directories
**    - Search each directory for the executable
**    - Return first match or NULL
**
** Example scenarios:
** - "/usr/bin/grep" -> returns copy of "/usr/bin/grep"
** - "./my_script" -> returns copy of "./my_script"
** - "grep" -> searches PATH, returns "/usr/bin/grep"
*/
char	*get_path(char *cmd, char **envp);

/*
** ft_execve - Executes a command in the current process
** ------------------------------------------------------
** @param cmd: NULL-terminated array of command and arguments
** @param envp: Environment variables to pass to new program
**
** This function wraps execve() with path resolution and error handling.
** If successful, the current process is replaced - this function never returns.
** If it fails (command not found, permission denied), it prints an error
** and exits with appropriate status code.
**
** Exit codes:
** - 127: Command not found (shell convention)
** - 1: Execution failed (permission denied, etc.)
**
** Note: On success, execve() replaces the entire process image,
** so the lines after execve() only run if it fails.
*/
void	ft_execve(char **cmd, char **envp);

/*
** =============================================================================
** FILE DESCRIPTOR UTILITY PROTOTYPES
** =============================================================================
** Helper functions for managing file descriptors and error reporting.
*/

/*
** ft_close - Closes both ends of a pipe with optional error reporting
** --------------------------------------------------------------------
** @param fd: Array of two file descriptors [read_end, write_end]
** @param word: Optional string for perror() message (NULL to skip)
** @return: Always returns -1 (convenient for error returns)
**
** This utility function:
** 1. Closes both file descriptors in the pipe array
** 2. Optionally calls perror() with the given message
** 3. Returns -1 for use in error return statements
**
** Usage patterns:
** - Normal cleanup: ft_close(fd, NULL)
** - Error cleanup: return ft_close(fd, "pipe failed")
**
** Why return -1? It allows compact error handling:
**     if (pipe(fd) == -1)
**         return (ft_close(fd, "Pipe failed"));  // Clean up and return error
*/
int		ft_close(int *fd, char *word);

/*
** =============================================================================
** CHILD PROCESS HANDLER PROTOTYPES
** =============================================================================
** Functions that run in child processes to set up I/O redirection
** and execute commands.
**
** PIPE VISUALIZATION:
**                    ┌──────────────┐
**   infile ──────────►│   CHILD 1   │─────────┐
**                    │   (cmd1)    │         │
**                    └──────────────┘         │
**                                            ▼
**                                      ┌──────────┐
**                                      │   PIPE   │
**                                      │ fd[0]/[1]│
**                                      └──────────┘
**                                            │
**                    ┌──────────────┐         │
**   outfile ◄────────│   CHILD 2   │◄────────┘
**                    │   (cmd2)    │
**                    └──────────────┘
*/

/*
** child1 - First child process: reads from infile, writes to pipe
** ----------------------------------------------------------------
** @param fd: Pipe file descriptors [read_end, write_end]
** @param argv: Command line arguments (argv[1]=infile, argv[2]=cmd1)
** @param envp: Environment variables for command execution
**
** This function is called in the first child process (after fork).
** It sets up I/O redirection so that:
** - STDIN (fd 0) reads from the input file (argv[1])
** - STDOUT (fd 1) writes to the pipe's write end (fd[1])
**
** Steps:
** 1. Open input file (argv[1]) for reading
** 2. Redirect STDIN to input file using dup2()
** 3. Redirect STDOUT to pipe write end using dup2()
** 4. Close original file descriptors (no longer needed)
** 5. Execute command (argv[2])
**
** Error handling:
** - If file open fails: print error, close pipe, exit(1)
** - If dup2 fails: exit(1)
** - If command not found: exit(127)
**
** Note: This function never returns - it either execve()'s or exit()'s
*/
void	child1(int *fd, char **argv, char **envp);

/*
** child2 - Second child process: reads from pipe, writes to outfile
** ------------------------------------------------------------------
** @param fd: Pipe file descriptors [read_end, write_end]
** @param argv: Command line arguments (argv[3]=cmd2, argv[4]=outfile)
** @param envp: Environment variables for command execution
**
** This function is called in the second child process (after fork).
** It sets up I/O redirection so that:
** - STDIN (fd 0) reads from the pipe's read end (fd[0])
** - STDOUT (fd 1) writes to the output file (argv[4])
**
** Steps:
** 1. Open/create output file (argv[4]) for writing
**    - O_WRONLY: Write only
**    - O_CREAT: Create if doesn't exist
**    - O_TRUNC: Truncate to zero length if exists
**    - 0644: File permissions (rw-r--r--)
** 2. Redirect STDIN to pipe read end using dup2()
** 3. Redirect STDOUT to output file using dup2()
** 4. Close original file descriptors (no longer needed)
** 5. Execute command (argv[3])
**
** Error handling:
** - If file open/create fails: print error, close pipe, exit(1)
** - If dup2 fails: exit(1)
** - If command not found: exit(127)
**
** Note: This function never returns - it either execve()'s or exit()'s
*/
void	child2(int *fd, char **argv, char **envp);

#endif
