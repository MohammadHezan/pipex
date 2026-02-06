/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hutils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:49:22 by mhaizan           #+#    #+#             */
/*   Updated: 2026/02/06 13:54:46 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** PIPEX UTILITY FUNCTIONS
** =============================================================================
**
** This file contains helper functions used throughout the pipex project.
** These utilities handle:
**
** 1. MEMORY MANAGEMENT
**    - ft_free_split(): Safely frees arrays created by ft_split()
**
** 2. FILE DESCRIPTOR MANAGEMENT
**    - ft_close(): Closes pipe file descriptors with optional error reporting
**
** 3. PATH RESOLUTION
**    - find_in_paths(): Searches directories for an executable
**    - get_path(): Main entry point for command path resolution
**
** 4. COMMAND EXECUTION
**    - ft_execve(): Wrapper around execve with error handling
**
** WHY SEPARATE UTILITIES?
** -----------------------
** Keeping these functions separate from the main logic provides:
** - Better code organization and readability
** - Reusability across different parts of the program
** - Easier testing and debugging
** - Compliance with 42's Norm (max 5 functions per file)
**
** =============================================================================
*/

#include "Hpipex.h"

/*
** =============================================================================
** ft_free_split - Safely frees a NULL-terminated array of strings
** =============================================================================
**
** PURPOSE:
** --------
** Properly deallocates memory for arrays created by ft_split().
** ft_split() allocates:
** 1. An array of char pointers
** 2. Each individual string in the array
**
** Both must be freed to prevent memory leaks.
**
** PARAMETERS:
** -----------
** @param split: NULL-terminated array of strings, or NULL
**
** MEMORY LAYOUT:
** --------------
** After ft_split("hello world", ' '):
**
**     split ─────────► ┌─────────┐
**                      │ ptr[0] ─┼──────► "hello\0"  [heap]
**                      ├─────────┤
**                      │ ptr[1] ─┼──────► "world\0"  [heap]
**                      ├─────────┤
**                      │  NULL   │
**                      └─────────┘
**                         [heap]
**
** After ft_free_split():
** - "hello\0" freed
** - "world\0" freed
** - ptr array freed
**
** ALGORITHM:
** ----------
** 1. Check if split is NULL (nothing to free)
** 2. Iterate through array until NULL terminator
** 3. Free each string
** 4. Free the array itself
**
** WHY CHECK FOR NULL?
** -------------------
** - ft_split() can return NULL on allocation failure
** - Calling free(NULL) is safe, but iterating over NULL crashes
** - Defensive programming prevents segfaults
**
** =============================================================================
*/
void	ft_free_split(char **split)
{
	int	i;  /* Index for iterating through the array */

	/* Safety check: if split is NULL, nothing to free */
	if (!split)
		return ;
	/* Initialize index to start of array */
	i = 0;
	/* Iterate through array until we hit the NULL terminator */
	/* Each iteration: free the string, then increment index */
	while (split[i])
		free(split[i++]);  /* Free string at index i, then i++ */
	/* Finally, free the array of pointers itself */
	free(split);
}

/*
** =============================================================================
** ft_close - Closes both pipe file descriptors with optional error message
** =============================================================================
**
** PURPOSE:
** --------
** Utility function that:
** 1. Closes both ends of a pipe
** 2. Optionally prints an error message using perror()
** 3. Returns -1 for convenient error return statements
**
** PARAMETERS:
** -----------
** @param fd: Array of two file descriptors [read_end, write_end]
**            Can be NULL if no fds need to be closed
** @param word: Optional prefix for perror() message
**              Pass NULL to skip error printing
**
** RETURN VALUE:
** -------------
** Always returns -1. This allows compact error handling like:
**     if (pipe(fd) == -1)
**         return (ft_close(fd, "Pipe failed"));
**
** WHY CLOSE BOTH ENDS?
** --------------------
** A pipe has two ends:
** - fd[0]: Read end (data flows OUT of pipe)
** - fd[1]: Write end (data flows INTO pipe)
**
** When cleaning up, both must be closed to:
** - Release kernel resources
** - Signal EOF to readers
** - Prevent deadlocks where processes wait forever
**
** PERROR() BEHAVIOR:
** ------------------
** perror(word) prints: "word: error description\n"
**
** The error description comes from the global 'errno' variable,
** which is set by the failed system call. Examples:
** - perror("pipe") -> "pipe: Too many open files"
** - perror("fork") -> "fork: Resource temporarily unavailable"
**
** =============================================================================
*/
int	ft_close(int *fd, char *word)
{
	/* Only close file descriptors if fd is not NULL */
	if (fd)
	{
		/* Close the read end of the pipe */
		close(fd[0]);
		/* Close the write end of the pipe */
		close(fd[1]);
	}
	/* If an error message was provided, print it */
	if (word)
		perror(word);  /* Prints: "word: system error message\n" */
	/* Always return -1 for error indication */
	return (-1);
}

/*
** =============================================================================
** find_in_paths - Searches PATH directories for an executable command
** =============================================================================
**
** PURPOSE:
** --------
** When you type 'ls' in a shell, how does it find '/bin/ls'?
** The shell searches through directories listed in the PATH environment
** variable. This function implements that search.
**
** PARAMETERS:
** -----------
** @param paths: Array of directory paths to search
**               Example: {"/bin", "/usr/bin", "/usr/local/bin", NULL}
** @param cmd: Name of the command to find
**             Example: "ls", "grep", "cat"
**
** RETURN VALUE:
** -------------
** - On success: Full path to executable (heap-allocated, caller must free)
** - On failure: NULL (command not found in any directory)
**
** ALGORITHM:
** ----------
** For each directory in paths:
**   1. Construct full path: directory + "/" + cmd
**      Example: "/usr/bin" + "/" + "ls" = "/usr/bin/ls"
**   2. Check if this path is executable using access(path, X_OK)
**   3. If executable, return this path
**   4. If not, free the path and try next directory
** If no directory contains the executable, return NULL
**
** ACCESS() AND X_OK:
** ------------------
** access(path, X_OK) checks if:
** 1. The file exists
** 2. The current user has execute permission
**
** X_OK is defined in <unistd.h> and means "check execute permission"
** Other modes: R_OK (read), W_OK (write), F_OK (exists)
**
** MEMORY MANAGEMENT:
** ------------------
** - Uses ft_strjoin twice to build the path
** - Frees intermediate string ('tmp')
** - Returns the full path (caller's responsibility to free)
** - Frees paths that weren't executable
**
** EXAMPLE TRACE:
** --------------
** find_in_paths({"/bin", "/usr/bin"}, "grep")
**
** Iteration 1: paths[0] = "/bin"
**   tmp = "/bin/"
**   full = "/bin/grep"
**   access("/bin/grep", X_OK) -> -1 (not found)
**   free("/bin/grep")
**
** Iteration 2: paths[1] = "/usr/bin"
**   tmp = "/usr/bin/"
**   full = "/usr/bin/grep"
**   access("/usr/bin/grep", X_OK) -> 0 (found!)
**   return "/usr/bin/grep"
**
** =============================================================================
*/
char	*find_in_paths(char **paths, char *cmd)
{
	char	*full;  /* Complete path: directory + "/" + command */
	char	*tmp;   /* Temporary: directory + "/" */
	int		i;      /* Index for iterating through paths array */

	i = 0;  /* Start at first directory */
	/* Iterate through all directories in PATH */
	while (paths[i])
	{
		/* Step 1: Append "/" to directory path */
		/* e.g., "/usr/bin" -> "/usr/bin/" */
		tmp = ft_strjoin(paths[i], "/");
		/* Step 2: Append command name to get full path */
		/* e.g., "/usr/bin/" + "ls" -> "/usr/bin/ls" */
		full = ft_strjoin(tmp, cmd);
		/* Free the intermediate string - no longer needed */
		free(tmp);
		/* Step 3: Check if this file exists and is executable */
		/* X_OK checks execute permission for current user */
		if (access(full, X_OK) == 0)
			return (full);  /* Found it! Return the path */
		/* Not found in this directory, free and try next */
		free(full);
		i++;  /* Move to next directory in PATH */
	}
	/* Command not found in any PATH directory */
	return (NULL);
}

/*
** =============================================================================
** get_path - Resolves a command name to its full executable path
** =============================================================================
**
** PURPOSE:
** --------
** This is the main entry point for command path resolution.
** Given a command, it returns the full path to the executable.
**
** PARAMETERS:
** -----------
** @param cmd: Command to resolve. Can be:
**             - Absolute path: "/usr/bin/ls"
**             - Relative path: "./my_script" or "../bin/tool"
**             - Bare command: "ls", "grep", "cat"
** @param envp: Environment variables array, which includes PATH
**
** RETURN VALUE:
** -------------
** - On success: Full path to executable (heap-allocated)
** - On failure: NULL (command not found)
**
** ALGORITHM:
** ----------
** 1. If cmd contains '/' (it's already a path):
**    - Return a copy of cmd (might be relative or absolute)
**
** 2. Otherwise (bare command name):
**    a. Search envp for "PATH=..."
**    b. Extract the path string after "PATH="
**    c. Split by ':' to get individual directories
**    d. Search each directory for the command
**    e. Return first match or NULL
**
** WHY CHECK FOR '/'?
** ------------------
** Commands containing '/' are paths (absolute or relative):
** - "/bin/ls"     -> absolute path (starts with /)
** - "./script"    -> relative to current directory
** - "../bin/cmd"  -> relative path going up
**
** For these, we don't search PATH - we use them directly.
** We still strdup() to maintain consistent memory management
** (caller always frees the returned string).
**
** ENVIRONMENT VARIABLE FORMAT:
** ----------------------------
** envp is an array of "KEY=value" strings:
** envp[0] = "HOME=/home/user"
** envp[1] = "PATH=/bin:/usr/bin:/usr/local/bin"
** envp[2] = "SHELL=/bin/bash"
** ...
** envp[n] = NULL
**
** We search for "PATH=" using strncmp, then extract the value after "PATH=".
**
** PATH FORMAT:
** ------------
** PATH is colon-separated list of directories:
** "/bin:/usr/bin:/usr/local/bin"
**
** After ft_split(path, ':'):
** {"/bin", "/usr/bin", "/usr/local/bin", NULL}
**
** =============================================================================
*/
char	*get_path(char *cmd, char **envp)
{
	char	**paths;  /* Array of directories from PATH */
	char	*res;     /* Result: full path to command */
	int		i;        /* Index for searching envp */

	i = 0;  /* Start at first environment variable */
	/* Check if command already contains a path separator */
	/* If so, it's already a path - no need to search */
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));  /* Return a copy of the path */
	/* Search environment variables for PATH= */
	/* strncmp compares first 5 chars: "PATH=" */
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;  /* Keep looking until we find PATH= or reach end */
	/* If PATH not found (envp[i] is NULL), can't search for command */
	if (!envp[i])
		return (NULL);
	/* Split PATH value by ':' to get array of directories */
	/* envp[i] + 5 skips "PATH=" prefix to get the value */
	/* e.g., "PATH=/bin:/usr/bin" + 5 = "/bin:/usr/bin" */
	paths = ft_split(envp[i] + 5, ':');
	/* Check if split failed (allocation error) */
	if (!paths)
		return (NULL);
	/* Search all PATH directories for the command */
	res = find_in_paths(paths, cmd);
	/* Free the paths array - we don't need it anymore */
	ft_free_split(paths);
	/* Return result (full path or NULL if not found) */
	return (res);
}

/*
** =============================================================================
** ft_execve - Executes a command with path resolution and error handling
** =============================================================================
**
** PURPOSE:
** --------
** A wrapper around execve() that:
** 1. Resolves the command to a full path
** 2. Handles "command not found" errors gracefully
** 3. Cleans up memory on failure
**
** Note: This function is currently unused in the main code path
** (execute_cmd() handles this inline), but is kept for potential
** future use or testing.
**
** PARAMETERS:
** -----------
** @param cmd: NULL-terminated array of command and arguments
**             Example: {"ls", "-la", "/home", NULL}
** @param envp: Environment variables to pass to the new program
**
** BEHAVIOR:
** ---------
** On SUCCESS: This function never returns!
** - execve() replaces the entire process image with the new program
** - Memory, code, stack - everything is replaced
** - The process continues as the new program
**
** On FAILURE: Prints error and calls exit()
** - "cmd: command not found\n" if path resolution fails
** - System error message if execve() fails
**
** EXIT CODES:
** -----------
** 127: Command not found (shell convention)
** 1: execve failed (permission denied, corrupt binary, etc.)
**
** WHY FREE MEMORY BEFORE EXIT?
** ----------------------------
** Strictly speaking, the OS reclaims all memory when a process exits.
** However, freeing explicitly:
** - Helps valgrind report true leaks vs. expected program-end memory
** - Is good practice and shows intentional memory management
** - Makes the code clearer about ownership
**
** EXECVE() DETAILS:
** -----------------
** int execve(const char *pathname, char *const argv[], char *const envp[]);
**
** - pathname: Full path to executable ("/usr/bin/ls")
** - argv: Command arguments (argv[0] should be command name)
** - envp: Environment variables for the new program
**
** Returns: -1 on error (and sets errno)
** On success: NEVER RETURNS - process is replaced!
**
** =============================================================================
*/
void	ft_execve(char **cmd, char **envp)
{
	char	*path;  /* Resolved full path to the command */

	/* Resolve command name to full executable path */
	path = get_path(cmd[0], envp);
	/* Check if path resolution failed */
	if (!path)
	{
		/* Print error message to stderr */
		write(2, cmd[0], ft_strlen(cmd[0]));  /* Command name */
		write(2, ": command not found\n", 21); /* Error suffix */
		/* Free the command array before exiting */
		ft_free_split(cmd);
		/* Exit with 127 - standard "command not found" code */
		exit(127);
	}
	/* Execute the command - replaces this process if successful */
	/* If execve succeeds, the lines below NEVER execute */
	if (execve(path, cmd, envp) == -1)
	{
		/* execve failed - print system error (includes errno info) */
		perror("execve");
		/* Clean up allocated memory */
		free(path);
		ft_free_split(cmd);
		/* Exit with general error code */
		exit(1);
	}
}
