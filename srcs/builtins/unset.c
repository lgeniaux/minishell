/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 18:12:30 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/28 18:37:14 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_unset(char **env, const char *name)
{
	int i;

	i = ft_find_env(env, name, -1);
	if (i >= 0)
	{
		free(env[i]); /* TODO ergh */
		while (env[i])
		{
			env[i] = env[i + 1];
			++i;
		}
	}
}

int	builtin_unset(int argc, char *argv[])
{
	int i;

	i = 1;
	while (i < argc)
		ft_unset(g_minishell.env, argv[i++]);
	return (0);	
}
