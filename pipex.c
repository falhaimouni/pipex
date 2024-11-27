/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 14:10:14 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/27 17:52:53 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	closed_fds(t_fds *fds)
{
	if (fds->in_fd >= 0)
		close(fds->in_fd);
	if (fds->out_fd >= 0)
		close(fds->out_fd);
	if (fds->pipe_fd[0] >= 0)
		close(fds->pipe_fd[0]);
	if (fds->pipe_fd[1] >= 0)
		close(fds->pipe_fd[1]);
}

void	handle_child_process(t_fds *fds, char *cmd, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(fds->in_fd, STDIN_FILENO) == -1 || dup2(fds->pipe_fd[1],
			STDOUT_FILENO) == -1)
	{
		perror("Error");
		closed_fds(fds);
		exit(1);
	}
	closed_fds(fds);
	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		free_cmd_args(cmd_args);
		write(2, "ERROR execute: command not found\n", 34);
		exit(1);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
		free_path(cmd_args, cmd_path);
	free_cmd_args(cmd_args);
	free(cmd_path);
}

void	handle_parent_process(t_fds *fds, char *cmd, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(fds->pipe_fd[0], STDIN_FILENO) == -1 || dup2(fds->out_fd,
			STDOUT_FILENO) == -1)
		mass("ERROR dup1 in");
	closed_fds(fds);
	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		free_cmd_args(cmd_args);
		write(2, "ERROR execute: command not found\n", 34);
		exit(1);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		free_cmd_args(cmd_args);
		free(cmd_path);
		mass("ERROR execute");
	}
	free_cmd_args(cmd_args);
	free(cmd_path);
}

void	close_fds(t_fds *fds)
{
	if (fds->in_fd >= 0)
		close(fds->in_fd);
	if (fds->out_fd >= 0)
		close(fds->out_fd);
}

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	pid_t	pid1;
	t_fds	fds;

	if (argc != 5)
		mass("Usage: ./pipex infile cmd1 cmd2 outfile");
	open_files(argv, &fds);
	create_pipe(&fds);
	pid = create_child_process();
	if (pid == 0 && fds.in_fd != -1)
		handle_child_process(&fds, argv[2], envp);
	else
	{
		pid1 = create_child_process();
		if (pid1 == 0)
			handle_parent_process(&fds, argv[3], envp);
		close(fds.pipe_fd[0]);
		close(fds.pipe_fd[1]);
		waitpid(-1, NULL, 0);
		waitpid(-1, NULL, 0);
		close_fds(&fds);
	}
	return (0);
}
