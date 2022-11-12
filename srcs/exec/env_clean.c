/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_clean.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 17:23:19 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/12 17:23:48 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_has_value(const char *v)
{
	while (*v)
	{
		if (*v == '=')
			return (1);
		++v;
	}
	return (0);
}

char	**create_clean_env(char **env)
{
	char	**copy;
	int		len;
	int		i;
	int		j;

	i = 0;
	len = 0;
	while (env[i])
	{
		if (env_has_value(env[i]))
			++len;
		++i;
	}
	copy = ft_calloc(len + 1, sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	j = 0;
	while (env[i])
	{
		if (env_has_value(env[i]))
			copy[j++] = env[i];
		++i;
	}
	return (copy);
}
