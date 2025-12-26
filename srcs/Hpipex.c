/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hpipex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:48:10 by mhaizan           #+#    #+#             */
/*   Updated: 2025/12/26 21:14:33 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Hpipex.h"

void	child1(int *fd, char **argv, char **envp)
{
	int		infile;
	char	**args;
	char	*cmd;

	infile = open(argv[1], O_RDONLY, 0644);
	if (infile == -1)
	{
		perror(argv[1]);
		exit(1);
	}
	dup2(infile, STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close(infile);
	close(fd[1]);
	close(fd[0]);
	args = ft_split(argv[2], ' ');
	cmd = get_path(args[0], envp);
	if (!cmd)
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
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
		perror(argv[4]);
		exit(1);
	}
	dup2(fd[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	close(outfile);
	args = ft_split(argv[3], ' ');
	cmd = get_path(args[0], envp);
	if (!cmd)
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
		ft_free_split(args);
		exit(127);
	}
	ft_execve(args, envp);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
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
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
