/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_builtin_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egomez-a <egomez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 11:14:41 by egomez-a          #+#    #+#             */
/*   Updated: 2023/03/29 14:34:49 by egomez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** export imprime "declare -x" y luego los nombres ordenados por ascii de las
** variables de entorno, luego '=' y luego las variables de entorno entre
** comillas. Si no hay variables de entorno tienen que estar las mínimas que
** son PWD y SHLVL. 
** Si pongo export hola=adios, sale al final una nueva var de entorno
** hola="adios"
** Para hacerlo meto variables de entorno en una matriz **temp
*/

char	**env_to_matrix(int num, t_list *list)
{
	char	**temp;
	int		i;
	char	*aux;

	temp = ft_calloc(num + 1, sizeof(char *));
	i = 0;
	while (i < num)
	{
		aux = ft_strjoin(((t_envel *)list->content)->name, "=\"");
		temp[i] = ft_strjoin(aux, ((t_envel *)list->content)->value);
		free(aux);
		i++;
		list = list->next;
	}
	return (temp);
}

void	order_matrix(char **matrix, int num)
{
	int		i;
	int		j;
	char	*temp;

	i = 1;
	while (i < num)
	{
		j = 0;
		while (j < num - i)
		{
			if (ft_strcmp(matrix[j], matrix[j + 1]) > 0)
			{
				temp = ft_strdup(matrix[j]);
				matrix[j] = ft_strdup(matrix[j + 1]);
				matrix[j + 1] = ft_strdup(temp);
			}
			j++;
		}
		i++;
	}
}

int	order_env_matrix(char **matrix, int num)
{
	char	**mat_ord;
	int		i;

	mat_ord = ft_calloc(num + 1, sizeof(char *));
	i = 0;
	while (matrix[i])
	{
		mat_ord[i] = ft_strdup(matrix[i]);
		i++;
	}
	mat_ord[i] = NULL;
	order_matrix(mat_ord, num);
	i = 0;
	while (i < num)
	{
		printf("declare -x %s\"\n", mat_ord[i]);
		i++;
	}
	ft_free_array(mat_ord);
	return (i);
}

int	fn_export(t_main *main)
{
	char	**temp;
	t_list	*list;
	t_envel	*temparg;
	int		num;
	int		i;

	list = main->envl;
	if (ft_matrixlen(main->exe_commands->args) != 0)
	{
		i = 0;
		while (main->exe_commands->args[i])
		{
			temparg = fn_enve_new(main->exe_commands->args[i],
					ft_strchr(main->exe_commands->args[i], '='));
			ft_lstadd_back(&main->envl, ft_lstnew(temparg));
			i++;
		}
		return (0);
	}
	num = ft_lstsize(list);
	temp = env_to_matrix(num, list);
	main->ret = order_env_matrix(temp, num);
	return (0);
}
