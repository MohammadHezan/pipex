/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hpipex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:48:32 by mhaizan           #+#    #+#             */
/*   Updated: 2025/12/26 20:58:01 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HPIPEX_H
# define HPIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>

int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	**ft_split(const char *s, char c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strchr(const char *s, int c);

void	ft_free_split(char **split);
char	*find_in_paths(char **paths, char *cmd);
char	**get_paths_array(char **envp);
char	*get_path(char *cmd, char **envp);
void	ft_execve(char **cmd, char **envp);

void	child1(int *fd, char **argv, char **envp);
void	child2(int *fd, char **argv, char **envp);

#endif