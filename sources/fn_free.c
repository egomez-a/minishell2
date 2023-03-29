/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egomez-a <egomez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 13:24:56 by juasanto          #+#    #+#             */
/*   Updated: 2023/03/29 16:59:08 by egomez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_free_exe(t_exe *exe_commands)
{
	if (exe_commands->cmd)
		free(exe_commands->cmd);
	if (!exe_commands->args)
		return ;
	ft_free_array(exe_commands->args);
}

void	ft_free_envel(void *content)
{
	t_envel	*envel;

	if (!content)
		return ;
	envel = content;
	free(envel->name);
	free(envel->value);
	free(envel);
}

void	ft_tokenclear(t_list **commands, void (*del)(void *))
{
	t_list	*temp;

	if (commands)
	{
		while (*commands)
		{
			temp = (*commands)->next;
			ft_lstdelone(*commands, del);
			(*commands) = temp;
		}
	}
}

void	ft_free_token(void *content)
{
	t_token	*token;

	if (!content)
		return ;
	token = content;
	if (token->word != NULL)
		free(token->word);
	if (token->extvar != NULL)
		free(token->extvar);
	if (token)
		free(token);
}

void	ft_freemain(t_main *main)
{
	if (!main->line)
		free(main->line);
	if (!main->home)
		free(main->home);
	if (!main->temp_pwd)
		free(main->temp_pwd);
	if (!main->temp_oldpwd)
		free(main->temp_oldpwd);
	ft_lstclear(&main->envl, ft_free_envel);
	// free(main->envlist);
// 	free(main);
}
