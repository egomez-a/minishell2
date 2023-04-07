/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alfgarci <alfgarci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 11:52:52 by alfgarci          #+#    #+#             */
/*   Updated: 2023/03/28 15:52:52 by alfgarci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_args(t_main *main, t_list *tokens, int num)
{
	t_list	*save;
	int		cont_arg;
	int		i;

	i = 0;
	cont_arg = 0;
	save = tokens;
	while (save != NULL && ((t_token *)save->content)->type == ARG)
	{
		cont_arg++;
		save = save->next;
	}
	main->exe_commands[num].args = ft_calloc(sizeof(char *), cont_arg + 1);
	while (tokens != NULL && ((t_token *)tokens->content)->type == ARG)
	{
		main->exe_commands[num].args[i] = ((t_token *)tokens->content)->word ;
		i++;
		tokens = tokens->next;
	}
	main->exe_commands[num].args[i] = NULL;
	return (cont_arg);
}

int	is_redirect(t_list *tokens)
{
	if (tokens == NULL)
		return (0);
	if (((t_token *)tokens->content)->type == LESS
		|| ((t_token *)tokens->content)->type == MORE
		|| ((t_token *)tokens->content)->type == HERE
		|| ((t_token *)tokens->content)->type == APPEND)
	{
		return (1);
	}
	else
		return (0);
}

int	add_redirect_here(t_main *main, t_list *tokens, int num)
{
	tokens = tokens->next;
	if (tokens == NULL)
	{
		main->exe_commands[num].is_here = -1;
	}
	else if (((t_token *)tokens->content)->type == ARG)
	{
		main->exe_commands[num].is_here = 1;
		main->exe_commands[num].lim = ft_strdup(((t_token *)tokens->content)->word);
		return (1);
	}
	else
		main->exe_commands[num].is_here = -1;
	return (1);
}

int	add_redirect_in(t_main *main, t_list *tokens, int num)
{
	int	fd;

	tokens = tokens->next;
	if (tokens != NULL && ((t_token *)tokens->content)->type == ARG)
	{
		if (access(((t_token *)tokens->content)->word, F_OK | R_OK) == 0)
		{
			fd = open(((t_token *)tokens->content)->word, O_RDONLY);
			main->exe_commands[num].fd_in = fd;
		}
	}
	else
		main->exe_commands[num].fd_in = -1;
	return (1);
}

int	add_redirect_out(t_main *main, t_list *tokens, int num)
{
	int	fd;

	tokens = tokens->next;

	if (tokens != NULL && ((t_token *)tokens->content)->type == ARG)
	{
		fd = open(((t_token *)tokens->content)->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		main->exe_commands[num].fd_out = fd;
	}
	else
		main->exe_commands[num].fd_out = -1;
	return (1);
}

int	add_redirect_append(t_main *main, t_list *tokens, int num)
{
	int	fd;

	tokens = tokens->next;
	if (tokens != NULL && ((t_token *)tokens->content)->type == ARG)
	{
		fd = open(((t_token *)tokens->content)->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
		main->exe_commands[num].fd_out = fd;
	}
	else

		main->exe_commands[num].fd_out = -1;
	return (1);
}

void	init_command(t_main *main, int num)
{
	main->exe_commands[num].cmd = NULL;
	main->exe_commands[num].args = NULL;
	main->exe_commands[num].fd_in = 0;
	main->exe_commands[num].fd_out = 1;
	main->exe_commands[num].is_here = 0;
	main->exe_commands[num].lim = NULL;
}
