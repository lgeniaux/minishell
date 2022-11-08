/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:26:19 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/08 13:35:35 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



static int	ft_set_env(char *var)
{
	int		i;
	int		pos;
	char	**copy;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		++i;
	}
	
    pos = ft_find_env(g_minishell.env, var, i);
	if (pos >= 0)
	{
		if (var[i] == '=')
		{
			free(g_minishell.env[pos]);
			g_minishell.env[pos] = var;
		}
		else
		{
			free(var);
		}
	}
	else
	{
        
		copy = ft_append_env(g_minishell.env, var);
		if (!copy)
			return (-1);
		free(g_minishell.env);
		g_minishell.env = copy;
	}
	return (0);
}

void set_oldpwd(char *oldpwd)
{
	char *tmp;

	tmp = ft_strjoin("OLDPWD=", oldpwd);
	ft_set_env(tmp);
}