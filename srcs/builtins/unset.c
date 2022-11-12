/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:12:30 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/12 13:47:47 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_unset(char **env, const char *name)
{
	int	i;

	i = ft_find_env(env, name, -1);
	if (i >= 0)
	{
		free(env[i]);
		while (env[i])
		{
			env[i] = env[i + 1];
			++i;
		}
	}
}

int	builtin_unset(int argc, char *argv[])
{
	int	rv;
	int	i;

	rv = 0;
	i = 1;
	while (i < argc)
	{
		if (!is_valid_ident(argv[i]))
		{
			rv = 1;
			ft_putstr_fd("minishell: unset: ", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
		}
		else
		{
			ft_unset(g_minishell.env, argv[i]);
		}
		++i;
	}
	return (rv);
}
