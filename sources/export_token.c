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

void	export_token(t_main *main)
{
	t_list	*tokens;
	int		i;
	int		cont_aux;
	int		num_com;

	i = -1;
	tokens = (t_list *)main->commands;
	first_token_cmd(tokens);
	print_list(tokens);
	num_com = count_pipes(tokens) + 1;
	printf("Num PIPES: %d\n", count_pipes(tokens));
	main->exe_commands = ft_calloc(sizeof (t_exe), num_com);
	printf("111111\n");
	while (++i < num_com )
	{
        while (tokens != NULL && ((t_token *)tokens->content)->type == PIPE)
        {
            cont_aux = 0;
            if (((t_token *)tokens->content)->type == CMD)
            {
                main->exe_commands[i].cmd = ((t_token *)tokens->content)->word;
                tokens = tokens->next;
                if (tokens == NULL)
                    break ;
                if (((t_token *)tokens->content)->type == ARG)
                {
                    cont_aux = add_args(main, tokens, i);
                    while (cont_aux-- > 0)
                        tokens = tokens->next;
                }
                while (is_redirect(tokens) == 1)
                {
                    if (((t_token *)tokens->content)->type == LESS)
                        cont_aux = add_redirect_in(main, tokens, i);
                    else if (((t_token *)tokens->content)->type == HERE)
                        cont_aux = add_redirect_here(main, tokens, i);
                    else if (((t_token *)tokens->content)->type == MORE)
                        cont_aux = add_redirect_out(main, tokens, i);
                    else if (((t_token *)tokens->content)->type == APPEND)
                        cont_aux = add_redirect_append(main, tokens, i);
                    while (cont_aux-- > 0)
                        tokens = tokens->next;
                }
            }
            else if (((t_token *)tokens->content)->type == HERE)
            {
                cont_aux = add_redirect_append(main, tokens, i);
                if (cont_aux == 0)
                    break ;
                tokens = tokens->next;
                main->exe_commands[i].cmd = ((t_token *)tokens->content)->word;
                tokens = tokens->next;
                if (tokens == NULL)
                    break ;
                if (((t_token *)tokens->content)->type == ARG)
                {
                    cont_aux = add_args(main, tokens, i);
                    while (cont_aux-- > 0)
                        tokens = tokens->next;
                }
                while (is_redirect(tokens) == 1)
                {
                    if (((t_token *)tokens->content)->type == LESS)
                        cont_aux = add_redirect_in(main, tokens, i);
                    else if (((t_token *)tokens->content)->type == HERE)
                        cont_aux = add_redirect_here(main, tokens, i);
                    else if (((t_token *)tokens->content)->type == MORE)
                        cont_aux = add_redirect_out(main, tokens, i);
                    else if (((t_token *)tokens->content)->type == APPEND)
                        cont_aux = add_redirect_append(main, tokens, i);
                    while (cont_aux-- > 0)
                        tokens = tokens->next;
                }
            }
        }
    }
}