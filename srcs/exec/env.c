/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 22:42:30 by alavaud           #+#    #+#             */
/*   Updated: 2022/11/11 18:21:34 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_io(t_io *io)
{
	io->savedin = dup(0);
	if (io->savedin < 0)
		return (-1);
	io->savedout = dup(1);
	if (io->savedout < 0)
	{
		close(io->savedin);
		return (-1);
	}
	return (0);
}

int	restore_io(t_io *io)
{
	int	rv;

	rv = 0;
	if (dup2(io->savedin, 0) < 0 || dup2(io->savedout, 1) < 0)
	{
		rv = -1;
		perror("could not restore io");
	}
	close(io->savedout);
	close(io->savedin);
	return (rv);
}

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
			if (*var == '=')
				break ;
			++var;
		}
		if (*var == '=')
			return (var + 1);
	}
	return (NULL);
}
