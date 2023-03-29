/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_builtin_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egomez-a <egomez-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/25 11:16:28 by egomez-a          #+#    #+#             */
/*   Updated: 2023/03/29 15:45:35 by egomez-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_strnum(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
	}
	return (1);
}

int	fn_exit1(char *temp)
{
	printf("exit\n");
	if (ft_atoi(temp) > 255)
	{
		free(temp);
		return (ft_atoi(temp - 256));
	}
	else
	{
		free(temp);
		return (ft_atoi(temp));
	}
}

int	fn_exit(t_main *main)
{
	char	*temp;

	temp = ft_strdup(((t_token *)main->commands->next->content)->word);
	main->ex = 1;
	if (ft_lstsize(main->commands) > 2)
	{
		printf("minishell: exit: too many arguments\n");
		main->ex = 0;
		free(temp);
		return (1);
	}
	else if (ft_lstsize(main->commands) == 2 && !is_strnum(temp))
	{
		printf("minishell: exit: %s: numeric argument required\n", temp);
		free(temp);
		return (255);
	}
	else if (ft_lstsize(main->commands) == 2)
		return (fn_exit1(temp));
	else
	{
		free(temp);
		return (0);
	}
}
