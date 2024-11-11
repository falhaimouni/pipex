/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 14:10:14 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/11 11:54:02 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	handle_child_process(t_fds *fds, char *cmd, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(fds->in_fd, STDIN_FILENO) == -1
		|| dup2(fds->pipe_fd[1], STDOUT_FILENO) == -1)
		mass("ERROR dup in");
	close(fds->in_fd);
	close(fds->out_fd);
	close(fds->pipe_fd[0]);
	close(fds->pipe_fd[1]);
	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		free_cmd_args(cmd_args);
		mass("ERROR execute: command not found");
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

void	handle_parent_process(t_fds *fds, char *cmd, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(fds->pipe_fd[0], STDIN_FILENO) == -1
		|| dup2(fds->out_fd, STDOUT_FILENO) == -1)
		mass("ERROR dup1 in");
	close(fds->in_fd);
	close(fds->out_fd);
	close(fds->pipe_fd[0]);
	close(fds->pipe_fd[1]);
	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		free_cmd_args(cmd_args);
		mass("ERROR execute: command not found");
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
	close(fds->in_fd);
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
	if (pid == 0)
		handle_child_process(&fds, argv[2], envp);
	else
	{
		pid1 = create_child_process();
		if (pid1 == 0)
			handle_parent_process(&fds, argv[3], envp);
		close(fds.pipe_fd[0]);
		close(fds.pipe_fd[1]);
		waitpid(pid, NULL, 0);
		waitpid(pid1, NULL, 0);
		close_fds(&fds);
	}
	return (0);
}
