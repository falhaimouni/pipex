/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 14:10:14 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/06 14:02:05 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	handle_error(const char *msg, int exit_code)
{
	perror(msg);
	exit(exit_code);
}

void	execute_command(char *cmd, char **envp, int in_fd, int out_fd)
{
	char	**cmd_args;
	char	*cmd_path;

	if (dup2(in_fd, STDIN_FILENO) == -1)
		handle_error("ERROR dup in", 1);
	if (dup2(out_fd, STDOUT_FILENO) == -1)
		handle_error("ERROR dup out", 1);
	close(in_fd);
	close(out_fd);
	cmd_args = ft_split(cmd, ' ');
	cmd_path = get_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		perror("ERROR find command");
		free_split(cmd_args);
		exit(1);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
	{
		perror("ERROR execute");
		free_split(cmd_args);
		free(cmd_path);
		exit(1);
	}
	free_split(cmd_args);
	free(cmd_path);
}

int	open_file(const char *filename, int flags, mode_t mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd < 0)
		handle_error("ERROR open file", 1);
	return (fd);
}

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
	wait_for_children(pid1, pid2);
	close(in_fd);
	close(out_fd);
	return (0);
}
