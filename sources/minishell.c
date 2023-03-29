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

	i = -1;
	cont = 0;
	j = -1;
	while (args[++i] != NULL)
		cont++;
	arr_com = (char **)malloc(sizeof(char *) * (cont + 2));
	i = 0;
	arr_com[i++] = cmd;
	while (args[++j] != NULL)
	{
		arr_com[i] = args[j]; 
		i++;
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

	argument_list = make_arr_comd(main->exe_commands->cmd ,main->exe_commands->args);
	paths_matrix = paths_from_env(main);
	f_tok_w_path = ft_calloc(ft_matrixlen(paths_matrix) + 1, sizeof(char *));
	i = -1;
	while (++i < ft_matrixlen(paths_matrix))
		f_tok_w_path[i] = ft_strjoin(paths_matrix[i],
				(((t_token *)main->commands->content)->word));
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
	t_list	*aux;
	int		i;
	int		cont_aux;
	int		j;
	int		num_com;

	i = -1;
	tokens = (t_list *)main->commands;
	first_token_cmd(tokens);
	print_list(tokens);
	num_com = count_pipes(tokens) + 1;
	printf("Num PIPES: %d\n", count_pipes(tokens));
	main->exe_commands = ft_calloc(sizeof (t_exe), num_com);
	while (++i < num_com )
	{
		cont_aux = 0;
		if (((t_token *)tokens->content)->type == CMD)
		{
			main->exe_commands[i]->cmd = ((t_token *)tokens->content)->word;
			tokens = tokens->next;
			if (((t_token *)tokens->content)->type == ARG)
			{
				aux = tokens;
				while (((t_token *)aux->content)->type == ARG)
				{
					cont_aux++;
					aux = aux->next;
				}
				main->exe_commands[i]->args = ft_calloc(sizeof(char *), cont_aux);
				j = -1;
				while (((t_token *)tokens->content)->type == ARG)
				{
					main->exe_commands[i]->args[++j] = ((t_token *)tokens->content)->word ;
					tokens = tokens->next;
				}


			}
			else if (((t_token *)tokens->content)->type == HERE)
			{

			}

		}
		main->exe_commands->cmd = ((t_token *)tokens->content)->word;
		while ()
		main->exe_commands->cmd = ((t_token *)tokens->content)->word;
		main->exe_commands->args = args_list_into_matrix(main);
		main->exe_commands->fd_in = ((t_token *)tokens->content)->fd;
		main->exe_commands->fd_out = ((t_token *)tokens->content)->fd;
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
