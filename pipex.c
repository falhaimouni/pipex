/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 14:10:14 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/10 12:00:38 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int	in_fd;
	int	out_fd;
	int	pipe_fd[2];
	pid_t	pid;
	pid_t pid1;
	int	i;

	if (argc != 5)
	{
		perror("Usage : ./pipex infile cmd1 cmd2 outfile");
		exit(1);
	}
	in_fd = open(argv[1], O_RDONLY);
	if (in_fd < 0)
	{
		perror("ERROR open in");
		exit(1);
	}
	out_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd < 0)
	{
		perror("ERROR open out");
		close (in_fd);
		exit(1);
	}
	if (pipe(pipe_fd) == -1)
	{
		perror("ERROR create pipe");
		close(in_fd);
		close(out_fd);
		exit(1);
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
		char	**cmd1_args = ft_split(argv[2], ' ');
		char	*cmd1_path = get_path(cmd1_args[0], envp);
		if (!cmd1_path)
		{
			perror("ERROR find cmd1");
			i = 0;
			while (cmd1_args[i])
			{
				free(cmd1_args[i]);
				i++;
			}
			free(cmd1_args);
			exit(1);
		}

		if (execve(cmd1_path, cmd1_args, envp) == -1)
		{
			perror("ERROR execute 1");
			i = 0;
			while (cmd1_args[i])
			{
				free(cmd1_args[i]);
				i++;
			}
			free(cmd1_args);
			exit(1);
		}
		i = 0;
			while (cmd1_args[i])
			{
				free(cmd1_args[i]);
				i++;
			}
			free(cmd1_args);
			exit(1);
	}
	else 
	{
		
		pid1 = fork();
		if (pid1 < 0)
		{
			perror("ERROR forking");
			close(in_fd);
			close(out_fd);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			return (1);
		}
		if(pid1 == 0)
		{
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
			char	**cmd2_args = ft_split(argv[3], ' ');
			char	*cmd2_path = get_path(cmd2_args[0], envp);
			if (!cmd2_path)
			{
				perror("ERROR find cmd2");
				i = 0;
				while (cmd2_args[i])
				{
					free(cmd2_args[i]);
					i++;
				}
				free(cmd2_args);
				exit(1);
			}

			if (execve(cmd2_path, cmd2_args, envp) == -1)
			{
				perror("ERROR execute 3");
				while (cmd2_args[i])
				{
					free(cmd2_args[i]);
					i++;
				}
				free(cmd2_args);
				exit(1);
			}
			while (cmd2_args[i])
				{
					free(cmd2_args[i]);
					i++;
				}
				free(cmd2_args);
				free(cmd2_path);
		}

	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid, NULL,0);
	waitpid(pid1, NULL, 0);
	close(in_fd);
	close(out_fd);
	return (0);
}
