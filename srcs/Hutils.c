/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hutils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:49:22 by mhaizan           #+#    #+#             */
/*   Updated: 2026/01/06 19:17:45 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Hpipex.h"

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

int	ft_close(int *fd, char *word)
{
	if (fd)
	{
		close(fd[0]);
		close(fd[1]);
	}
	if (word)
		perror(word);
	return (-1);
}

char	*find_in_paths(char **paths, char *cmd)
{
	char	*full;
	char	*tmp;
	int		i;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*res;
	int		i;

	i = 0;
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	res = find_in_paths(paths, cmd);
	ft_free_split(paths);
	return (res);
}

void	ft_execve(char **cmd, char **envp)
{
	char	*path;

	path = get_path(cmd[0], envp);
	if (!path)
	{
		write(2, cmd[0], ft_strlen(cmd[0]));
		write(2, ": command not found\n", 21);
		ft_free_split(cmd);
		exit(127);
	}
	if (execve(path, cmd, envp) == -1)
	{
		perror("execve");
		free(path);
		ft_free_split(cmd);
		exit(1);
	}
}
