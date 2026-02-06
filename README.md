*This project has been created as part of the 42 curriculum by mhaizan.*

<!-- Project Banner -->
<div align="center">
  <h1>🔗 Pipex</h1>
  <p><em>Recreating Unix shell pipe behavior with process and file descriptor management</em></p>
  
  ![42 Badge](https://img.shields.io/badge/42-Project-blue)
  ![Language](https://img.shields.io/badge/Language-C-green)
  ![Norminette](https://img.shields.io/badge/Norminette-Passing-success)
  ![Type](https://img.shields.io/badge/Type-Systems%20Programming-orange)
  <img width="201" height="170" alt="Screenshot from 2026-02-06 13-35-10" src="https://github.com/user-attachments/assets/212019ec-2817-46a7-954e-de915d8a1239" />

  <p><strong>A 42 School Systems Programming Project by mhaizan</strong></p>
</div>

---

## 📋 Table of Contents

1. [Description](#-description)
2. [Instructions](#-instructions)
3. [42 School Rules & Requirements](#-42-school-rules--requirements)
4. [The Challenge](#-the-challenge)
5. [Understanding Unix Pipes](#-understanding-unix-pipes)
6. [Core System Calls](#-core-system-calls)
7. [How Pipex Works](#-how-pipex-works)
8. [Process Architecture](#-process-architecture)
9. [File Descriptor Management](#-file-descriptor-management)
10. [PATH Resolution](#-path-resolution)
11. [Project Architecture](#-project-architecture)
12. [Code Walkthrough](#-code-walkthrough)
13. [Error Handling](#-error-handling)
14. [Memory Management](#-memory-management)
15. [Testing](#-testing)
16. [What I Learned](#-what-i-learned)
17. [Resources](#-resources)

---

## 📝 Description

**Pipex** is a systems programming project from the 42 School curriculum that challenges students to recreate the Unix shell pipe mechanism. The program replicates this shell behavior:

```bash
< infile cmd1 | cmd2 > outfile
```

### Project Goal

The primary objective is to understand and correctly implement:

- **Process creation** with `fork()`
- **Inter-process communication** with `pipe()`
- **File descriptor redirection** with `dup2()`
- **Program execution** with `execve()`
- **Process synchronization** with `waitpid()`

### Brief Overview

When you run:
```bash
./pipex infile "cmd1 arg1" "cmd2 arg2" outfile
```

Pipex performs these operations:
1. Creates a **pipe** for data transfer between commands
2. **Forks** two child processes
3. **Child 1**: Reads from `infile`, executes `cmd1`, writes to pipe
4. **Child 2**: Reads from pipe, executes `cmd2`, writes to `outfile`
5. **Parent**: Waits for both children and returns the exit status of `cmd2`

### Key Features

- ✅ Reproduces exact shell pipe behavior
- ✅ Proper error handling with descriptive messages
- ✅ PATH environment variable resolution
- ✅ Handles absolute, relative, and bare command names
- ✅ Returns correct exit status (like a real shell)
- ✅ Fully Norminette compliant
- ✅ No memory leaks

---

## 📖 Instructions

### Prerequisites

Before compiling, ensure you have:
- **GCC** or **Clang** compiler
- **Make** build system
- A Unix-like operating system (Linux, macOS, or WSL on Windows)

### Compilation

Clone the repository and compile using the Makefile:

```bash
# Clone the repository
git clone https://github.com/yourusername/pipex.git
cd pipex

# Compile the program
make

# The executable 'pipex' will be created in the root directory
```

#### Makefile Rules

| Command | Description |
|---------|-------------|
| `make` or `make all` | Compile the program (includes libft) |
| `make clean` | Remove object files (`.o` files) |
| `make fclean` | Remove object files and the executable |
| `make re` | Recompile everything from scratch |

### Installation

No installation is required. After compilation, the `pipex` executable can be run directly from the project directory.

### Execution

The program accepts exactly 4 arguments:

```bash
./pipex infile "cmd1 [args]" "cmd2 [args]" outfile
```

#### Arguments Explained

| Argument | Description | Example |
|----------|-------------|---------|
| `infile` | Input file (read by cmd1) | `input.txt` |
| `cmd1` | First command with arguments | `"grep hello"` |
| `cmd2` | Second command with arguments | `"wc -l"` |
| `outfile` | Output file (written by cmd2) | `output.txt` |

#### Usage Examples

```bash
# Basic usage - count lines containing "hello"
./pipex input.txt "grep hello" "wc -l" output.txt

# Equivalent shell command:
< input.txt grep hello | wc -l > output.txt

# Sort and get unique entries
./pipex data.txt "sort" "uniq" sorted.txt

# Find and count .c files
./pipex . "ls -la" "grep '.c'" result.txt
```

#### Edge Cases

```bash
# Non-existent input file
./pipex no_file "cat" "wc -l" out    # Error: no_file: No such file or directory

# Unknown command
./pipex in "not_a_cmd" "cat" out     # Error: not_a_cmd: command not found

# Permission denied
./pipex /etc/shadow "cat" "wc" out   # Error: /etc/shadow: Permission denied
```

---

## 🎯 Introduction

**Pipex** teaches fundamental Unix programming concepts that are essential for understanding how shells and operating systems work. The seemingly simple task of connecting two commands with a pipe involves multiple complex system calls working together.

### Why This Project Matters

This project teaches several critical concepts in systems programming:

| Concept | Description |
|---------|-------------|
| **Process Control** | Creating and managing child processes with `fork()` |
| **IPC (Inter-Process Communication)** | Using pipes for data transfer between processes |
| **File Descriptors** | Understanding Unix's "everything is a file" philosophy |
| **I/O Redirection** | Manipulating standard input/output streams |
| **Program Loading** | How `execve()` replaces a process image |
| **Exit Status** | Proper error propagation in Unix tradition |

The skills learned here directly apply to:
- Shell implementation
- System administration tools
- Server development
- Build systems and automation

---

## 📜 42 School Rules & Requirements

### The Norm (Norminette)

All code must comply with **42's coding standard** called "The Norm":

| Rule | Description |
|------|-------------|
| **Function Length** | Maximum 25 lines per function |
| **Functions per File** | Maximum 5 functions per `.c` file |
| **Line Length** | Maximum 80 characters per line |
| **Variable Declaration** | All variables declared at the start of functions |
| **No `for` loops** | Only `while` loops are allowed |
| **No comments inside functions** | Comments only in headers |
| **Header Files** | Must have include guards |
| **Global Variables** | Forbidden |

### Mandatory Part Requirements

```
Program name    : pipex
Turn in files   : Makefile, *.h, *.c
Makefile rules  : NAME, all, clean, fclean, re
Arguments       : file1 cmd1 cmd2 file2
External functs : open, close, read, write, malloc, free, perror,
                  strerror, access, dup, dup2, execve, exit, fork,
                  pipe, unlink, wait, waitpid
Libft allowed   : Yes
Description     : Replicate shell pipe behavior
```

### Allowed System Calls

| Function | Header | Purpose |
|----------|--------|---------|
| `open` | `<fcntl.h>` | Open files |
| `close` | `<unistd.h>` | Close file descriptors |
| `read` | `<unistd.h>` | Read from file descriptor |
| `write` | `<unistd.h>` | Write to file descriptor |
| `malloc` | `<stdlib.h>` | Dynamic memory allocation |
| `free` | `<stdlib.h>` | Memory deallocation |
| `perror` | `<stdio.h>` | Print error message |
| `access` | `<unistd.h>` | Check file permissions |
| `dup` | `<unistd.h>` | Duplicate file descriptor |
| `dup2` | `<unistd.h>` | Duplicate fd to specific number |
| `execve` | `<unistd.h>` | Execute a program |
| `exit` | `<stdlib.h>` | Terminate process |
| `fork` | `<unistd.h>` | Create child process |
| `pipe` | `<unistd.h>` | Create pipe |
| `wait` | `<sys/wait.h>` | Wait for child process |
| `waitpid` | `<sys/wait.h>` | Wait for specific child |

### Error Handling Requirements

The program must handle errors gracefully:

| Error Condition | Expected Behavior |
|-----------------|-------------------|
| Wrong argument count | Print usage error |
| Input file doesn't exist | Print "file: No such file or directory" |
| Input file no permission | Print "file: Permission denied" |
| Command not found | Print "cmd: command not found" |
| Output file can't be created | Print appropriate error |

---

## 🎮 The Challenge

### The Problem Statement

Replicate this shell behavior:
```bash
< file1 cmd1 | cmd2 > file2
```

This means:
1. `cmd1` reads from `file1` (not keyboard)
2. `cmd1`'s output goes to `cmd2` (not screen)
3. `cmd2`'s output goes to `file2` (not screen)

### Why Is This Hard?

| Challenge | Explanation |
|-----------|-------------|
| **Process Isolation** | Each command runs in its own process |
| **Coordination** | Processes must communicate through the pipe |
| **Redirection** | Standard I/O must be rewired correctly |
| **Cleanup** | File descriptors must be closed properly |
| **Error Handling** | Must handle failures at any step |

---

## 📚 Understanding Unix Pipes

### What Is a Pipe?

A pipe is a unidirectional data channel between processes. Think of it as a tube:
- One end for writing (data goes IN)
- One end for reading (data comes OUT)
- First-In-First-Out (FIFO) ordering

```
    ┌─────────────────────────────────────┐
    │              PIPE                   │
    │                                     │
    │  Write End ─────────► Read End      │
    │   (fd[1])   data flow   (fd[0])     │
    │                                     │
    └─────────────────────────────────────┘
```

### How pipe() Works

```c
int fd[2];
pipe(fd);
// fd[0] = read end
// fd[1] = write end
```

After `pipe()`:
- Writing to `fd[1]` puts data in the pipe
- Reading from `fd[0]` retrieves that data
- Data is buffered (typically 64KB on Linux)

### Pipe in Fork Context

When we `fork()` after `pipe()`:

```
           BEFORE FORK                    AFTER FORK
    ┌──────────────────────┐      ┌──────────────────────┐
    │       PARENT         │      │       PARENT         │
    │  fd[0] ─┐            │      │  fd[0] ─┐            │
    │         │            │      │         │            │
    │  fd[1] ─┼─► PIPE     │      │  fd[1] ─┼─► PIPE ◄──┼─ fd[1]  │
    │         │            │      │         │            │         │
    │         ▼            │      │         ▼            │  fd[0] ◄┘
    └──────────────────────┘      │                      │
                                  │       CHILD          │
                                  └──────────────────────┘
```

Both parent and child have their own copies of `fd[0]` and `fd[1]`, but they all point to the **same pipe**.

---

## 🔧 Core System Calls

### fork() - Process Creation

`fork()` creates an exact copy of the current process:

```c
pid_t pid = fork();

if (pid == -1)
    // Error occurred
else if (pid == 0)
    // This is the CHILD process
else
    // This is the PARENT process (pid = child's PID)
```

**What gets copied:**
- All memory (variables, heap, stack)
- All file descriptors
- Program counter (both continue from same point)

**What's different:**
- Process ID (PID)
- Return value of `fork()`

### pipe() - Create Communication Channel

```c
int fd[2];
if (pipe(fd) == -1)
    perror("pipe failed");
// fd[0] = read end
// fd[1] = write end
```

### dup2() - File Descriptor Redirection

`dup2(oldfd, newfd)` makes `newfd` a copy of `oldfd`:

```c
dup2(file_fd, STDIN_FILENO);   // STDIN now reads from file
dup2(pipe_fd, STDOUT_FILENO);  // STDOUT now writes to pipe
```

**Before dup2(file_fd, 0):**
```
fd 0 (STDIN)  → keyboard
fd 3 (file)   → file.txt
```

**After dup2(file_fd, 0):**
```
fd 0 (STDIN)  → file.txt
fd 3 (file)   → file.txt
```

### execve() - Replace Process Image

```c
execve("/bin/ls", {"ls", "-la", NULL}, envp);
// If successful, this line is NEVER reached
// The process becomes /bin/ls
```

**What happens:**
- Current process's code/data is **completely replaced**
- File descriptors are preserved (unless marked close-on-exec)
- PID stays the same
- Only returns on failure (returns -1)

### waitpid() - Wait for Child

```c
int status;
pid_t pid = waitpid(child_pid, &status, 0);

if (WIFEXITED(status))
    int exit_code = WEXITSTATUS(status);  // Get exit code
if (WIFSIGNALED(status))
    int signal = WTERMSIG(status);        // Get signal number
```

---

## ⚙️ How Pipex Works

### Step-by-Step Execution

```
1. VALIDATE ARGUMENTS
   └── argc must be 5

2. CREATE PIPE
   └── pipe(fd) creates fd[0] (read) and fd[1] (write)

3. FORK CHILD 1 (cmd1)
   ├── Open infile as STDIN
   ├── Connect pipe write end to STDOUT
   ├── Close unused file descriptors
   └── execve(cmd1)

4. FORK CHILD 2 (cmd2)
   ├── Connect pipe read end to STDIN
   ├── Open outfile as STDOUT
   ├── Close unused file descriptors
   └── execve(cmd2)

5. PARENT CLEANUP
   ├── Close both pipe ends
   ├── Wait for both children
   └── Return exit status of cmd2
```

### Visual Flow

```
                        ┌─────────────────┐
    ┌───────────────────│     PARENT      │───────────────────┐
    │                   │   (main())      │                   │
    │     fork()        └────────┬────────┘      fork()       │
    ▼                            │                            ▼
┌───────────────┐         waitpid()            ┌───────────────┐
│    CHILD 1    │                              │    CHILD 2    │
├───────────────┤                              ├───────────────┤
│ STDIN ← infile│                              │ STDIN ← pipe  │
│ STDOUT → pipe │══════════════════════════════│ STDOUT → outfile│
│ execve(cmd1)  │        data flow             │ execve(cmd2)  │
└───────────────┘        ────────►             └───────────────┘
```

---

## 🏗️ Process Architecture

### Three Processes Running

```
┌──────────────────────────────────────────────────────────────┐
│                    PROCESS TREE                              │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│                       PARENT                                 │
│                      (pipex)                                 │
│                     PID: 1000                                │
│                         │                                    │
│          ┌──────────────┴──────────────┐                     │
│          │                             │                     │
│          ▼                             ▼                     │
│      CHILD 1                       CHILD 2                   │
│      (cmd1)                        (cmd2)                    │
│     PID: 1001                     PID: 1002                  │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

### File Descriptor Table (Per Process)

**Parent Process (after creating pipe, before fork):**
```
┌─────┬──────────────────────────┐
│ FD  │ Points To                │
├─────┼──────────────────────────┤
│  0  │ Standard Input (terminal)│
│  1  │ Standard Output (terminal)│
│  2  │ Standard Error (terminal)│
│  3  │ Pipe Read End            │
│  4  │ Pipe Write End           │
└─────┴──────────────────────────┘
```

**Child 1 (after dup2 redirections):**
```
┌─────┬──────────────────────────┐
│ FD  │ Points To                │
├─────┼──────────────────────────┤
│  0  │ infile (redirected)      │
│  1  │ Pipe Write End (redirect)│
│  2  │ Standard Error (terminal)│
└─────┴──────────────────────────┘
```

**Child 2 (after dup2 redirections):**
```
┌─────┬──────────────────────────┐
│ FD  │ Points To                │
├─────┼──────────────────────────┤
│  0  │ Pipe Read End (redirect) │
│  1  │ outfile (redirected)     │
│  2  │ Standard Error (terminal)│
└─────┴──────────────────────────┘
```

---

## 📁 File Descriptor Management

### Why Close Unused FDs?

**Problem:** If parent keeps pipe write end open, child2 never sees EOF.

```
Scenario: Parent doesn't close fd[1]

Child1 finishes writing → closes its fd[1]
Child2 reads from fd[0] → waiting for EOF
BUT: Parent still has fd[1] open!
Result: Child2 hangs forever waiting for more data
```

**Solution:** Every process closes FDs it doesn't use.

### Closing Strategy

| Process | Must Close | Reason |
|---------|------------|--------|
| Parent | `fd[0]`, `fd[1]` | Not using pipe at all |
| Child1 | `fd[0]`, `infile` (after dup2) | Only needs STDOUT→pipe |
| Child2 | `fd[1]`, `outfile` (after dup2) | Only needs STDIN←pipe |

---

## 🔍 PATH Resolution

### How Commands Are Found

When you type `ls`, the shell searches for `/bin/ls` or `/usr/bin/ls`. Pipex does the same:

```
User input: "grep"

1. Check if contains '/':
   "grep" has no '/' → search PATH

2. Get PATH from environment:
   PATH=/usr/local/bin:/usr/bin:/bin

3. Split by ':':
   ["/usr/local/bin", "/usr/bin", "/bin"]

4. Try each directory:
   /usr/local/bin/grep → access() fails
   /usr/bin/grep → access() succeeds!

5. Return: "/usr/bin/grep"
```

### Path Types Handled

| Input | Type | Resolution |
|-------|------|------------|
| `/bin/ls` | Absolute | Use as-is |
| `./script` | Relative | Use as-is |
| `../bin/cmd` | Relative | Use as-is |
| `ls` | Bare name | Search PATH |

---

## 🗂️ Project Architecture

### Directory Structure

```
pipex/
├── Makefile              # Build automation
├── README.md             # This documentation
├── srcs/
│   ├── Hpipex.h          # Header: includes, prototypes, documentation
│   ├── Hpipex.c          # Core: main, child processes, waiting
│   └── Hutils.c          # Utilities: PATH resolution, cleanup
└── libft/
    ├── libft.h           # Libft header with all function prototypes
    ├── Makefile          # Libft build rules
    ├── ft_split.c        # String splitting (critical for pipex)
    ├── ft_strjoin.c      # Path building: dir + "/" + cmd
    ├── ft_strlen.c       # String length for write()
    ├── ft_strchr.c       # Check if cmd contains '/'
    ├── ft_strncmp.c      # Find "PATH=" in environment
    ├── ft_strdup.c       # Duplicate path strings
    └── ... (other libft functions)
```

### Module Responsibilities

| Module | Purpose | Key Functions |
|--------|---------|---------------|
| **Hpipex.c** | Pipeline orchestration | `main()`, `child1()`, `child2()`, `parent_wait()` |
| **Hutils.c** | Reusable utilities | `get_path()`, `find_in_paths()`, `ft_close()` |
| **Hpipex.h** | Interface definitions | All prototypes and includes |
| **Makefile** | Build automation | Compile pipex and link with libft |

### Data Flow

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│   ARGV      │────►│    MAIN     │────►│   PIPE      │
│  (infile,   │     │  (create    │     │  (fd[0],    │
│   cmds,     │     │   pipe,     │     │   fd[1])    │
│   outfile)  │     │   fork)     │     │             │
└─────────────┘     └─────────────┘     └──────┬──────┘
                                               │
                    ┌──────────────────────────┼──────────────────────────┐
                    │                          │                          │
                    ▼                          │                          ▼
            ┌─────────────┐                    │                  ┌─────────────┐
            │   CHILD 1   │                    │                  │   CHILD 2   │
            │ infile→cmd1 │═══════════════════════════════════════│ cmd2→outfile│
            │   →pipe     │        pipe data flow                 │ ←pipe       │
            └─────────────┘                                       └─────────────┘
```

---

## 📖 Code Walkthrough

### Main Function Flow

```c
int main(int argc, char **argv, char **envp)
{
    // 1. Validate: argc must be 5
    if (argc != 5)
        return error;
    
    // 2. Create pipe
    pipe(fd);
    
    // 3. Fork child 1
    pid1 = fork();
    if (pid1 == 0)
        child1(fd, argv, envp);  // Never returns
    
    // 4. Fork child 2
    pid2 = fork();
    if (pid2 == 0)
        child2(fd, argv, envp);  // Never returns
    
    // 5. Parent cleanup
    close(fd[0]);
    close(fd[1]);
    
    // 6. Wait and return status
    return parent_wait(pid2);
}
```

### Child 1 Process

```c
void child1(int *fd, char **argv, char **envp)
{
    // 1. Open input file
    infile = open(argv[1], O_RDONLY);
    
    // 2. Redirect STDIN to infile
    dup2(infile, STDIN_FILENO);
    
    // 3. Redirect STDOUT to pipe write end
    dup2(fd[1], STDOUT_FILENO);
    
    // 4. Close unused FDs
    close(infile);
    close(fd[0]);
    close(fd[1]);
    
    // 5. Execute command
    execute_cmd(argv[2], envp);  // Never returns
}
```

### Child 2 Process

```c
void child2(int *fd, char **argv, char **envp)
{
    // 1. Open/create output file
    outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    // 2. Redirect STDIN to pipe read end
    dup2(fd[0], STDIN_FILENO);
    
    // 3. Redirect STDOUT to outfile
    dup2(outfile, STDOUT_FILENO);
    
    // 4. Close unused FDs
    close(outfile);
    close(fd[0]);
    close(fd[1]);
    
    // 5. Execute command
    execute_cmd(argv[3], envp);  // Never returns
}
```

---

## ⚠️ Error Handling

### Error Categories

| Category | Example | Exit Code |
|----------|---------|-----------|
| File not found | `./pipex nofile "cat" "wc" out` | 1 |
| Permission denied | `./pipex /etc/shadow "cat" "wc" out` | 1 |
| Command not found | `./pipex in "badcmd" "wc" out` | 127 |
| System call failure | pipe/fork fails | -1 |

### Error Messages

All errors use `perror()` for descriptive messages:

```bash
$ ./pipex nonexistent "cat" "wc" out
nonexistent: No such file or directory

$ ./pipex /root/file "cat" "wc" out
/root/file: Permission denied

$ ./pipex in "badcmd" "wc" out
badcmd: command not found
```

### Exit Status Convention

| Status | Meaning |
|--------|---------|
| 0 | Success |
| 1 | General error |
| 127 | Command not found |
| 128+N | Killed by signal N |

---

## 💾 Memory Management

### Allocation Points

| Function | What's Allocated | Who Frees |
|----------|-----------------|-----------|
| `ft_split()` | Command array | `ft_free_split()` |
| `ft_strjoin()` | Path strings | `free()` |
| `get_path()` | Final path | `free()` |

### Memory Safety Pattern

```c
// Allocate
char **args = ft_split(cmd_str, ' ');

// Check success
if (!args)
    exit(1);

// Use the memory
char *path = get_path(args[0], envp);

// Clean up on error
if (!path)
{
    ft_free_split(args);
    exit(127);
}

// After execve succeeds: OS cleans up
// After execve fails: clean up manually
execve(path, args, envp);
ft_free_split(args);
free(path);
```

### Leak Prevention

- Every `malloc` has a corresponding `free`
- Error paths always clean up before `exit`
- `ft_free_split()` handles NULL gracefully
- `execve()` success = OS reclaims memory

---

## 🧪 Testing

### Basic Functional Tests

```bash
# Create test file
echo -e "hello world\nhello there\nhi" > infile

# Test 1: grep and count
./pipex infile "grep hello" "wc -l" outfile
cat outfile  # Should show: 2

# Compare with shell
< infile grep hello | wc -l > expected
diff outfile expected  # Should show nothing (identical)
```

### Error Tests

```bash
# Test: Missing input file
./pipex no_file "cat" "wc -l" out
# Expected: no_file: No such file or directory

# Test: Unknown command
./pipex infile "notacmd" "cat" out
# Expected: notacmd: command not found

# Test: Bad permissions
./pipex /etc/shadow "cat" "wc" out
# Expected: /etc/shadow: Permission denied
```

### Exit Status Tests

```bash
# Test: Normal success
./pipex infile "cat" "wc -l" out
echo $?  # Should be 0

# Test: Second command fails
./pipex infile "cat" "false" out
echo $?  # Should be 1

# Test: Command not found
./pipex infile "cat" "badcmd" out
echo $?  # Should be 127
```

### Memory Leak Test

```bash
# Using valgrind
valgrind --leak-check=full --show-leak-kinds=all \
    ./pipex infile "cat" "wc -l" outfile

# Expected: no leaks are possible
```

### Test Script

```bash
#!/bin/bash
echo "=== Pipex Test Suite ==="

# Setup
echo -e "line1\nline2\nline3" > test_in

# Test 1: Basic functionality
echo -n "Test 1 (basic): "
./pipex test_in "cat" "wc -l" test_out
[ "$(cat test_out)" = "3" ] && echo "PASS" || echo "FAIL"

# Test 2: Compare with shell
echo -n "Test 2 (shell comparison): "
./pipex test_in "grep line" "wc -l" test_out
< test_in grep line | wc -l > expected
diff -q test_out expected > /dev/null && echo "PASS" || echo "FAIL"

# Cleanup
rm -f test_in test_out expected

echo "=== Tests Complete ==="
```

---

## 📚 What I Learned

### Technical Skills

| Skill | What I Learned |
|-------|----------------|
| **Process Management** | How `fork()` creates processes, parent-child relationships |
| **IPC** | How pipes enable communication between processes |
| **File Descriptors** | Unix's unified I/O model, descriptor tables |
| **I/O Redirection** | Using `dup2()` to rewire stdin/stdout |
| **Program Execution** | How `execve()` loads and runs programs |
| **Error Handling** | Proper Unix error reporting conventions |

### Key Insights

1. **Everything is a file** - In Unix, pipes, terminals, and files all use the same interface
2. **Fork duplicates everything** - Understanding what's shared vs. copied is crucial
3. **Close unused FDs** - Prevents deadlocks and resource leaks
4. **Execve replaces the process** - No code after execve runs on success
5. **Exit status matters** - Programs communicate success/failure through exit codes

### Debugging Techniques

- Print file descriptors at each step (during development)
- Use `strace` to trace system calls
- Valgrind for memory leaks
- Test edge cases systematically

---

## 🔗 Resources

### Man Pages (Essential Reading)

- [man 2 pipe](https://man7.org/linux/man-pages/man2/pipe.2.html) - Create pipe
- [man 2 fork](https://man7.org/linux/man-pages/man2/fork.2.html) - Create process
- [man 2 dup2](https://man7.org/linux/man-pages/man2/dup.2.html) - Duplicate FD
- [man 2 execve](https://man7.org/linux/man-pages/man2/execve.2.html) - Execute program
- [man 2 waitpid](https://man7.org/linux/man-pages/man2/waitpid.2.html) - Wait for child
- [man 2 open](https://man7.org/linux/man-pages/man2/open.2.html) - Open file
- [man 3 perror](https://man7.org/linux/man-pages/man3/perror.3.html) - Error messages

### Books & Articles

- [The Linux Programming Interface](https://man7.org/tlpi/) - Comprehensive Unix/Linux reference
- [Advanced Programming in the UNIX Environment](https://www.apuebook.com/) - Classic systems programming book
- [Beej's Guide to Unix IPC](https://beej.us/guide/bgipc/) - Accessible IPC tutorial

### 42-Specific Resources

- [42 Docs - Pipex](https://harm-smits.github.io/42docs/projects/pipex) - Community documentation
- [Pipex Tutorial](https://csnotes.medium.com/pipex-tutorial-42-project-4469f5dd5901) - Step-by-step guide

---

## 🤖 AI Usage Disclosure

In accordance with 42's policy on AI transparency, here is how AI tools were used in this project:

### AI Tools Used

- **GitHub Copilot / Claude** - AI coding assistant

### Tasks Where AI Was Used

| Task | AI Involvement | Description |
|------|----------------|-------------|
| **Code Documentation** | ✅ Used | AI assisted in writing comprehensive comments and documentation for all source files, explaining the purpose of each function and the logic behind the implementation |
| **README Writing** | ✅ Used | AI helped structure and write this README file, including explanations of system calls, diagrams, and technical documentation |
| **Code Review** | ✅ Used | AI was used to review code for potential improvements and clarity |

### Tasks Where AI Was NOT Used

| Task | AI Involvement | Description |
|------|----------------|-------------|
| **System Call Understanding** | ❌ Not Used | Understanding of fork, pipe, dup2, execve came from reading man pages and documentation |
| **Core Implementation** | ❌ Not Used | All process management, pipe handling, and I/O redirection logic was written independently |
| **Debugging** | ❌ Not Used | All bugs were identified and fixed through manual testing and debugging |
| **Algorithm Design** | ❌ Not Used | The overall program flow was designed based on understanding Unix concepts |

### Why This Approach?

AI was used as a **documentation tool** to produce professional-quality documentation while ensuring the learning objectives were met through independent implementation of the core functionality.

---

## 🏆 Conclusion

Pipex is a foundational project that demystifies how Unix shells work. Understanding process creation, inter-process communication, and file descriptor management opens doors to:

- Building your own shell (minishell)
- Creating system utilities
- Understanding how servers handle multiple connections
- Debugging complex multi-process applications

The key takeaways:

1. **Unix is elegant** - A few simple primitives (fork, pipe, dup2, execve) enable powerful composition
2. **Processes are isolated** - Communication requires explicit channels
3. **File descriptors are universal** - Everything uses the same interface
4. **Error handling is critical** - Proper cleanup prevents resource leaks

---

<div align="center">
  <p><strong>Made with ❤️ at 42 Amman</strong></p>
  <p><em>mhaizan - 2026</em></p>
</div>
