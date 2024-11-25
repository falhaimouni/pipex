/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:34:12 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/25 11:44:20 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	mass(char *s)
{
	perror(s);
	exit(1);
}

void	open_files(char **argv, t_fds *fds)
{
	fds->in_fd = open(argv[1], O_RDONLY);
	if (fds->in_fd < 0)
		perror("ERROR");
	fds->out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fds->out_fd < 0)
	{
		close(fds->in_fd);
		mass("ERROR");
	}
}

void	create_pipe(t_fds *fds)
{
	if (pipe(fds->pipe_fd) == -1)
		perror("");
}

pid_t	create_child_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Error forking");
		exit(1);
	}
	return (pid);
}

void	free_cmd_args(char **cmd_args)
{
	int	i;

	if (!cmd_args)
		return ;
	i = 0;
	while (cmd_args[i])
	{
		free(cmd_args[i]);
		i++;
	}
	free(cmd_args);
}
