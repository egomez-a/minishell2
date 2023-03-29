/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_envcheck.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egomez-a <egomez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 15:36:18 by egomez-a          #+#    #+#             */
/*   Updated: 2023/03/28 15:40:13 by egomez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	fn_check_env_lvlshl(t_main *main)
{
	t_list	*list;
	t_envel	*envlist;
	int		check;

	list = main->envl;
	check = 0;
	while (list != NULL)
	{
		if (ft_strcmp((((t_envel *)list->content)->name), "SHLVL") == 0)
			check = 1;
		list = list->next;
	}
	if (check == 0)
	{
		envlist = fn_enve_new(ft_strdup("SHLVL=1"), "=1");
		ft_lstadd_back(&main->envl, ft_lstnew(envlist));
	}
}

void	fn_check_env_oldpwd(t_main *main)
{
	t_list	*list;
	t_envel	*envlist;
	int		check;

	list = main->envl;
	check = 0;
	while (list != NULL)
	{
		if (ft_strcmp((((t_envel *)list->content)->name), "OLDPWD") == 0)
			check = 1;
		list = list->next;
	}
	if (check == 0)
	{
		envlist = fn_enve_new(ft_strdup("OLDPWD=NULL"), "=NULL");
		ft_lstadd_back(&main->envl, ft_lstnew(envlist));
	}
	fn_check_env_lvlshl(main);
}

void	fn_check_env_pwd(t_main *main)
{
	t_list	*list;
	t_envel	*envlist;
	char	cwd[1024];
	int		check;

	list = main->envl;
	check = 0;
	while (list != NULL)
	{
		if (ft_strcmp((((t_envel *)list->content)->name), "PWD") == 0)
			check = 1;
		list = list->next;
	}
	if (check == 0)
	{
		envlist = fn_enve_new(ft_strjoin("PWD=", getcwd(cwd, sizeof(cwd))),
				ft_strjoin("=", getcwd(cwd, sizeof(cwd))));
		ft_lstadd_back(&main->envl, ft_lstnew(envlist));
	}
	fn_check_env_oldpwd(main);
}
