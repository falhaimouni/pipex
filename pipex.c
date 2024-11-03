/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 14:10:14 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/03 14:40:35 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	int	in_fd;
	int	out_fd;
	int	pipe_fd[2];
	pid_t	pid;

	if (argc != 5)
	{
		write(2, "Usage : ./pipex infile cmd1 cmd2 outfile\n", 41);
		return (1);
	}
	in_fd = open(argv[1], O_RDONLY);
	if (in_fd < 0)
	{
		perror("ERROR open in");
		return (1);
	}
	out_fd = open(argv[4], O_WRONLY | O_CREAT, 0644);
	if (out_fd < 0)
	{
		perror("ERROR open out");
		close (in_fd);
		return (1);
	}
	if (pipe(pipe_fd) == -1)
	{
		perror("ERROR create pipe");
		close(in_fd);
		close(out_fd);
		return (1);
	}
	pid = fork();
	if (pid < 0)
	{
		perror("ERROR forking");
		close(in_fd);
		close(out_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	if (pid == 0)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("ERROR dup in");
			exit(1);
		}
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("ERROR dup out");
			exit(1);
		}
		close(in_fd);
		close(out_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		char	*cmd1_args[] = {argv[2], NULL};

		if (execve(cmd1_args[0], cmd1_args, NULL) == -1)
		{
			perror("ERROR execute 1");
			exit(1);
		}
	}
	else
	{
		waitpid(pid, NULL, 0);
		if (dup2(pipe_fd[0],STDIN_FILENO) == -1)
		{
			perror("ERROR dup1 in");
			exit(1);
		}
		if (dup2(out_fd,STDOUT_FILENO) == -1)
		{
			perror("ERROR dup1 out");
			exit(1);
		}
		close(in_fd);
		close(out_fd);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		char	*cmd2_args[] = {argv[3], NULL};

		if (execve(cmd2_args[0], cmd2_args, NULL) == -1)
		{
			perror("ERROR execute 3");
			exit(1);
		}

	}
	close(in_fd);
	close(out_fd);
	return (0);
}
