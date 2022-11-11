/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:43:03 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 13:06:38 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_append_env(char **env, char *var)
{
	char	**copy;
	int		i;

	i = 0;
	while (env[i])
		++i;
	copy = ft_calloc(i + 2, sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (env[i])
	{
		copy[i] = env[i];
		++i;
	}
	copy[i] = var;
	return (copy);
}

static int	compare_keys(const char *a, const char *b)
{
	int	i;

	i = 0;
	while (a[i] && b[i])
	{
		if (b[i] == '=' || a[i] == '=')
			break ;
		++i;
	}
	return (ft_strncmp(a, b, i));
}

void	sort_vars(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (compare_keys(env[i], env[j]) > 0)
			{
				tmp = env[j];
				env[j] = env[i];
				env[i] = tmp;
			}
			++j;
		}
		++i;
	}
}
