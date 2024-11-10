/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 11:22:35 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/10 11:28:07 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char **envp)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (path);
}

char	**split_path(char *path)
{
	return (ft_split(path, ':'));
}

char	*build_full_path(char *dir, char *cmd)
{
	int		dir_len;
	int		cmd_len;
	char	*full_path;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = (char *)malloc(dir_len + cmd_len + 2);
	if (full_path)
	{
		ft_strcpy(full_path, dir);
		full_path[dir_len] = '/';
		ft_strcpy(full_path + dir_len + 1, cmd);
	}
	return (full_path);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*get_path(char *cmd, char **envp)
{
	char	*path;
	char	**dir;
	char	*full_path;
	int		i;

	path = find_path(envp);
	if (!path)
		return (NULL);
	dir = split_path(path);
	if (!dir)
		return (NULL);
	i = 0;
	while (dir[i])
	{
		full_path = build_full_path(dir[i], cmd);
		if (!full_path)
		{
			free_split(dir);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free_split(dir);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split(dir);
	return (NULL);
}
