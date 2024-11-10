/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: falhaimo <falhaimo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 12:28:15 by falhaimo          #+#    #+#             */
/*   Updated: 2024/11/10 11:25:55 by falhaimo         ###   ########.fr       */
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

#endif
