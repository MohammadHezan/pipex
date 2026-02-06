/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hpipex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammad-hezan <mohammad-hezan@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 19:33:31 by mhaizan           #+#    #+#             */
/*   Updated: 2026/02/06 13:54:46 by mohammad-he      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** =============================================================================
** PIPEX MAIN SOURCE FILE
** =============================================================================
**
** This file contains the core implementation of the pipex program, which
** replicates the Unix shell pipe behavior:
**
**     ./pipex infile "cmd1" "cmd2" outfile
**
** Is equivalent to:
**
**     < infile cmd1 | cmd2 > outfile
**
** FILE ORGANIZATION:
** ------------------
** 1. execute_cmd()   - Parses and executes a command string
** 2. child1()        - First child: infile -> cmd1 -> pipe
** 3. child2()        - Second child: pipe -> cmd2 -> outfile
** 4. parent_wait()   - Parent waits for children and gets exit status
** 5. main()          - Entry point: creates pipe, forks, orchestrates
**
** PROCESS ARCHITECTURE:
** ---------------------
**
**     ┌─────────────┐     fork()      ┌─────────────┐
**     │             │ ────────────────►│   CHILD 1   │
**     │             │                 │ (runs cmd1) │
**     │   PARENT    │                 └──────┬──────┘
**     │  (main())   │                        │ writes to pipe
**     │             │                        ▼
**     │             │                 ┌──────────────┐
**     │             │     fork()      │     PIPE     │
**     │             │ ───────────┐    │  fd[0]/fd[1] │
**     │             │            │    └──────────────┘
**     └──────┬──────┘            │           │
**            │                   │           │ reads from pipe
**            │ waitpid()         ▼           ▼
**            │              ┌─────────────┐
**            │              │   CHILD 2   │
**            └──────────────│ (runs cmd2) │
**                           └─────────────┘
**
** =============================================================================
*/

#include "Hpipex.h"

/*
** =============================================================================
** execute_cmd - Splits a command string and executes it via execve()
** =============================================================================
**
** PURPOSE:
** --------
** Takes a command string like "grep hello" and:
** 1. Splits it into an array: {"grep", "hello", NULL}
** 2. Finds the full path of the executable: "/usr/bin/grep"
** 3. Replaces current process with the command using execve()
**
** PARAMETERS:
** -----------
** @param cmd_str: The command string to execute (e.g., "ls -la")
** @param envp: Environment variables array (passed to execve)
**
** RETURN VALUE:
** -------------
** This function never returns on success - execve() replaces the process.
** On failure, it calls exit() with status 127 (command not found convention).
**
** ALGORITHM:
** ----------
** 1. Split cmd_str by spaces to get command and arguments
** 2. Validate that we got at least one token (the command itself)
** 3. Resolve the command to a full path using get_path()
** 4. Call execve() to replace process with the command
** 5. If execve() returns (it only does on error), clean up and exit
**
** ERROR HANDLING:
** ---------------
** - Empty/NULL command: prints ": command not found", exits 127
** - Command not in PATH: prints "cmd: command not found", exits 127
** - execve() failure: exits 127 (rare, usually permission issues)
**
** MEMORY MANAGEMENT:
** ------------------
** On failure, all allocated memory (args array, path string) is freed
** before calling exit(). On success, the memory is automatically
** released when execve() replaces the process image.
**
** EXAMPLE:
** --------
** execute_cmd("grep hello", envp);
**   -> args = {"grep", "hello", NULL}
**   -> path = "/usr/bin/grep"
**   -> execve("/usr/bin/grep", {"grep", "hello", NULL}, envp)
**   -> Current process becomes 'grep hello'
**
** =============================================================================
*/
static void	execute_cmd(char *cmd_str, char **envp)
{
	char	**args;  /* Array of command arguments after splitting */
	char	*path;   /* Full path to the executable */

	/* Split the command string by spaces into an array of arguments */
	/* Example: "ls -la" -> {"ls", "-la", NULL} */
	args = ft_split(cmd_str, ' ');
	/* Check if split failed or produced an empty result */
	/* This handles cases like empty string "" or just spaces "   " */
	if (!args || !args[0])
	{
		/* Print error message to stderr (fd 2) */
		write(2, ": command not found\n", 21);
		/* Free the args array if it was allocated but empty */
		ft_free_split(args);
		/* Exit with 127 - standard shell "command not found" code */
		exit(127);
	}
	/* Resolve the command name to its full executable path */
	/* e.g., "grep" -> "/usr/bin/grep" */
	path = get_path(args[0], envp);
	/* If command not found in PATH or doesn't exist */
	if (!path)
	{
		/* Print the command name that wasn't found */
		write(2, args[0], ft_strlen(args[0]));
		/* Print the error suffix */
		write(2, ": command not found\n", 21);
		/* Clean up allocated memory before exiting */
		ft_free_split(args);
		/* Exit with 127 - command not found */
		exit(127);
	}
	/* Execute the command - replaces current process image */
	/* If successful, this line is the last thing this process runs */
	/* The lines below only execute if execve() fails */
	execve(path, args, envp);
	/* If we reach here, execve failed (shouldn't happen normally) */
	/* Clean up memory */
	ft_free_split(args);
	free(path);
	/* Exit with error code */
	exit(127);
}

/*
** =============================================================================
** child1 - First child process handler (input file -> pipe)
** =============================================================================
**
** PURPOSE:
** --------
** Sets up the first child process to:
** 1. Read input from a file (infile)
** 2. Execute the first command (cmd1)
** 3. Send output to the pipe (for cmd2 to read)
**
** This replicates the left side of: < infile cmd1 | cmd2 > outfile
**
** PARAMETERS:
** -----------
** @param fd: Pipe file descriptors array
**            - fd[0]: Read end of pipe (unused by child1)
**            - fd[1]: Write end of pipe (stdout will point here)
** @param argv: Command line arguments
**              - argv[1]: Input filename (infile)
**              - argv[2]: First command to execute (cmd1)
** @param envp: Environment variables for execve()
**
** I/O REDIRECTION VISUALIZATION:
** ------------------------------
**
**     BEFORE dup2():                    AFTER dup2():
**     ┌─────────────┐                   ┌─────────────┐
**     │ STDIN  (0) ─┼── keyboard        │ STDIN  (0) ─┼── infile
**     │ STDOUT (1) ─┼── terminal        │ STDOUT (1) ─┼── pipe[1]
**     │ STDERR (2) ─┼── terminal        │ STDERR (2) ─┼── terminal
**     └─────────────┘                   └─────────────┘
**
** ALGORITHM:
** ----------
** 1. Open the input file (argv[1]) for reading
** 2. Use dup2() to redirect STDIN to read from infile
** 3. Use dup2() to redirect STDOUT to write to pipe
** 4. Close original file descriptors (not needed after dup2)
** 5. Execute the first command (argv[2])
**
** WHY CLOSE FDs AFTER dup2()?
** ---------------------------
** After dup2(infile, STDIN_FILENO):
** - Both 'infile' and STDIN (0) point to the same file
** - We only need STDIN, so close the original 'infile' fd
** - This prevents resource leaks and running out of file descriptors
**
** ERROR HANDLING:
** ---------------
** - File doesn't exist: perror prints error, exits 1
** - Permission denied: perror prints error, exits 1
** - dup2 failure: exits 1 (rare, usually fd limit reached)
**
** =============================================================================
*/
void	child1(int *fd, char **argv, char **envp)
{
	int	infile;  /* File descriptor for the input file */

	/* Open the input file (argv[1]) for reading only */
	/* O_RDONLY = Open for reading only */
	/* 0644 = Permission mode (not used for O_RDONLY, included for consistency) */
	infile = open(argv[1], O_RDONLY, 0644);
	/* Check if open() failed (returns -1 on error) */
	if (infile == -1)
	{
		/* Print descriptive error: "filename: error description" */
		/* perror() automatically adds the system error message */
		perror(argv[1]);
		/* Close both pipe ends before exiting */
		/* Important: prevents parent from hanging waiting for pipe data */
		close(fd[0]);
		close(fd[1]);
		/* Exit with error status */
		exit(1);
	}
	/* Redirect STDIN (fd 0) to read from infile instead of keyboard */
	/* dup2(old_fd, new_fd) makes new_fd point to same file as old_fd */
	/* After this, reading from STDIN actually reads from infile */
	if (dup2(infile, STDIN_FILENO) == -1)
		exit(1);  /* dup2 failed - exit immediately */
	/* Redirect STDOUT (fd 1) to write to pipe instead of terminal */
	/* After this, writing to STDOUT actually writes to pipe */
	/* fd[1] is the WRITE end of the pipe */
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		exit(1);  /* dup2 failed - exit immediately */
	/* Close the original infile fd - we now access it through STDIN */
	close(infile);
	/* Close both pipe file descriptors */
	/* We now access pipe write end through STDOUT, don't need fd[1] */
	/* We never needed fd[0] (read end) in child1 */
	ft_close(fd, NULL);
	/* Execute the first command (argv[2]) */
	/* Example: if argv[2] is "grep hello", this runs grep */
	/* Input comes from infile, output goes to pipe */
	/* This function never returns - process is replaced by cmd */
	execute_cmd(argv[2], envp);
}

/*
** =============================================================================
** child2 - Second child process handler (pipe -> output file)
** =============================================================================
**
** PURPOSE:
** --------
** Sets up the second child process to:
** 1. Read input from the pipe (output from cmd1)
** 2. Execute the second command (cmd2)
** 3. Write output to a file (outfile)
**
** This replicates the right side of: < infile cmd1 | cmd2 > outfile
**
** PARAMETERS:
** -----------
** @param fd: Pipe file descriptors array
**            - fd[0]: Read end of pipe (stdin will point here)
**            - fd[1]: Write end of pipe (unused by child2)
** @param argv: Command line arguments
**              - argv[3]: Second command to execute (cmd2)
**              - argv[4]: Output filename (outfile)
** @param envp: Environment variables for execve()
**
** I/O REDIRECTION VISUALIZATION:
** ------------------------------
**
**     BEFORE dup2():                    AFTER dup2():
**     ┌─────────────┐                   ┌─────────────┐
**     │ STDIN  (0) ─┼── keyboard        │ STDIN  (0) ─┼── pipe[0]
**     │ STDOUT (1) ─┼── terminal        │ STDOUT (1) ─┼── outfile
**     │ STDERR (2) ─┼── terminal        │ STDERR (2) ─┼── terminal
**     └─────────────┘                   └─────────────┘
**
** ALGORITHM:
** ----------
** 1. Open/create the output file (argv[4]) for writing
** 2. Use dup2() to redirect STDIN to read from pipe
** 3. Use dup2() to redirect STDOUT to write to outfile
** 4. Close original file descriptors
** 5. Execute the second command (argv[3])
**
** OPEN FLAGS EXPLAINED:
** ---------------------
** O_WRONLY: Open for writing only
** O_CREAT:  Create file if it doesn't exist
** O_TRUNC:  If file exists, truncate it to zero length (overwrite)
** 0644:     Permission bits (rw-r--r--)
**           - Owner: read + write (6)
**           - Group: read only (4)
**           - Others: read only (4)
**
** =============================================================================
*/
void	child2(int *fd, char **argv, char **envp)
{
	int	outfile;  /* File descriptor for the output file */

	/* Open the output file for writing, create if needed, truncate if exists */
	/* This mirrors shell behavior: > outfile creates/overwrites the file */
	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	/* Check if open() failed */
	if (outfile == -1)
	{
		/* Print descriptive error message */
		perror(argv[4]);
		/* Close pipe ends to prevent parent from hanging */
		close(fd[0]);
		close(fd[1]);
		/* Exit with error status */
		exit(1);
	}
	/* Redirect STDIN to read from pipe's read end */
	/* After this, reading from STDIN actually reads from pipe */
	/* fd[0] is the READ end of the pipe */
	if (dup2(fd[0], STDIN_FILENO) == -1)
		exit(1);  /* dup2 failed */
	/* Redirect STDOUT to write to outfile */
	/* After this, writing to STDOUT actually writes to outfile */
	if (dup2(outfile, STDOUT_FILENO) == -1)
		exit(1);  /* dup2 failed */
	/* Close the original outfile fd - we now access it through STDOUT */
	close(outfile);
	/* Close both pipe fds - we now access read end through STDIN */
	ft_close(fd, NULL);
	/* Execute the second command (argv[3]) */
	/* Input comes from pipe (cmd1's output), output goes to outfile */
	execute_cmd(argv[3], envp);
}

/*
** =============================================================================
** parent_wait - Waits for all child processes and returns last exit status
** =============================================================================
**
** PURPOSE:
** --------
** The parent process uses this function to:
** 1. Wait for both child processes to complete
** 2. Capture and return the exit status of the last command (cmd2)
**
** This mimics shell behavior where a pipeline's exit status is the
** exit status of the last command in the pipeline.
**
** Example:
**     false | true   -> exit status 0 (true succeeded)
**     true | false   -> exit status 1 (false failed)
**
** PARAMETERS:
** -----------
** @param last_pid: Process ID of the second child (cmd2)
**                  We track this because its exit status is what we return
**
** RETURN VALUE:
** -------------
** Returns the exit status of the last command (cmd2):
** - If normally exited: returns the exit code (0-255)
** - If killed by signal: returns 128 + signal number
** - Default: returns 0
**
** ALGORITHM:
** ----------
** 1. Loop twice (once for each child)
** 2. Call waitpid(-1, ...) to wait for ANY child to finish
** 3. If the finished child is last_pid, save its status
** 4. After both finish, decode and return last_pid's status
**
** WHY waitpid(-1)?
** ----------------
** waitpid(-1, ...) waits for ANY child process. This is important because:
** - Child1 might finish before or after child2 (depends on commands)
** - We need to reap both children to prevent zombie processes
** - We specifically track last_pid to return the correct status
**
** EXIT STATUS DECODING:
** ---------------------
** The 'status' from waitpid is encoded. We use macros to decode:
**
** WIFEXITED(status):     Returns true if child exited normally (called exit())
** WEXITSTATUS(status):   Returns the exit code passed to exit()
** WIFSIGNALED(status):   Returns true if child was killed by a signal
** WTERMSIG(status):      Returns the signal number that killed the child
**
** Signal exit convention: When a process is killed by signal N,
** shells report exit status as 128 + N. For example:
** - SIGTERM (15): exit status = 143
** - SIGKILL (9):  exit status = 137
**
** =============================================================================
*/
static int	parent_wait(pid_t last_pid)
{
	int		status;       /* Raw status from waitpid */
	int		last_status;  /* Status of the last command (cmd2) */
	pid_t	pid;          /* PID of the child that just finished */
	int		i;            /* Loop counter */

	i = 0;             /* Initialize counter */
	last_status = 0;   /* Default to success */
	/* Loop twice - once for each child process */
	while (i < 2)
	{
		/* Wait for ANY child to finish */
		/* -1 means "any child", &status receives the exit info */
		/* 0 = no special flags (blocking wait) */
		pid = waitpid(-1, &status, 0);
		/* If waitpid failed (shouldn't happen normally) */
		if (pid == -1)
			break ;  /* Exit loop early */
		/* Check if this is the second child (the one we care about) */
		if (pid == last_pid)
			last_status = status;  /* Save its status */
		i++;  /* Move to next child */
	}
	/* Decode the exit status of the last command */
	/* Check if process exited normally (via exit() or return from main) */
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));  /* Return the exit code */
	/* Check if process was killed by a signal */
	if (WIFSIGNALED(last_status))
		return (WTERMSIG(last_status) + 128);  /* Return 128 + signal number */
	/* Default case (shouldn't normally reach here) */
	return (0);
}

/*
** =============================================================================
** main - Program entry point and pipeline orchestrator
** =============================================================================
**
** PURPOSE:
** --------
** This is the entry point that orchestrates the entire pipeline:
**
**     ./pipex infile "cmd1" "cmd2" outfile
**
** Equivalent to shell command:
**
**     < infile cmd1 | cmd2 > outfile
**
** PARAMETERS:
** -----------
** @param argc: Argument count (must be exactly 5)
** @param argv: Argument values
**              - argv[0]: Program name ("./pipex")
**              - argv[1]: Input file path (infile)
**              - argv[2]: First command (cmd1)
**              - argv[3]: Second command (cmd2)
**              - argv[4]: Output file path (outfile)
** @param envp: Environment variables (contains PATH, HOME, etc.)
**
** RETURN VALUE:
** -------------
** Returns the exit status of the last command (cmd2), following shell conventions.
** Returns -1 if there's an error before executing commands.
**
** ALGORITHM:
** ----------
** 1. Validate argument count (must be 5)
** 2. Create a pipe for inter-process communication
** 3. Fork first child (will run cmd1)
** 4. Fork second child (will run cmd2)
** 5. Close pipe in parent (children have their own copies)
** 6. Wait for both children to complete
** 7. Return exit status of second child
**
** PIPE MECHANICS:
** ---------------
** pipe(fd) creates a unidirectional data channel:
**
**     fd[0] ◄──────────── fd[1]
**     (read)   data flow  (write)
**
** When fork() creates a child:
** - Child gets COPIES of all file descriptors
** - Both parent and children share the same pipe
** - Each process must close the fds it doesn't use
**
**     Parent: fd[0], fd[1] ─┐
**     Child1: fd[0], fd[1] ─┼─► All point to SAME pipe
**     Child2: fd[0], fd[1] ─┘
**
** FORK MECHANICS:
** ---------------
** fork() returns:
** - In parent: child's PID (positive number)
** - In child: 0
** - On error: -1
**
** This allows us to branch:
**     pid = fork();
**     if (pid == 0)
**         // This code runs in child
**     else
**         // This code runs in parent
**
** WHY CLOSE PIPE IN PARENT?
** -------------------------
** The parent doesn't use the pipe for data transfer. If it keeps the
** write end open, the read end won't see EOF when child1 finishes,
** and child2 will hang forever waiting for more data.
**
** =============================================================================
*/
int	main(int argc, char **argv, char **envp)
{
	int		fd[2];   /* Pipe file descriptors: fd[0]=read, fd[1]=write */
	pid_t	pid1;    /* Process ID of first child */
	pid_t	pid2;    /* Process ID of second child */

	/* Validate argument count */
	/* We need exactly 5 arguments: program name + infile + cmd1 + cmd2 + outfile */
	if (argc != 5)
	{
		/* Print error to stderr */
		write(2, "Invalid arguments\n", 18);
		/* Return error code */
		return (-1);
	}
	/* Create a pipe for inter-process communication */
	/* pipe() fills fd[0] (read end) and fd[1] (write end) */
	if (pipe(fd) == -1)
		return (ft_close(fd, "Pipe failed"));  /* Print error and return -1 */
	/* Create first child process */
	/* fork() duplicates this process - child gets a copy of everything */
	pid1 = fork();
	/* Check if fork failed */
	if (pid1 == -1)
		return (ft_close(fd, "Fork failed"));  /* Clean up and exit */
	/* This code runs only in child1 (fork returns 0 in child) */
	if (pid1 == 0)
		child1(fd, argv, envp);  /* Never returns - calls execve or exit */
	/* Parent continues here - create second child */
	pid2 = fork();
	/* This code runs only in child2 */
	if (pid2 == 0)
		child2(fd, argv, envp);  /* Never returns - calls execve or exit */
	/* Check if second fork failed */
	if (pid2 == -1)
		return (ft_close(fd, "Fork failed"));
	/* Parent: close pipe file descriptors */
	/* Important! Without this, children may hang waiting for EOF */
	ft_close(fd, NULL);
	/* Wait for both children and return exit status of the last one */
	/* This makes pipex behave like a shell pipeline */
	return (parent_wait(pid2));
}
