/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egomez-a <egomez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 11:51:25 by juasanto          #+#    #+#             */
/*   Updated: 2023/03/27 14:57:24 by egomez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fn_exe_cm(t_main *main)
{
	main->pid.p1 = fork();
	if (main->pid.p1 < 0)
		perror("Error Fork P1\n");
	if (main->pid.p1 == 0)
		execve("pwd", NULL, main->env);
	return (0);
}

char	**make_arr_comd(char *cmd, char **args)
{
	int		i;
	int		j;
	int		cont;
	char	**arr_com;

	i = 0;
	j = 0;
	cont = 0;
	while (args[i] != NULL)
	{
		cont++;
		i++;
	}
	arr_com = (char **)malloc(sizeof(char *) * (cont + 2));
	i = 0;
	arr_com[i++] = cmd;
	while (args[j] != NULL)
	{
		arr_com[i] = args[j];
		i++;
		j++;
	}
	arr_com[i] = NULL;
	return (arr_com);
}

void	check_first_token(t_main *main)
{
	char	**argument_list;
	char	**paths_matrix;
	char	**f_tok_w_path;
	int		i;

	argument_list = make_arr_comd(main->exe_commands[0].cmd ,main->exe_commands[0].args);
	paths_matrix = paths_from_env(main);
	f_tok_w_path = ft_calloc(ft_matrixlen(paths_matrix) + 1, sizeof(char *));
	i = -1;
	while (++i < ft_matrixlen(paths_matrix))
		f_tok_w_path[i] = ft_strjoin(paths_matrix[i], main->exe_commands[0].cmd);
	i = -1;
	while (++i < ft_matrixlen(paths_matrix))
	{
		if (access(f_tok_w_path[i], X_OK) == 0)
		{
			printf ("El path %d %s se puede leer", i, f_tok_w_path[i]);
			if (execve(f_tok_w_path[i], argument_list, NULL) != -1)
				printf("1er token %s SI se puede ejecutar\n", f_tok_w_path[i]);
			break ;
		}
	}
	free(f_tok_w_path);
	free(paths_matrix);
	free(argument_list);
}

void	export_token(t_main *main)
{
	t_list	*tokens;
	int		i;
	int		cont_aux;

	i = 0;
	tokens = (t_list *)main->commands;
	print_list(tokens);
	main->num_cmd = count_pipes(tokens) + 1;
	printf("Num PIPES: %d\n", count_pipes(tokens));
	main->exe_commands = ft_calloc(sizeof (t_exe), main->num_cmd);
	while (i < main->num_cmd)
	{
		first_token_cmd(tokens);
		init_command(main, i);
        while (tokens != NULL && ((t_token *)tokens->content)->type != PIPE)
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
                    tokens = tokens->next;
                }
            }
            else if (((t_token *)tokens->content)->type == HERE)
            {
				printf("Estoy aqui\n");
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
			if (tokens != NULL)
				tokens = tokens->next;
			i++;
        }
    }
}

int	main(int argc, char **argv, char **env)
{
	t_main	*main;

	main = NULL;
	main = fn_init_main(main, argc, argv, env);
	ft_env_list(main, env);
	while (main->ex == 0)   // esto me da problemas ... tenemos que revisarlo
	{
		main->line = check_prompt(argc, argv);
		if (main->line[0] != '\0')
		{
			main->lenght_line = ft_strlen(main->line);
			if (main->line[0] == '9')
				break;
			if (fn_clean_quotes(main) != 0)
			{
				printf("Número impar de quotes\n");
				main->ret = 1;
				exit(255); //TODO: Llevar a mensaje de error.
			}
			main->tokens = fn_token_list(main);
			export_token(main);
			print_comands(main);
			if (check_builtins(main) == 1)
				fn_check_word(main->commands, main);
			else
				check_first_token(main);
		}
		if (main->ex != 1)
			free(main->line);
		ft_lstclear(&main->commands, ft_free_token);
		ft_free_array(main->exe_commands->args);
	}
	ft_freemain(main);
	system("leaks -q minishell");
	return (main->ret);
}
