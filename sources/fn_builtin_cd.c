/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egomez-a <egomez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 11:13:35 by egomez-a          #+#    #+#             */
/*   Updated: 2023/03/28 12:44:35 by egomez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// caso cd 'a secas', sin argumentos
int	ft_cd_alone(t_main *main)
{
	if (!main->home)
	{
		write(2, "minishell: cd: HOME not set\n", 29);
		return (1);
	}
	main->temp_oldpwd = ft_strdup(main->temp_pwd);
	free(main->temp_pwd);
	main->temp_pwd = ft_strdup(main->home);
	chdir(main->temp_pwd);
	return (0);
}

// caso 'cd -' debe ir a OLDPWD
int	ft_cd_guion(t_main *main)
{
	char	*tmp;

	tmp = NULL;
	if (!main->temp_oldpwd)
	{
		write(2, "minishell: cd: OLDPWD not set\n", 30);
		return (1);
	}
	tmp = ft_strdup(main->temp_pwd);
	free(main->temp_pwd);
	main->temp_pwd = ft_strdup(main->temp_oldpwd);
	main->temp_oldpwd = ft_strdup(tmp);
	free(tmp);
	chdir(main->temp_pwd);
	return (0);
}

// caso cd path 
int	ft_cd_path(t_main *main, char *route)
{
	char	tmp[1024];

	if (open(route, O_RDONLY) < 0)
	{
		write(2, "minishell: cd: ", 15);
		write(2, route, ft_strlen(route));
		write(2, ": No such file or directory\n", 28);
		return (1);
	}
	main->temp_oldpwd = ft_strdup(main->temp_pwd);
	chdir(route);
	getcwd(tmp, 1024);
	free(main->temp_pwd);
	main->temp_pwd = ft_strdup(tmp);
	return (0);
}

char	*ft_cd_route(t_main *main, char **arguments)
{
	if (*arguments[1] == '/')
		return (ft_strdup(arguments[1]));
	else
		return (ft_path_suffix(main->temp_pwd, arguments[1]));
}

int	fn_cd(t_main *main)
{
	char	**arguments;
	char	*route;

	arguments = ft_split(main->line, ' ');
	key_envvariables(main);
	if (!arguments[1] || !ft_strcmp(arguments[1], "~"))
		main->ret = ft_cd_alone(main);
	else if (arguments[1] && !ft_strcmp(arguments[1], "-"))
		main->ret = ft_cd_guion(main);
	else if (arguments[1])
	{
		route = ft_cd_route(main, arguments);
		if (ft_check_cd(route) == 1)
		{
			free(main->temp_pwd);
			return (1);
		}
		main->ret = ft_cd_path(main, route);
		free(route);
	}
	update_key_envvariables(main);
	ft_free_array(arguments);
	return (main->ret);
}
