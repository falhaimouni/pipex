/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 11:22:35 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/27 17:34:34 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path_in_env(char **envp)
{
	char	*path;

	path = find_path(envp);
	if (!path)
		return (NULL);
	return (path);
}

char	**split_path_dirs(char *path)
{
	char	**dir;

	dir = split_path(path);
	if (!dir)
		return (NULL);
	return (dir);
}

char	*check_access_and_build_path(char **dir, char *cmd)
{
	char	*full_path;
	int		i;

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

char	*get_path(char *cmd, char **envp)
{
	char	*path;
	char	**dir;
	char	*full_path;

	path = find_path_in_env(envp);
	if (!path)
		return (NULL);
	dir = split_path_dirs(path);
	if (!dir)
		return (NULL);
	full_path = check_access_and_build_path(dir, cmd);
	return (full_path);
}

void	free_path(char **cmd_args, char *cmd_path)
{
	free_cmd_args(cmd_args);
	free(cmd_path);
	mass("ERROR execute");
}
