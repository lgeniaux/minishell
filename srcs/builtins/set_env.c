/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 18:26:19 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 02:32:59 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_set_env_utils(const char *var, int pos, int i)
{
	char	**copy;
	char	*tmp;

	tmp = ft_strdup(var);
	if (!tmp)
		return (-1);
	if (pos >= 0)
	{
		if (var[i] == '=')
		{
			free(g_minishell.env[pos]);
			g_minishell.env[pos] = tmp;
		}
		else
			free(tmp);
	}
	else
	{
		copy = ft_append_env(g_minishell.env, tmp);
		if (!copy)
		{
			free(tmp);
			return (0);
		}
		free(g_minishell.env);
		g_minishell.env = copy;
	}
	return (1);
}

int	ft_set_env(const char *var)
{
	int		i;
	int		pos;

	i = 0;
	while (var[i])
	{
		if (var[i] == '=')
			break ;
		++i;
	}
	pos = ft_find_env(g_minishell.env, var, i);
	if (!ft_set_env_utils(var, pos, i))
		return (-1);
	return (0);
}

int	ft_set_env_kv(const char *key, const char *value)
{
	size_t	len;
	char	*var;
	int		rv;

	len = ft_strlen(key) + 1 + ft_strlen(value) + 1;
	var = malloc(len);
	if (!var)
		return (-1);
	ft_strlcpy(var, key, len);
	ft_strlcat(var, "=", len);
	ft_strlcat(var, value, len);
	rv = ft_set_env(var);
	free(var);
	return (rv);
}
