/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hutils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:49:22 by mhaizan           #+#    #+#             */
/*   Updated: 2025/12/26 21:14:53 by mhaizan          ###   ########.fr       */
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

char	**get_paths_array(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split(envp[i] + 5, ':'));
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

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = get_paths_array(envp);
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
		fprintf(stderr, "%s: command not found\n", cmd[0]);
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
