/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgeniaux <lgeniaux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 18:49:24 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/10 21:55:52 by lgeniaux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_get_shlvl(t_msh *msh)
{
	char	*shlvl;
	int		i;

	shlvl = ft_getenv(msh->env, "SHLVL", -1);
	if (shlvl)
	{
		i = 0;
		while (ft_isdigit(shlvl[i]))
			++i;
		if (i > 0 && !shlvl[i])
			return (ft_atoi(shlvl));
	}
	return (0);
}

int	msh_update_shlvl(t_msh *msh)
{
	char	*buf;
	int		n;
	int		lvl;
	int		rv;

	buf = malloc(32);
	if (!buf)
		return (-1);
	lvl = msh_get_shlvl(msh) + 1;
	n = ft_strlcpy(buf, "SHLVL=", 32);
	ft_itoa(lvl, buf + n);
	rv = ft_set_env(buf);
	if (rv < 0)
		free(buf);
	return (rv);
}

int	msh_check_path(t_msh *msh)
{
	char	*path;
	int		rv;

	path = ft_getenv(msh->env, "PATH", -1);
	if (!path)
	{
		path = ft_strdup("PATH=/usr/local/bin:/bin:/usr/bin:.");
		if (!path)
			return (-1);
		rv = ft_set_env(path);
		if (rv < 0)
			free(path);
		return (rv);
	}
	return (0);
}

void	msh_exit(int code)
{
	g_minishell.exit_code = code;
	g_minishell.should_exit = 1;
}

static void	free_envp(int i, char **clone)
{
	while (i-- > 0)
	{
		free(clone[i]);
	}
	free(clone);
}

char	**clone_env(char **envp)
{
	char	**clone;
	int		i;

	i = 0;
	while (envp[i])
		++i;
	clone = ft_calloc(i + 1, sizeof(char *));
	if (clone)
	{
		i = 0;
		while (envp[i])
		{
			clone[i] = ft_strdup(envp[i]);
			if (!clone[i])
			{
				free_envp(i, clone);
				return (NULL);
			}
			++i;
		}
	}
	return (clone);
}
