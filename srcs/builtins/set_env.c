/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:26:19 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/09 21:58:18 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_set_env(char *var)
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

int	ft_set_env_kv(const char *key, const char *value)
{
	size_t	len;
	char	*var;

	len = ft_strlen(key) + 1 + ft_strlen(value) + 1;
	var = malloc(len);
	if (!var)
		return (-1);
	ft_strlcpy(var, key, len);
	ft_strlcat(var, "=", len);
	ft_strlcat(var, value, len);
	if (ft_set_env(var) < 0)
	{
		free(var);
		return (-1);
	}
	return (0);
}
