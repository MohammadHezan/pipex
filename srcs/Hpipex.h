/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hpipex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 20:48:32 by mhaizan           #+#    #+#             */
/*   Updated: 2026/01/06 18:44:19 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HPIPEX_H
# define HPIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include "../libft/libft.h"

void	ft_free_split(char **split);
char	*find_in_paths(char **paths, char *cmd);
char	*get_path(char *cmd, char **envp);
void	ft_execve(char **cmd, char **envp);
int		ft_close(int *fd, char *word);

void	child1(int *fd, char **argv, char **envp);
void	child2(int *fd, char **argv, char **envp);

#endif
