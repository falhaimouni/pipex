/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:41:11 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/27 17:50:54 by falhaimo         ###   ########.fr       */
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
	char	**ozha;

	ozha = ft_split(path, ':');
	return (ozha);
}

char	*build_full_path(char *dir, char *cmd)
{
	int		dir_len;
	int		cmd_len;
	char	*full_path;

	if (!cmd || !cmd[0])
		return (NULL);
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = (char *)malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
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
