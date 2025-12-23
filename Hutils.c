#include <fcntl.h>

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

char	*get_path(char *cmd, char **envp)
{
	char	**paths;
	char	*full;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd)); 
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		full = ft_strjoin(paths[i++], "/");
		full = ft_strjoin(full, cmd);
		if (access(full, X_OK) == 0 && access(full, F_OK) == 0)
		{
			ft_free_split(paths);
			return (full);
		}
		free(full);
	}
	ft_free_split(paths);
	return (NULL);
}
