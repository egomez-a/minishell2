/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfgarci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 13:48:07 by alfgarci          #+#    #+#             */
/*   Updated: 2023/04/06 01:48:09 by alfgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*add_slash(char *str)
{
	char	*ret;

	ret = ft_strjoin(str, "/");
	free(str);
	return (ret);
}

static char	**get_path(char **envp)
{
	int		i;
	int		j;
	char	**path;

	i = -1;
	j = -1;
	path = NULL;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp[i] + 5, ':');
			if (!path)
				return (NULL);
			while (path[++j])
			{
				if (path[j][ft_strlen(path[j]) - 1] != '/')
					path[j] = add_slash(path[j]);
			}
			break ;
		}
	}
	return (path);
}

static int	check_path(char **cmd, char **envp)
{
	int		i;
	char	**path;
	char	*tmp;

	i = 0;
	path = get_path(envp);
	if (!path)
		return (0);
	while (path[++i])
	{
		tmp = ft_strjoin(path[i], *cmd);
		if (access(tmp, F_OK) == 0)
		{
			free_split(path);
			free(*cmd);
			*cmd = tmp;
			return (1);
		}
		free(tmp);
	}
	free_split(path);
	if (access(*cmd, F_OK) == 0)
		return (1);
	ft_printf("%s: Command not found\n", cmd[0]);
	exit(-1);
}

char	**get_cmd(const char *arg_cmd, char **envp)
{
	char	**cmd;

	cmd = ft_split(arg_cmd, ' ');
	if (!cmd)
		return (NULL);
	check_path(cmd, envp);
	return (cmd);
}
