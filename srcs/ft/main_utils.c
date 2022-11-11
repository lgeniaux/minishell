/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alavaud <alavaud@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 18:49:24 by lgeniaux          #+#    #+#             */
/*   Updated: 2022/11/11 02:40:55 by alavaud          ###   ########lyon.fr   */
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
	char	buf[16];
	int		lvl;

	lvl = msh_get_shlvl(msh) + 1;
	return (ft_set_env_kv("SHLVL", ft_itoa(lvl, buf)));
}

int	msh_check_path(t_msh *msh)
{
	if (!ft_getenv(msh->env, "PATH", -1))
		return (ft_set_env_kv("PATH", "/usr/local/bin:/bin:/usr/bin:."));
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
