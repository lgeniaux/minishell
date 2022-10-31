/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 22:42:30 by alavaud           #+#    #+#             */
/*   Updated: 2022/10/28 18:24:13 by alavaud          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_free(char **env)
{
	int	i;

	if (env)
	{
		i = 0;
		while (env[i])
			free(env[i++]);
		free(env);
	}
}

int	ft_find_env(char **env, const char *name, int len)
{
	int	i;
	int	j;

	i = 0;
	if (len < 0)
		len = ft_strlen(name);
	while (env[i])
	{
		j = 0;
		while (env[i][j] == name[j] && j < len)
			++j;
		if ((!env[i][j] || env[i][j] == '=') && j == len)
			return (i);
		++i;
	}
	return (-1);
}

char	*ft_getenv(char **env, const char *name, int len)
{
	int		pos;
	char	*var;

	if (len < 0)
		len = ft_strlen(name);
	pos = ft_find_env(env, name, len);
	if (pos >= 0)
	{
		var = env[pos];
		while (*var)
		{
			if (*var++ == '=')
				break ;
		}
		return (var);
	}
	return (NULL);
}
