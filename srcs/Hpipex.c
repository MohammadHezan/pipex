/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hpipex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/26 20:48:10 by mhaizan           #+#    #+#             */
/*   Updated: 2026/01/04 19:30:41 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Hpipex.h"

void	ft_close_all(int *fd, int file)
{
	close(fd[0]);
	close(fd[1]);
	if (file >= 0)
		close(file);
}

void	child1(int *fd, char **argv, char **envp)
{
	int		infile;
	char	**args;
	char	*cmd;

	infile = open(argv[1], O_RDONLY, 0644);
	if (infile == -1)
	{
		perror(argv[1]);
		ft_close_all(fd, infile);
		exit(1);
	}
	dup2(infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	ft_close_all(fd, infile);
	args = ft_split(argv[2], ' ');
	if (!args || !args[0])
	{
		write(2, ": command not found\n", 21);
		ft_free_split(args);
		exit(127);
	}
	cmd = get_path(args[0], envp);
	if (!cmd)
	{
		write(2, args[0], ft_strlen(args[0]));
		write(2, ": command not found\n", 21);
		ft_free_split(args);
		exit(127);
	}
	ft_execve(args, envp);
}

void	child2(int *fd, char **argv, char **envp)
{
	char	**args;
	char	*cmd;
	int		outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == -1)
	{
		ft_close_all(fd, outfile);
		perror(argv[4]);
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	ft_close_all(fd, outfile);
	args = ft_split(argv[3], ' ');
	if (!args || !args[0])
	{
		write(2, ": command not found\n", 21);
		ft_free_split(args);
		exit(127);
	}
	cmd = get_path(args[0], envp);
	if (!cmd)
	{
		write(2, args[0], ft_strlen(args[0]));
		write(2, ": command not found\n", 21);
		ft_free_split(args);
		exit(127);
	}
	ft_execve(args, envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	int		status1;
	int		status2;
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
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
	ft_close_all(fd, -1);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}
