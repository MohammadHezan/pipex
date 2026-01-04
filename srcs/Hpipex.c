/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hpipex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 19:33:31 by mhaizan           #+#    #+#             */
/*   Updated: 2026/01/04 19:39:24 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Hpipex.h"

static void	execute_cmd(char *cmd_str, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd_str, ' ');
	if (!args || !args[0])
	{
		write(2, ": command not found\n", 21);
		ft_free_split(args);
		exit(127);
	}
	path = get_path(args[0], envp);
	if (!path)
	{
		write(2, args[0], ft_strlen(args[0]));
		write(2, ": command not found\n", 21);
		ft_free_split(args);
		exit(127);
	}
	execve(path, args, envp);
	ft_free_split(args);
	free(path);
	exit(127);
}

void	child1(int *fd, char **argv, char **envp)
{
	int	infile;

	infile = open(argv[1], O_RDONLY, 0644);
	if (infile == -1)
	{
		perror(argv[1]);
		close(fd[0]);
		close(fd[1]);
		exit(1);
	}
	dup2(infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(infile);
	close(fd[0]);
	close(fd[1]);
	execute_cmd(argv[2], envp);
}

void	child2(int *fd, char **argv, char **envp)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
	{
		perror(argv[4]);
		close(fd[0]);
		close(fd[1]);
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(outfile);
	close(fd[0]);
	close(fd[1]);
	execute_cmd(argv[3], envp);
}

static int	parent_wait(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;

	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
		return (write(2, "Invalid arguments\n", 18), -1);
	if (pipe(fd) == -1)
		return (perror("Pipe failed"), -1);
	pid1 = fork();
	if (pid1 == 0)
		child1(fd, argv, envp);
	pid2 = fork();
	if (pid2 == 0)
		child2(fd, argv, envp);
	close(fd[0]);
	close(fd[1]);
	return (parent_wait(pid1, pid2));
}
