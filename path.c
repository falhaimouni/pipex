/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:26:42 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/10 08:26:49 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*get_path(char *cmd, char **envp)
{
	char	*path = NULL;
	int	i;
	char	**dir;
	char	*full_path;
	int	cmd_len;
	int	dir_len;
	int	j;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break;
		}
		i++;
	}
	if (!path)
		return (NULL);
	dir = ft_split(path, ':');
	if (!dir)
		return (NULL);
	cmd_len = ft_strlen(cmd);
	i = 0;
	while (dir[i])
	{
		dir_len = ft_strlen(dir[i]);
		full_path = (char *)malloc(dir_len + cmd_len + 2);
		if (!full_path)
			return (NULL);
		ft_strcpy(full_path, dir[i]);
		full_path[dir_len] = '/';
		ft_strcpy(full_path + dir_len + 1, cmd);
		if (access(full_path, X_OK) == 0)
		{
			j = 0;
			while (dir[j])
			{
				free (dir[j]);
				j++;
			}
			free(dir);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	j = 0;
	while (dir[j])
	{
		free(dir[j]);
		j++;
	}
	free(dir);
	return (NULL);
}

