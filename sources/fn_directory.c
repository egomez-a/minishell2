/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_directory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egomez-a <egomez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 12:30:04 by egomez-a          #+#    #+#             */
/*   Updated: 2023/03/28 12:34:21 by egomez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*update_oldpwd(t_main *main)
{
	char	dir_temp[1024];
	char	*path;

	path = ft_strdup(getcwd(dir_temp, 1024));
	main->temp_oldpwd = ft_strdup(main->temp_pwd);
	main->temp_pwd = ft_strdup(path);
	free(path);
	return (main->temp_pwd);
}

void	update_key_envvariables(t_main *main)
{
	t_list	*list;

	list = main->envl;
	while (list != NULL)
	{
		if (ft_strcmp((((t_envel *)list->content)->name), "OLDPWD") == 0)
			((t_envel *)list->content)->value = ft_strdup(main->temp_oldpwd);
		if (ft_strcmp((((t_envel *)list->content)->name), "PWD") == 0)
			((t_envel *)list->content)->value = ft_strdup(main->temp_pwd);
		list = list->next;
	}
}

void	key_envvariables(t_main *main)
{
	t_list	*list;

	list = main->envl;
	while (list != NULL)
	{
		if (ft_strcmp ((((t_envel *)list->content)->name), "HOME") == 0)
			main->home = ft_strdup(((t_envel *)list->content)->value);
		if (ft_strcmp ((((t_envel *)list->content)->name), "OLDPWD") == 0)
			main->temp_oldpwd = ft_strdup(((t_envel *)list->content)->value);
		if (ft_strcmp ((((t_envel *)list->content)->name), "PWD") == 0)
			main->temp_pwd = ft_strdup(((t_envel *)list->content)->value);
		list = list->next;
	}
	return ;
}

int	ft_check_cd(char *directory)
{
	DIR		*dire;

	dire = opendir(directory);
	if (!dire)
	{
		perror ("minishell");
		return (1);
	}
	closedir(dire);
	return (0);
}

char	*ft_path_suffix(char *path, char *suffix)
{
	char	*s1;
	char	*s2;

	s1 = ft_strjoin(path, "/");
	s2 = ft_strjoin(s1, suffix);
	free(s1);
	return (s2);
}
