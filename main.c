/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 08:15:03 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/10 08:22:14 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
		handle_error("ERROR create pipe", 1);
}

pid_t	create_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		handle_error("ERROR forking", 1);
	return (pid);
}

void	close_pipes(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	wait_for_children(pid_t pid1, pid_t pid2)
{
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	int		in_fd;
	int		out_fd;
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
		handle_error("Usage : ./pipex infile cmd1 cmd2 outfile", 1);
	in_fd = open_file(argv[1], O_RDONLY, 0);
	out_fd = open_file(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	create_pipe(pipe_fd);
	pid1 = create_fork();
	if (pid1 == 0)
	{
		close(pipe_fd[0]);
		execute_command(argv[2], envp, in_fd, pipe_fd[1]);
	}
	pid2 = create_fork();
	if (pid2 == 0)
	{
		close(pipe_fd[1]);
		execute_command(argv[3], envp, pipe_fd[0], out_fd);
	}
	close_pipes(pipe_fd);
	return (wait_for_children(pid1, pid2), close(in_fd), close(out_fd), 0);
}
