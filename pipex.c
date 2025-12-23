#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

static void    child1(int *fd, char **argv, char **envp)
{
    int     fd;
    char	**args;
	char	*cmd;

    if ((fd = open(argv[1], O_WRONLY, 0644)) == -1)
    {
        perror("Open failed");
        return;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    close(fd[1]);
    args = ft_split(argv[2], ' ');
	cmd = get_path(args[0], envp);
	if (!cmd)
		exit_cmd_error(args, argv[3]);
	execve(cmd, args, envp);
    perror("Execve failed");
    exit(1);
}

static void    child2(int *fd, char **argv, char **envp)
{
    int fd;
    char	**args;
	char	*cmd;

    if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        perror("Open failed");
        return;
    }
    dup2(fd[0], STDIN_FILENO);
    close(fd[1]);
    close(fd[0]);
    args = ft_split(argv[3], ' ');
	cmd = get_path(args[0], envp);
	if (!cmd)
		exit_cmd_error(args, argv[3]);
	execve(cmd, args, envp);
    perror("Execve failed");
    exit(1);
}

int main(int argc, char **argv, char **envp)
{
    int fd[2];
    pid_t pid1;
    pid_t pid2;

    if (argc < 5)
    {
        perror("Invalid arguments");
        return (-1);
    }
    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return (-1);
    }
    pid1 = fork();
    if (pid1 == 0)
        child1(fd, argv, envp);
    pid2 = fork();
    if (pid2 == 0)
        child2(fd, argv, envp);
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
