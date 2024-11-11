/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 12:28:15 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/11 11:38:16 by falhaimo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_fds
{
	int	in_fd;
	int	out_fd;
	int	pipe_fd[2];
}	t_fds;

char	**ft_split(char const *s, char c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strlen(const char *s);
char	*ft_strcpy(char *dest, char *src);
char	*ft_strdup(const char *s);
char	*find_path(char **envp);
char	**split_path(char *path);
char	*build_full_path(char *dir, char *cmd);
void	free_split(char **split);
char	*get_path(char *cmd, char **envp);
void	mass(char *s);
void	open_files(char **argv, t_fds *fds);
void	create_pipe(t_fds *fds);
pid_t	create_child_process(void);
void	free_cmd_args(char **cmd_args);
void	handle_child_process(t_fds *fds, char *cmd, char **envp);
void	handle_parent_process(t_fds *fds, char *cmd, char **envp);
void	close_fds(t_fds *fds);
char	*find_path_in_env(char **envp);
char	**split_path_dirs(char *path);
char	*check_access_and_build_path(char **dir, char *cmd);
char	*get_path(char *cmd, char **envp);

#endif
